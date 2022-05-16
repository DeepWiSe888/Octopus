/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : eth_udp_send.v
 * Description   : eth_udp_send
 * *********************************************************************/
module  eth_udp_send(
        input                   clk                             ,
        input                   rst_n                           ,
        //Eth
        input         [15: 0]   src_port                        ,
        input         [15: 0]   dst_port                        ,
        input         [31: 0]   src_ip                          ,
        input         [31: 0]   dst_ip                          ,
        input         [15: 0]   udp_data_len                    ,
        input                   udp_trig                        ,
        output  reg   [15: 0]   ip_len                          ,
        output  reg   [ 7: 0]   ip_data                         ,
        //wfifo
        input   wire  [ 7: 0]   wfifo_rd_data                   ,
        output  reg             wfifo_rd_en                     ,
        // input   wire  [31: 0]   datain                          ,
        // output  reg   [8:0]     ram_rd_addr                     ,
        output  reg             send_finish 

);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
parameter   IDLE        =       2'd0                            ;
parameter   IP_HEADER   =       2'd1                            ;
parameter   UDP_HEADER  =       2'd2                            ;
parameter   UDP_DATA    =       2'd3                            ;
reg     [ 1: 0]                 state_c /*synthesis preserve*/  ;
reg     [ 1: 0]                 state_n /*synthesis keep*/      ;
//cnt1
reg     [15: 0]                 cnt1_num                        ;
reg     [15: 0]                 cnt1                            ;
wire                            add_cnt1                        ;
wire                            end_cnt1                        ;
//check_sum
wire    [31: 0]                 ip_header[ 2: 0]                ;
reg     [31: 0]                 check_buffer                    ;
wire    [15: 0]                 check_sum                       ;
reg     [15: 0]                 udp_len                         ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/
assign  ip_header[0]        =   {8'h45,8'h00,ip_len};
assign  ip_header[1]        =   {8'h00,8'h00,8'h00,8'h00};
assign  ip_header[2]        =   {8'h40,8'h11,8'h00,8'h00};

//udp_len
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_len <=  16'd0;
    end
    else begin
        udp_len <=  udp_data_len + 16'd8;
    end
end

//ip_len
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        ip_len   <=  16'h0000;
    end
    else begin
        ip_len   <=  udp_len + 16'd20;
    end
end

//check_buffer
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        check_buffer    <=  32'd0;
    end
    else if(state_c == IP_HEADER)begin
        if(add_cnt1 && cnt1 == 1-1)begin
            check_buffer    <=  ip_header[0][15:0] + ip_header[0][31:16] + ip_header[1][15:0] + ip_header[1][31:16] + 
                ip_header[2][15:0] + ip_header[2][31:16] + src_ip[15:0] + src_ip[31:16] + dst_ip[15:0] + dst_ip[31:16];
        end
        else if(add_cnt1 && cnt1 == 2-1)begin
            check_buffer    <=  check_buffer[31:16] + check_buffer[15:0];
        end
        else if(add_cnt1 && cnt1 == 3-1)begin
            check_buffer    <=  ~check_buffer;
        end
    end
end

//check_sum
assign  check_sum       =   check_buffer[15: 0];

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
end

assign  add_cnt1        =       state_c != IDLE;       
assign  end_cnt1        =       add_cnt1 && cnt1 == cnt1_num-1;

//cnt1_num
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        cnt1_num    <=  16'd0;
    end
    else if(state_n == IP_HEADER)begin
        cnt1_num    <=  16'd20;
    end
    else if(state_n == UDP_HEADER)begin
        cnt1_num    <=  16'd8;
    end
    else if(state_n == UDP_DATA)begin
        cnt1_num    <=  udp_data_len ;
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
            send_finish <= 1;
            if(udp_trig)begin
                state_n = IP_HEADER;
               
            end
            else begin
                state_n = state_c;
            end
        end
        IP_HEADER:begin
            send_finish <= 0;
            if(end_cnt1)begin
                state_n = UDP_HEADER;
            end
            else begin
                state_n = state_c;
            end
        end
        UDP_HEADER:begin
            if(end_cnt1)begin
                state_n = UDP_DATA;
            end
            else begin
                state_n = state_c;
            end
        end
        UDP_DATA:begin
            if(end_cnt1)begin
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

//ip_data
reg     [15:0]                 tx_data_counter                        ;
reg     [4:0]                  i                                      ;
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        ip_data         <=  8'h00;
        tx_data_counter <=  16'h0;
    end
    else if(state_c == IP_HEADER)begin
        case(cnt1)
            //ver hdr_len,tos,total_len
            00:ip_data  <=  ip_header[0][31:24];
            01:ip_data  <=  ip_header[0][23:16];
            02:ip_data  <=  ip_header[0][15: 8];
            03:ip_data  <=  ip_header[0][ 7: 0];
            //id,rsv df mf offset
            04:ip_data  <=  ip_header[1][31:24];
            05:ip_data  <=  ip_header[1][23:16];
            06:ip_data  <=  ip_header[1][15: 8];
            07:ip_data  <=  ip_header[1][ 7: 0];
            //ttl,protocol udp
            08:ip_data  <=  8'h40;
            //protocol udp
            09:ip_data  <=  8'h11;
            //check sum  
            10:ip_data  <=  check_sum[15: 8];
            11:ip_data  <=  check_sum[ 7: 0];
            //src_ip
            12:ip_data  <=  src_ip[31:24];
            13:ip_data  <=  src_ip[23:16];
            14:ip_data  <=  src_ip[15: 8];
            15:ip_data  <=  src_ip[ 7: 0];
            //dst_ip
            16:ip_data  <=  dst_ip[31:24];
            17:ip_data  <=  dst_ip[23:16];
            18:ip_data  <=  dst_ip[15: 8];
            19:ip_data  <=  dst_ip[ 7: 0];
            default:begin
                ip_data <=  8'h00;
            end
        endcase
    end
    else if(state_c == UDP_HEADER)begin
        case(cnt1)
            //UDP
            //src_port
            00:ip_data[ 7: 0]  <=  src_port[15: 8];
            01:ip_data[ 7: 0]  <=  src_port[ 7: 0];
            //dst_port
            02:ip_data[ 7: 0]  <=  dst_port[15: 8];
            03:ip_data[ 7: 0]  <=  dst_port[ 7: 0];
            //length
            04:ip_data[ 7: 0]  <=  udp_len[15: 8];
            05:ip_data[ 7: 0]  <=  udp_len[ 7: 0];
            //udp check sum
            06:ip_data[ 7: 0]  <=  8'h00;
            07:
            begin 
                ip_data[ 7: 0]  <=  8'h00;
                 wfifo_rd_en <= 1; 
            end 
            default:begin
                ip_data <=  8'h00;
            end
        endcase
        tx_data_counter <=   16'h0;        
        //ram_rd_addr     <=    9'h0;       
        i               <=    0;
    end
    else if(state_c == UDP_DATA)begin
           //udp data
            ip_data     <=  wfifo_rd_data; 
                                                  
            // tx_data_counter<=tx_data_counter+1'b1;           
            // if(i==0) 
            // begin    
            //     ip_data[7:0]<=datain[31:24];
            //     i<=i+1'b1;
            // end
            // else if(i==1) 
            // begin
            //     ip_data[7:0]<=datain[23:16];
            //     i<=i+1'b1;
            //       ram_rd_addr<=ram_rd_addr+1'b1;
            // end
            // else if(i==2) begin
            //    ip_data[7:0]<=datain[15:8];
            //    i<=i+1'b1;
             
            //    if(ram_rd_addr == 4)
            //    begin 
            //         frame      <= datain ;
            //         frame1     <= frame ;
            //         frame_cnt <= frame - frame1;        
            //         if(frame_cnt != 1)frame_cnt_ERR <= 1;
            //         else frame_cnt_ERR <= 0;                            
            //     end    
            // end
            // else if(i==3)
            // begin
            //     ip_data[7:0]<=datain[7:0];
            //     i<=0;                           
            // end
        
    end
    else if(state_c == IDLE)begin 
             wfifo_rd_en <= 0;  
    end 
end

//wfifo_rd_en
//assign  wfifo_rd_en     =   (state_n == UDP_DATA) ? 1'b1 : 1'b0;

endmodule
