/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : eth_arp_send.v
 * Description   : eth_arp_send
 * *********************************************************************/
module  eth_arp_send(
        input                   clk                             ,
        input                   rst_n                           ,
        //Eth
        input                   arp_ack_trig                    ,
        input                   arp_req_trig                    ,
        input         [31: 0]   arp_src_ip                      ,
        input         [31: 0]   arp_dst_ip                      ,
        input         [47: 0]   arp_src_mac                     ,
        input         [47: 0]   arp_dst_mac                     ,
        output  reg   [ 7: 0]   arp_data                        
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
//cnt
reg     [ 5: 0]                 cnt                             ;
wire                            add_cnt                         ;
wire                            end_cnt                         ;

reg     [15: 0]                 opcode                          ;
reg                             flag                            ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/

//opcode
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        opcode  <=  16'd0;
    end
    else if(arp_ack_trig)begin
        opcode  <=  16'd2;
    end
    else if(arp_req_trig)begin
        opcode  <=  16'd1;
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
end

assign  add_cnt     =       flag;       
assign  end_cnt     =       add_cnt && cnt == 46-1;   

//flag
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        flag    <=  1'b0;
    end
    else if(arp_ack_trig || arp_req_trig)begin
        flag    <=  1'b1;
    end
    else if(end_cnt)begin
        flag    <=  1'b0;
    end
end

//arp_data
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        arp_data   <=  8'h00;
    end
    else if(add_cnt)begin
        case(cnt)
            //hdwr_type
            00:arp_data    <=  8'h00;
            01:arp_data    <=  8'h01;
            //protocol_type
            02:arp_data    <=  8'h08;
            03:arp_data    <=  8'h00;
            //hdwr_size
            04:arp_data    <=  8'h06;
            //protocol_size
            05:arp_data    <=  8'h04;
            //opcode
            06:arp_data    <=  opcode[15:8];
            07:arp_data    <=  opcode[ 7:0];
            //sender_mac
            08:arp_data    <=  arp_src_mac[47:40];
            09:arp_data    <=  arp_src_mac[39:32];
            10:arp_data    <=  arp_src_mac[31:24];
            11:arp_data    <=  arp_src_mac[23:16];
            12:arp_data    <=  arp_src_mac[15: 8];
            13:arp_data    <=  arp_src_mac[ 7: 0];
            //sender_ip
            14:arp_data    <=  arp_src_ip[31:24];
            15:arp_data    <=  arp_src_ip[23:16];
            16:arp_data    <=  arp_src_ip[15: 8];
            17:arp_data    <=  arp_src_ip[ 7: 0];
            //target_mac
            18:arp_data    <=  arp_dst_mac[47:40];
            19:arp_data    <=  arp_dst_mac[39:32];
            20:arp_data    <=  arp_dst_mac[31:24];
            21:arp_data    <=  arp_dst_mac[23:16];
            22:arp_data    <=  arp_dst_mac[15: 8];
            23:arp_data    <=  arp_dst_mac[ 7: 0];
            //target_ip
            24:arp_data    <=  arp_dst_ip[31:24];
            25:arp_data    <=  arp_dst_ip[23:16];
            26:arp_data    <=  arp_dst_ip[15: 8];
            27:arp_data    <=  arp_dst_ip[ 7: 0];
            //填充字段
            28:arp_data    <=  8'h00;
            29:arp_data    <=  8'h00;
            30:arp_data    <=  8'hff;
            31:arp_data    <=  8'hff;
            32:arp_data    <=  8'hff;
            33:arp_data    <=  8'hff;
            34:arp_data    <=  8'hff;
            35:arp_data    <=  8'hff;
            36:arp_data    <=  8'h00;
            37:arp_data    <=  8'h23;
            38:arp_data    <=  8'hcd;
            39:arp_data    <=  8'h76;
            40:arp_data    <=  8'h63;
            41:arp_data    <=  8'h1a;
            42:arp_data    <=  8'h08;
            43:arp_data    <=  8'h06;
            44:arp_data    <=  8'h00;
            45:arp_data    <=  8'h01;
            default:begin
                arp_data   <=  8'h00;
            end
        endcase
    end
end

endmodule
