/************************************************************************
 * Author        : Wen Chunyang
 * Email         : 1494640955@qq.com
 * Create time   : 2018-05-04 16:49
 * Last modified : 2018-05-04 16:49
 * Filename      : auto_read_write.v
 * Description   : 
 * *********************************************************************/
module  auto_read_write(
        input                   clk                     ,
        input                   rst_n                   ,
        //wfifo
        input                   wfifo_wclk              ,
        input                   wfifo_wr_en             ,
        input         [ 7: 0]   wfifo_wr_data           ,
        input                   wfifo_rclk              ,
        input                   wfifo_rd_en             ,
        output  wire  [ 7: 0]   wfifo_rd_data           ,
        //trig
        output  wire            udp_tx                 
);
//=====================================================================\
// ********** Define Parameter and Internal Signals *************
//=====================================================================/
wire    [10: 0]                 wfifo_rdusedw                   ; 
wire    [10: 0]                 wfifo_wrusedw                   ; 
reg                             tx_en                           ;
reg                             tx_en_r                         ;
//======================================================================
// ***************      Main    Code    ****************
//======================================================================
//tx_en
always  @(posedge clk or negedge rst_n)begin
    if(rst_n==1'b0)begin
        tx_en   <=  1'b0;
    end
    else if(wfifo_wrusedw >= 'd1282)begin
        tx_en   <=  1'b1;
    end
    else begin
        tx_en   <=  1'b0;
    end
end

//tx_en_r
always  @(posedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        tx_en_r <=  1'b0;
    end
    else begin
        tx_en_r <=  tx_en;
    end
end

assign  udp_tx      =   tx_en && (~tx_en_r);

wfifo_8x2k	wfifo_8x2k_inst(
        .data                   (wfifo_wr_data                  ),
        .rdclk                  (wfifo_rclk                     ),
        .rdreq                  (wfifo_rd_en                    ),
        .wrclk                  (wfifo_wclk                     ),
        .wrreq                  (wfifo_wr_en                    ),
        .q                      (wfifo_rd_data                  ),
        .rdusedw                (wfifo_rdusedw                  ),
        .wrusedw                (wfifo_wrusedw                  )
);


endmodule
