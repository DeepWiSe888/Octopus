/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : eth_mac_send.v
 * Description   : eth_mac_send
 * *********************************************************************/
module  eth_mac_send(
        input                   clk                             ,
        input                   rst_n                           ,
        input                   udp_tx                          ,
        input                   arp_ack_tx                      ,
        input                   arp_req                      ,
        //Eth
        input         [15: 0]   ip_len                          ,
        input         [47: 0]   src_mac                         ,
        input         [47: 0]   dst_mac                         ,
        input         [15: 0]   udp_type                        ,
        input         [15: 0]   arp_type                        ,
        input         [ 7: 0]   ip_data                         ,
        input         [ 7: 0]   arp_data                        ,
        output  reg             udp_trig                        ,
        output  reg             arp_ack_trig                    ,
        output  reg             arp_req_trig                    ,
        //crc
        input         [31: 0]   crc                             ,
        output  reg             init                            ,
        output  reg             en                              ,
        //gmii
        output  reg             gmii_tx_en                      ,
        output  wire            gmii_tx_er                      ,
        output  reg   [ 7: 0]   gmii_tx_data 
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
parameter   IDLE        =       2'd0                            ;
parameter   ETH_HEADER  =       2'd1                            ;
parameter   PACK        =       2'd2                            ;
parameter   CRC         =       2'd3                            ;
reg     [ 1: 0]                 state_c /*synthesis preserve*/  ; //时序逻辑变量
reg     [ 1: 0]                 state_n /*synthesis keep*/      ; //组合逻辑变量
//cnt
reg     [15: 0]                 cnt_num                         ;
reg     [15: 0]                 cnt                             ;
wire                            add_cnt                         ;
wire                            end_cnt                         ;
//eth
reg     [15: 0]                 eth_type                        ;
reg                             flag_udp                        ;
reg                             flag_arp_ack                    ;
reg                             flag_arp_req                    ;
reg     [47: 0]                 eth_dst_mac                     ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/
assign  gmii_tx_er  =   1'b0;


reg arp_reg1;
reg arp_reg2;
wire arp_req_tx;
always  @(posedge clk or negedge rst_n)begin

    if(rst_n==0)begin

        arp_reg2 <= 0;
        arp_reg1 <= 0;
    end
    else
    begin
        arp_reg1 <= arp_req;
        arp_reg2 <= arp_reg1;
    end
end

assign arp_req_tx = (!arp_reg2) & arp_reg1;
//eth_type
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        eth_type    <=  16'd0;
    end
    else if(udp_tx)begin
        eth_type    <=  udp_type;
    end
    else if(arp_ack_tx || arp_req_tx)begin
        eth_type    <=  arp_type;
    end
end

//eth_dst_mac
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        eth_dst_mac <=  48'hFF_FF_FF_FF_FF_FF;
    end
    else if(udp_tx || arp_ack_tx)begin
        eth_dst_mac <=  dst_mac; 
    end
    else if(arp_req_tx)begin
        eth_dst_mac <=  48'hFF_FF_FF_FF_FF_FF;
    end
end

//flag_udp
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        flag_udp    <=  1'b0;
    end
    else if(udp_tx)begin
        flag_udp    <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        flag_udp    <=  1'b0;
    end
end

//flag_arp_ack
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        flag_arp_ack    <=  1'b0;
    end
    else if(arp_ack_tx)begin
        flag_arp_ack    <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        flag_arp_ack    <=  1'b0;
    end
end

//flag_arp_req
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        flag_arp_req    <=  1'b0;
    end
    else if(arp_req_tx)begin
        flag_arp_req    <=  1'b1;
    end
    else if((state_c == CRC) && end_cnt)begin
        flag_arp_req    <=  1'b0;
    end
end

//cnt_num
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        cnt_num <=  16'd0;
    end
    else if(state_n == ETH_HEADER)begin
        cnt_num <=  16'd22;
    end
    else if(state_n == PACK)begin
        if(flag_udp)begin
            cnt_num <=  ip_len;
        end
        else if(flag_arp_ack || flag_arp_req)begin
            cnt_num <=  16'd46;
        end
    end
    else if(state_n == CRC)begin
        cnt_num <=  16'd4;
    end
    else begin
        cnt_num <=  16'd0;
    end
end

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

assign  add_cnt     =       state_c != IDLE;       
assign  end_cnt     =       add_cnt && cnt == cnt_num - 1;   

//udp_trig 
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_trig    <=  1'b0;
    end
    else if((flag_udp) && (state_c == ETH_HEADER) && (cnt == 16'd19))begin
        udp_trig    <=  1'b1;
    end
    else begin
        udp_trig    <=  1'b0;
    end
end

//arp_ack_trig
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_ack_trig    <=  1'b0;
    end
    else if((flag_arp_ack) && (state_c == ETH_HEADER) && (cnt == 16'd19))begin
        arp_ack_trig    <=  1'b1;
    end
    else begin
        arp_ack_trig    <=  1'b0;
    end
end

//arp_req_trig
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_req_trig    <=  1'b0;
    end
    else if((flag_arp_req) && (state_c == ETH_HEADER) && (cnt == 16'd19))begin
        arp_req_trig    <=  1'b1;
    end
    else begin
        arp_req_trig    <=  1'b0;
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
            if(udp_tx || arp_ack_tx || arp_req_tx)begin
                state_n = ETH_HEADER;
            end
            else begin
                state_n = state_c;
            end
        end
        ETH_HEADER:begin
            if(end_cnt)begin
                state_n = PACK;
            end
            else begin
                state_n = state_c;
            end
        end
        PACK:begin
            if(end_cnt)begin
                state_n = CRC;
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

//gmii_tx_data 
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        gmii_tx_data    <=  8'h00;
    end
    else if(state_c == ETH_HEADER)begin
        case(cnt)
            //前导码和分隔符
            0,1,2,3,4,5,6:
                gmii_tx_data    <=  8'h55;
            7:      gmii_tx_data    <=  8'hd5;
            //目的mac地址
            8:      gmii_tx_data    <=  eth_dst_mac[47:40];
            9:      gmii_tx_data    <=  eth_dst_mac[39:32];
            10:     gmii_tx_data    <=  eth_dst_mac[31:24];
            11:     gmii_tx_data    <=  eth_dst_mac[23:16];
            12:     gmii_tx_data    <=  eth_dst_mac[15: 8];
            13:     gmii_tx_data    <=  eth_dst_mac[ 7: 0];
            //源mac地址
            14:     gmii_tx_data    <=  src_mac[47:40];
            15:     gmii_tx_data    <=  src_mac[39:32];
            16:     gmii_tx_data    <=  src_mac[31:24];
            17:     gmii_tx_data    <=  src_mac[23:16];
            18:     gmii_tx_data    <=  src_mac[15: 8];
            19:     gmii_tx_data    <=  src_mac[ 7: 0];
            //以太网帧类型
            20:     gmii_tx_data    <=  eth_type[15: 8];
            21:     gmii_tx_data    <=  eth_type[ 7: 0];
            default:begin
                gmii_tx_data    <=  8'h00;
            end
        endcase
    end
    else if(state_c == PACK)begin
        if(flag_udp)begin
            gmii_tx_data    <=  ip_data;
        end
        else if(flag_arp_ack || flag_arp_req)begin
            gmii_tx_data    <=  arp_data;
        end
    end
    else if(state_c == CRC)begin
        case(cnt)
            0:  gmii_tx_data    <=  {~crc[24], ~crc[25], ~crc[26], ~crc[27], ~crc[28], ~crc[29], ~crc[30], ~crc[31]};
            1:  gmii_tx_data    <=  {~crc[16], ~crc[17], ~crc[18], ~crc[19], ~crc[20], ~crc[21], ~crc[22], ~crc[23]};
            2:  gmii_tx_data    <=  {~crc[8], ~crc[9], ~crc[10], ~crc[11], ~crc[12], ~crc[13], ~crc[14], ~crc[15]};
            3:  gmii_tx_data    <=  {~crc[0], ~crc[1], ~crc[2], ~crc[3], ~crc[4], ~crc[5], ~crc[6], ~crc[7]};
            default:begin
                gmii_tx_data    <=  8'h00;
            end
        endcase
    end
end

//gmii_tx_en 
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        gmii_tx_en  <=  1'b0;
    end
    else if(state_c != IDLE)begin
        gmii_tx_en  <=  1'b1;
    end
    else begin
        gmii_tx_en  <=  1'b0;
    end
end

//init
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        init    <=  1'b0;
    end
    else if((state_c ==  ETH_HEADER) && add_cnt && (cnt == 8-1))begin
        init    <=  1'b1;
    end
    else begin
        init    <=  1'b0;
    end
end

//en    
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        en  <=  1'b0;
    end
    else if((state_c ==  ETH_HEADER) && add_cnt && (cnt == 9-1))begin
        en  <=  1'b1;
    end
    else if((state_c == CRC) && add_cnt && (cnt == 1-1))begin
        en  <=  1'b0;
    end
end


endmodule
