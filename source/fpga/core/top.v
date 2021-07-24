//-------------------------Timescale----------------------------//
`timescale 1 ns / 1 ps 
//-----------------------Module-------------------//
module top(
	input clk_25m,
	
	//arm fsmc
	input wrn,
	input rdn,
	input cs0,
	inout [15:0]db,
	input [23:16]ab,	
	input FSMC_CLK,
	input NADV,


	
	//SDRAM
	inout [15:0] DATA,		//
	output CLK_SDRAM,			//
	output [12:0] ADDR,		//
	output [1 :0] BA,			//
	output CKE,					//
	output CS_N,				//
	output RAS_N,				//
	output CAS_N,				//
	output WE_N,				//
	output [1 :0] DQM	,	   //
	
	//lcd
	output led_red,
	output led_green,
	output led_blue,
	
	//X4
	output x4_mosi ,	      // output  x4_mosi_sig
	output x4_sclk,	      // output  x4_sclk_sig
	output x4_cs ,	         // output  x4_cs_sig
	input  x4_miso,	      // input  x4_miso_sig
	
	output x4_enable,    	//  
	input  x4_isr,	         // 
	
	//X4_2
	output x4_2_mosi ,	  
	output x4_2_sclk,	     
	output x4_2_cs ,	      
	input  x4_2_miso,	     
	
	output x4_2_enable,     	//  
	input  x4_2_isr,	        // 
	
	
	//toMCU
	output fft_isr2mcu,   	//  
	output fir_isr2mcu,	    //  
	output x4data_isr2mcu,	//  	
	
	//antenna receive SWITCH
	output r_switch_A,	        //  	
	output r_switch_B,		    //  
	output r_switch_C,	
	//antenna transfer SWITCH
	output t_switch_A,	        //  	
	output t_switch_B,		    //  
	output t_switch_C
);


/*****************clk rst_n**********************************************/
wire rst_n;

rst_n u1(
	.clk_25m(clk_25m),
	.rst_n(rst_n)
);

pll u2(
	.inclk0(clk_25m),
	.c0(clk_100m),
	.c1(clk_50m),
);
/****************************to mcu**************************************/		
wire [7:0]x4_chanle;
wire write_spi_en;	
wire [7:0]write_spi_addr;	  
wire [7:0]write_spi_data;
wire write_spi_done;
wire read_spi_en;
wire [7:0]read_spi_addr;
wire [7:0]read_spi_data;
wire read_spi_done;

arm_cmd_ctrl u3(
	.clk_100m(clk_100m),
	.clk_25m(clk_25m),
	.rst_n(rst_n),
	.wrn(wrn),
	.rdn(rdn),
	.cs0(cs0),
	.db(db),
	.ab(ab),
   .FSMC_CLK(FSMC_CLK),
	.NADV(NADV),	
	
	//MCU spi CMD
	.x4_chanle(x4_chanle),
	.write_spi_en(write_spi_en), 
	.write_spi_addr(write_spi_addr), 
	.write_spi_data(write_spi_data),
	.write_spi_done(write_spi_done), 
	
	.read_spi_en(read_spi_en),
	.read_spi_addr(read_spi_addr), 
	.read_spi_data(read_spi_data),
	.read_spi_done(read_spi_done), 	
	
	

);

/**********************to x4***********************************/

wire SPI_tx_en;
wire SPI_rx_en;
wire SPI_CS;
wire [7:0]SPI_data_in;
wire [7:0]SPI_data_out;
wire SPI_tx_done;
wire SPI_rx_done;
wire x4_init_done;


wire mcu_tx_en;
wire mcu_rx_en;
wire mcu_CS;
wire [7:0]mcu_data_in;

wire fpga_tx_en;
wire fpga_rx_en;
wire fpga_CS;
wire [7:0]fpga_data_in;
assign x4_isr2mcu = x4_isr;

assign CS          =  0 ? fpga_CS:mcu_CS;
assign SPI_tx_en   =  0 ? fpga_tx_en:mcu_tx_en;
assign SPI_rx_en   =  0 ? fpga_rx_en:mcu_rx_en;
assign SPI_data_in =  0 ? fpga_data_in:mcu_data_in;



x4_ctrl u4(

   .clk   (clk_100m)  , 
   .rst_n (rst_n)     , 	 
	.x4_isr(x4_isr)    ,
	
	//MCU spi CMD
	.write_spi_en(write_spi_en), 
	.write_spi_addr(write_spi_addr), 
	.write_spi_data(write_spi_data),
	.write_spi_done(write_spi_done), 
	
	.read_spi_en(read_spi_en),
	.read_spi_addr(read_spi_addr), 
	.read_spi_data(read_spi_data),
	.read_spi_done(read_spi_done), 
	
	
   .SPI_CS      (fpga_CS),
	.SPI_rx_en   (fpga_rx_en), 
	.SPI_tx_en   (fpga_tx_en), 
	.SPI_data_in (fpga_data_in),  
	
	.SPI_data_out(SPI_data_out), 
	.SPI_tx_done (SPI_tx_done), 
	.SPI_rx_done (SPI_rx_done),
	
   .x4_init_done(x4_init_done),
	
	.r_switch_A(r_switch_A),	        //  	
	.r_switch_B(r_switch_B),		    //  
	.r_switch_C(r_switch_C),	
	//antenna transfer SWITCH
	.t_switch_A(t_switch_A),	        //  	
	.t_switch_B(t_switch_B),		    //  
	.t_switch_C(t_switch_C),
);

spi_module u5(
	 .I_clk(clk_25m)          , 
	 .I_rst_n(rst_n)          , 
	 .I_rx_en(SPI_rx_en)      , 
	 .I_tx_en(SPI_tx_en)      , 
	 .I_data_in(SPI_data_in)  ,  
	 .O_data_out(SPI_data_out), 
	 .O_tx_done(SPI_tx_done)  , 
	 .O_rx_done(SPI_rx_done)  , 
	 .I_cs(CS)                ,
	 
	 .I_spi_miso(x4_miso)     , // SPI串行输入，用来接收从机的数据
	 .O_spi_sck(x4_sclk)      , // SPI时钟
	 .O_spi_cs(x4_cs)         , // SPI片选信
	 .O_spi_mosi(x4_mosi)     ,    // SPI输出，用来给从机发送数
	 
);

spi_module u6(
	 .I_clk(clk_25m)          , 
	 .I_rst_n(rst_n)          , 
	 .I_rx_en(  )      , 
	 .I_tx_en( )      , 
	 .I_data_in( )  ,  
	 .O_data_out( ), 
	 .O_tx_done( )  , 
	 .O_rx_done( )  , 
	 .I_cs( )                ,
	 
	 .I_spi_miso(x4_2_miso )     , // SPI串行输入，用来接收从机的数据
	 .O_spi_sck(x4_2_sclk )      , // SPI时钟
	 .O_spi_cs(x4_2_cs )         , // SPI片选信
	 .O_spi_mosi(x4_2_mosi )     ,    // SPI输出，用来给从机发送数
	 
);
/**************************sdram***********************************************/

wire rd_en;
wire wd_en;
wire rd_avalid;
wire rd_avalid_r;
wire sdram_cs;
wire [25:0]sdram_addr;
wire [15:0]sdram_wdata;
wire [15:0]sdram_rdata;
	
sdram_wr sd0
(
   .clk(clk_100m),
	.rst_n(rst_n),
	.rd_en(rd_en),
	.wd_en(wd_en),
	.rd_avalid(rd_avalid),
	.rd_avalid_r(rd_avalid_r),
	.sdram_cs(sdram_cs),
	.sdram_addr(sdram_addr),
	.sdram_wdata(sdram_wdata),
	.sdram_rdata(sdram_rdata),

);

sdram sd1 (
        .clk_clk                  (clk_100m),                      //        clk.clk
        .reset_reset_n            (rst_n),                         //      reset.reset_n
        .sdram_ctrl_address       (sdram_addr),                    // sdram_ctrl.address
        .sdram_ctrl_byteenable_n  (),                             // byteenable_n
        .sdram_ctrl_chipselect    (sdram_cs),                     // chipselect
        .sdram_ctrl_writedata     (sdram_wdata),                 //writedata
        .sdram_ctrl_read_n        (rd_en),                       // read_n
        .sdram_ctrl_write_n       (wd_en),                       // write_n
        .sdram_ctrl_readdata      (sdram_rdata),                 // readdata
        .sdram_ctrl_readdatavalid (rd_avalid),                     //readdatavalid
        .sdram_ctrl_waitrequest   (),   //           .waitrequest
      
		  .sdram_out_addr           (ADDR),           //  sdram_out.addr
        .sdram_out_ba             (BA),             //           .ba
        .sdram_out_cas_n          (CAS_N),          //           .cas_n
        .sdram_out_cke            (CKE),            //           .cke
        .sdram_out_cs_n           (CS_N),           //           .cs_n
        .sdram_out_dq             (DATA),             //           .dq
        .sdram_out_dqm            (DQM),            //           .dqm
        .sdram_out_ras_n          (RAS_N),          //           .ras_n
        .sdram_out_we_n           (WE_N)            //           .we_n
    );

	 
	 
	 
/**************FFT*****************/
	 
wire        x4data_valid;      
wire [24:0] x4data_data;

wire        fft_sop;
wire        fft_eop;
wire        fft_valid;
wire [24:0] fft_data;

FFT_top FFT_u(
    .clk_100m            (clk_100m),
    .rst_n               (rst_n),
        
    .x4_data_clk         (aud_bclk),
    .x4_imag_data        (),
    .x4_real_data        (x4data_data),
    .x4_data_valid       (x4data_valid),
        
    .data_real          (fft_data),
	 .data_imag           (),
    .data_sop           (fft_sop),
    .data_eop           (fft_eop),
    .data_valid         (fft_valid)
    );
	 
/*****************fir****************/ 
fir f0(
    .clk                (aud_bclk),
    .reset_n            (rst_n),
    
    .ast_sink_data      (x4data_data),
    .ast_sink_valid     (x4data_valid),
    .ast_sink_error     (),
    .ast_sink_ready     (),
    
    .ast_source_ready   (1'b1),
    .ast_source_data    (fir_odata_w),
    .ast_source_valid   (fir_dvalid_w),
    .ast_source_error   (),
);				  


/****************************cnn******************************************/

wire GO;
wire [3:0] RESULT;
wire we_database;
wire  [10:0] dp_database;
wire  [12:0] address_p_database;
wire STOP;
cnn_top
(
  .clk(clk_100m), 
  .GO(), 
  .RESULT(RESULT), 
  .we_database(dp_database),
  .dp_database(address_p_database), 
  .address_p_database(address_p_database),
  .STOP(STOP),
 );
 

endmodule
