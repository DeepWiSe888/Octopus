/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : eth_receive.v
 * Description   : eth_receive
 * *********************************************************************/
module  eth_receive(
        input                   clk                             ,
        input                   rst_n                           ,
        //gmii
        input                   gmii_rx_dv                      ,
        input         [ 7: 0]   gmii_rx_data                    ,
        input                   gmii_rx_er                      ,
        output  reg             arp_ack_tx                      ,
        output  reg   [47: 0]   dst_mac                         ,
        output  wire            cmd_flag                        ,
        output  wire  [ 7: 0]   cmd_data                        
       
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
parameter   IDLE            =   3'd0                            ;
parameter   FRAME_START     =   3'd1                            ;
parameter   ETH_HEADER      =   3'd2                            ;
parameter   ARP             =   3'd3                            ;
parameter   IP_HEADER       =   3'd4                            ;
parameter   UDP_HEADER      =   3'd5                            ;
parameter   UDP_DATA        =   3'd6                            ;
parameter   CRC             =   3'd7                            ;
reg     [ 2: 0]                 state_c   /*synthesis preserve*/;
reg     [ 2: 0]                 state_n   /*synthesis keep*/    ;
//cnt
reg     [15: 0]                 cnt_num                         ;
reg     [15: 0]                 cnt                             ;
wire                            add_cnt                         ;
wire                            end_cnt                         ;
//cnt1
reg     [ 2: 0]                 cnt1                            ;
wire                            add_cnt1                        ;
wire                            end_cnt1                        ;
//arp
reg     [47: 0]                 src_mac                         ;
reg     [15: 0]                 opcode                          ;
reg                             opcode_ack                      ;
reg                             opcode_req                      ;
reg     [47: 0]                 dst_mac_tmp                     ;
reg     [31: 0]                 arp_src_ip                      ;
reg     [31: 0]                 arp_dst_ip                      ;
reg                             arp_src_ip_ok                   ;
reg                             arp_dst_ip_ok                   ;
//udp
reg     [ 7: 0]                 protocol                        ;
reg                             protocol_udp                    ;
reg     [31: 0]                 udp_dst_ip                      ;
reg                             udp_dst_ip_ok                   ;
reg     [15: 0]                 udp_len                         ;
reg     [15: 0]                 src_port                        ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/
//cnt
always @(posedge clk or negedge rst_n)begin
    if(!rst_n)begin
        cnt <= 0;
    end
    else if(add_cnt)begin
        if(end_cnt)
            cnt <= 0;
        else
            cnt <= cnt + 1;
    end
    else begin
        cnt <=  0;
    end
end

assign  add_cnt     =       (state_c != FRAME_START) && gmii_rx_dv && (gmii_rx_er == 1'b0);       
assign  end_cnt     =       add_cnt && cnt == cnt_num-1;   

//cnt_num
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        cnt_num <=  16'd1;
    end
    else if(state_n == ETH_HEADER)begin
        cnt_num <=  16'd14;
    end
    else if(state_n == ARP)begin
        cnt_num <=  16'd46;
    end
    else if(state_n == IP_HEADER)begin
        cnt_num <=  16'd20;
    end
    else if(state_n == UDP_HEADER)begin
        cnt_num <=  16'd8;
    end
    else if(state_n == UDP_DATA)begin
        if(udp_len <= 16'd26) //udp数据不足18位时，不足的位会补0，所以udp数据至少会发18位，udp_len = udp_header_len + udp_data = 8 + udp_data;
            cnt_num <=  16'd18;
        else begin
            cnt_num <=  udp_len - 16'd8;
        end
    end
    else if(state_n == CRC)begin
        cnt_num <=  16'd4;
    end
    else begin
        cnt_num <=  16'd1;
    end
end

//state_c
always@(posedge clk or negedge rst_n)begin
    if(!rst_n)begin
        state_c <= IDLE;
    end
    else begin
        state_c <= state_n;
    end
end

//state_n
always@(*)begin
    case(state_c)
        IDLE:begin
            if(end_cnt1)begin
                state_n = FRAME_START;
            end
            else begin
                state_n = state_c;
            end
        end
        FRAME_START:begin
            if(gmii_rx_dv && (gmii_rx_data == 8'hd5))begin
                state_n = ETH_HEADER;
            end
            else begin
                state_n = state_c;
            end
        end
        ETH_HEADER:begin
            if(end_cnt && (src_mac != 48'h00_0a_35_01_fe_c0) && (src_mac != 48'hff_ff_ff_ff_ff_ff))begin 
                state_n = IDLE;
            end
            else if(end_cnt && ((src_mac == 48'h00_0a_35_01_fe_c0) || (src_mac == 48'hff_ff_ff_ff_ff_ff)) && (gmii_rx_data == 8'h06))begin//eth_type只检测其低8位，06:arp;00:ip;
                state_n = ARP;
            end
            else if(end_cnt && (src_mac == 48'h00_0a_35_01_fe_c0) && (gmii_rx_data == 8'h00))begin
                state_n = IP_HEADER;
            end
            else begin
                state_n = state_c;
            end
        end
        ARP:begin
            if(end_cnt)begin
                state_n = CRC;
            end
            else if(gmii_rx_dv == 1'b0)begin
                state_n = IDLE;
            end
            else begin
                state_n = state_c;
            end
        end
        IP_HEADER:begin
            if(end_cnt && ((protocol_udp == 1'b0) || (udp_dst_ip_ok == 1'b0)))begin
                state_n = IDLE;
            end
            else if(end_cnt && protocol_udp && udp_dst_ip_ok)begin
                state_n = UDP_HEADER;
            end
            else begin
                state_n = state_c;
            end
        end
        UDP_HEADER:begin
            if(end_cnt && (src_port != 16'd5000))begin
                state_n = IDLE;
            end
            else if(end_cnt && (src_port == 16'd5000))begin
                state_n = UDP_DATA;
            end
            else begin
                state_n = state_c;
            end
        end
        UDP_DATA:begin
            if(end_cnt)begin
                state_n = CRC;
            end
            else if(gmii_rx_dv == 1'b0)begin
                state_n = IDLE;
            end
            else begin
                state_n = state_c;
            end
        end
        CRC:begin
            if(end_cnt)begin
                state_n = IDLE;
            end
            else begin
                state_n = state_c;
            end
        end
        default:begin
            state_n = IDLE;
        end
    endcase
end

//cnt1
always @(posedge clk or negedge rst_n)begin
    if(!rst_n)begin
        cnt1 <= 0;
    end
    else if(add_cnt1)begin
        if(end_cnt1)
            cnt1 <= 0;
        else
            cnt1 <= cnt1 + 1;
    end
    else begin
        cnt1    <=  0;
    end
end

assign  add_cnt1        =       (gmii_rx_dv && (gmii_rx_data == 8'h55) && (state_c == IDLE));       
assign  end_cnt1        =       add_cnt1 && cnt1 == 7-1;


//src_mac fpga的mac地址 
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        src_mac <=  48'd0;
    end
    else if((state_c == ETH_HEADER) && add_cnt && (cnt <= 6-1))begin
        src_mac <=  {src_mac[39:0],gmii_rx_data};
    end
end

//******************************ARP begin*******************************************
//opcode
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        opcode  <=  16'd0;
    end
    else if((state_c == ARP) && add_cnt && (cnt >= 7-1) && (cnt <= 8-1))begin
        opcode  <=  {opcode[7:0],gmii_rx_data};
    end
end

//opcode_ack
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        opcode_ack  <=  1'b0;
    end
    else if((state_c == ARP) && add_cnt && (cnt == 9-1) && (opcode == 16'h0002))begin
        opcode_ack  <=  1'b1;
    end
    else if(end_cnt)begin
        opcode_ack  <=  1'b0;
    end
end

//opcode_req
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        opcode_req  <=  1'b0;
    end
    else if((state_c == ARP) && add_cnt && (cnt == 9-1) && (opcode == 16'h0001))begin
        opcode_req  <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        opcode_req  <=  1'b0;
    end
end

//dst_mac_tmp
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        dst_mac_tmp <=  48'd0;
    end
    else if((state_c == ARP) && add_cnt && (cnt >= 9-1) && (cnt <= 14-1))begin
        dst_mac_tmp <=  {dst_mac_tmp[39:0],gmii_rx_data};
    end
end

//arp_dst_ip
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_dst_ip  <=  32'hc0_a8_00_03;
    end
    else if((state_c == ARP) && add_cnt && (cnt >= 15-1) && (cnt <= 18-1))begin
        arp_dst_ip  <=  {arp_dst_ip[23:0],gmii_rx_data};
    end
end

//arp_dst_ip_ok
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_dst_ip_ok   <=  1'b0;
    end
    else if((state_c == ARP) && add_cnt && (cnt == 19-1) && (arp_dst_ip == 32'hc0_a8_00_03))begin
        arp_dst_ip_ok   <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        arp_dst_ip_ok   <=  1'b0;
    end
end

//arp_src_ip
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_src_ip  <=  32'hc0_a8_00_02;
    end
    else if((state_c == ARP) && add_cnt && (cnt >= 25-1) && (cnt <= 28-1))begin
        arp_src_ip  <=  {arp_src_ip[23:0],gmii_rx_data};
    end
end

//arp_src_ip_ok
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_src_ip_ok   <=  1'b0;
    end
    else if((state_c == ARP) && add_cnt && (cnt == 29-1) && (arp_src_ip == 32'hc0_a8_00_02))begin
        arp_src_ip_ok   <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        arp_src_ip_ok   <=  1'b0;
    end
end

//dst_mac
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        dst_mac <=  48'hFF_FF_FF_FF_FF_FF;
    end
    else if((opcode_ack || opcode_req) && arp_dst_ip_ok && arp_src_ip_ok)begin
        dst_mac <=  dst_mac_tmp;
    end
end

//arp_ack_tx
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_ack_tx  <=  1'b0;
    end
    else if((state_c == CRC) && end_cnt && opcode_req && arp_src_ip_ok)begin
        arp_ack_tx  <=  1'b1;
    end
    else begin
        arp_ack_tx  <=  1'b0;
    end
end
//******************************ARP end*********************************************

//******************************UDP begin*******************************************
//protocol
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        protocol    <=  8'h00;
    end
    else if((state_c == IP_HEADER) && add_cnt && (cnt == 10-1))begin
        protocol    <=  gmii_rx_data;
    end
end

//protocol_udp
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        protocol_udp    <=  1'b0;
    end
    else if((state_c == IP_HEADER) && add_cnt && (cnt == 11-1) && (protocol == 8'h11))begin
        protocol_udp    <=  1'b1;
    end
    else if(end_cnt)begin
        protocol_udp    <=  1'b0;
    end
end

//udp_dst_ip
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_dst_ip  <=  32'd0;
    end
    else if((state_c == IP_HEADER) && add_cnt && (cnt >= 13-1) && (cnt <= 16-1))begin
        udp_dst_ip  <=  {udp_dst_ip[23:0],gmii_rx_data};
    end
end

//udp_dst_ip_ok
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_dst_ip_ok   <=  1'b0;
    end
    else if((state_c == IP_HEADER) && add_cnt && (cnt == 17-1) && (udp_dst_ip == 32'hc0_a8_00_03))begin
        udp_dst_ip_ok   <=  1'b1;
    end
    else if(end_cnt)begin
        udp_dst_ip_ok   <=  1'b0;
    end
end

//src_port fpga端口
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        src_port    <=  16'd0;
    end
    else if((state_c == UDP_HEADER) && add_cnt && (cnt >= 3-1) && (cnt <= 4-1))begin
        src_port    <=  {src_port[7:0],gmii_rx_data};
    end
end

//udp_len
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_len <=  16'd0;
    end
    else if((state_c == UDP_HEADER) && add_cnt && (cnt >= 5-1) && (cnt <= 6-1))begin
        udp_len <=  {udp_len[7:0],gmii_rx_data};
    end
end

//udp data write to ram

// reg [ 2: 0]   byte_counter;
// reg [31: 0]   mydata; 
// reg [15: 0]   data_counter;
// always  @(posedge clk or negedge rst_n)
// begin
//     if(rst_n == 1'b0)
//     begin
//         byte_counter  <=  3'h0;
//         mydata        <= 32'h0;
//         data_counter  <= 16'h0;
//     end
//     else 
//     begin
//         if(cmd_flag==1'b1) 
//         begin
//                download_ram_len  <= udp_len;
//                if (data_counter == udp_len  - 16'd9) 
//                begin         //存最后的数据
//                     data_counter<=0;
            
//                     download_ram_waddr        <= download_ram_waddr+1'b1; 
//                     download_ram_wren  <=  1'b1;               //写RAM
//                     download_ram_done  <=  1'b1;                            
//                     if(byte_counter==3'd3) begin
//                           download_ram_wdata <={mydata[23:0],gmii_rx_data[7:0]};
//                           byte_counter<=0;
//                      end
//                     else if(byte_counter==3'd2) begin
//                           download_ram_wdata <={mydata[15:0],gmii_rx_data[7:0],8'h00};       //不满32bit,补0
//                           byte_counter<=0;
//                      end
//                     else if(byte_counter==3'd1) begin
//                           download_ram_wdata <={mydata[7:0],gmii_rx_data[7:0],16'h0000};     //不满32bit,补0
//                           byte_counter<=0;
//                      end
//                     else if(byte_counter==3'd0) begin
//                          download_ram_wdata <={gmii_rx_data[7:0],24'h000000};              //不满32bit,补0
//                           byte_counter<=0;
//                      end
//                 end
//                 else begin
//                      data_counter <= data_counter+1'b1;
//                     if(byte_counter < 3'd3)   begin
//                           mydata<={mydata[23:0],gmii_rx_data[7:0]};
//                           byte_counter<=byte_counter+1'b1;
//                           download_ram_wren<=1'b0;  
//                      end
//                      else begin
//                           download_ram_wdata <={mydata[23:0],gmii_rx_data[7:0]};
//                           byte_counter<=3'd0;
//                           download_ram_wren<=1'b1;                        //接受4byes数据,写ram请求
//                           download_ram_waddr<=download_ram_waddr+1'b1;                              
//               end   
//           end                            
//         end
//         else    
//         begin
//             data_counter      <= 16'h0; 
//             download_ram_wren <=  1'b0;   
//             byte_counter      <=  3'h0;
//             download_ram_waddr<=  9'h0;
//             mydata            <= 32'h0; 
//             download_ram_done <=  1'b0; 
//         end    
//     end
// end

//cmd_flag
assign  cmd_flag    =   (state_c == UDP_DATA) ? 1'b1 : 1'b0;
assign  cmd_data    =   gmii_rx_data;
//******************************UDP end*********************************************

endmodule
