/************************************************************************
 * Author        : Wen Chunyang
 * Email         : 1494640955@qq.com
 * Create time   : 2018-11-22 12:55
 * Last modified : 2018-11-22 12:55
 * Filename      : generate_data.v
 * Description   : 
 * *********************************************************************/
module  generate_data(
        input                   clk                             ,
        input                   rst_n                           ,
        input                   udp_wr                          ,
        //wfifo
        output  reg             wfifo_wclk                      ,
        output  reg             wfifo_wr_en                     ,
        output  reg   [ 7: 0]   wfifo_wr_data 
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
//cnt0
reg     [15: 0]                 cnt0                            ;
wire                            add_cnt0                        ;
wire                            end_cnt0                        ;
//cnt1
reg     [15: 0]                 cnt1                            ;
wire                            add_cnt1                        ;
wire                            end_cnt1                        ;
//cnt2
reg     [15: 0]                 cnt2                            ;
wire                            add_cnt2                        ;
wire                            end_cnt2                        ;
reg                             udp_wr_r                        ;
reg                             flag                            ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/
//wfifo_wclk
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        wfifo_wclk  <=  1'b0;
    end
    else begin
        wfifo_wclk  <=  ~wfifo_wclk;
    end
end

//udp_wr_r
always  @(posedge wfifo_wclk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        udp_wr_r    <=  1'b0;
    end
    else begin
        udp_wr_r    <=  udp_wr;
    end
end

assign  udp_wr_pos  =   udp_wr && (~udp_wr_r);

//cnt0
always @(posedge wfifo_wclk or negedge rst_n)begin
    if(!rst_n)begin
        cnt0 <= 0;
    end
    else if(add_cnt0)begin
        if(end_cnt0)
            cnt0 <= 0;
        else
            cnt0 <= cnt0 + 1;
    end
end

assign  add_cnt0        =       flag;
assign  end_cnt0        =       add_cnt0 && cnt0 == 8680-1;

//cnt1
always @(posedge wfifo_wclk or negedge rst_n)begin 
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

assign  add_cnt1        =       end_cnt0;
assign  end_cnt1        =       add_cnt1 && cnt1 == 480-1;

//cnt2
always @(posedge wfifo_wclk or negedge rst_n)begin
    if(!rst_n)begin
        cnt2 <= 0;
    end
    else if(add_cnt2)begin
        if(end_cnt2)
            cnt2 <= 0;
        else
            cnt2 <= cnt2 + 1;
    end
end

assign  add_cnt2        =       end_cnt1;       
assign  end_cnt2        =       add_cnt2 && cnt2 == 1-1;

//flag
always  @(posedge wfifo_wclk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        flag    <=  1'b0;
    end
    else if(udp_wr_pos)begin
        flag    <=  1'b1;
    end
    else if(end_cnt2)begin
        flag    <=  1'b0;
    end
end

//wfifo_wr_en
always  @(posedge wfifo_wclk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        wfifo_wr_en <=  1'b0;
    end
    else if(add_cnt0 && (cnt0 == 1-1))begin
        wfifo_wr_en <=  1'b1;
    end
    else if(add_cnt0 && (cnt0 == 1283-1))begin
        wfifo_wr_en <=  1'b0;
    end
end

//wfifo_wr_data
always  @(posedge wfifo_wclk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        wfifo_wr_data   <=  8'd0;
    end
    else if(add_cnt0 && cnt0 == 1-1)begin
        wfifo_wr_data   <=  cnt1[7:0];
    end
    else if(add_cnt0 && cnt0 == 2-1)begin
        wfifo_wr_data   <=  cnt1[15:8];
    end
    else if(add_cnt0 && cnt0[0])begin
        wfifo_wr_data   <=  8'h00;
    end
    else if(add_cnt0 && cnt0[0] == 1'b0)begin
        wfifo_wr_data   <=  8'h1f;
    end
end





endmodule
