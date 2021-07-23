module FFT_top(
    input         clk_100m,
    input         rst_n,
     
    input         x4_data_clk,
    input         x4_data_valid,
    input  [24:0] x4_imag_data,
    input  [24:0] x4_real_data,
	 
    output        data_sop,
    output        data_eop,
    output        data_valid,
	 
    output [24:0] data_real,
	 output [24:0] data_imag
    );

//wire define
wire [24:0] x4_real_data_w;
wire [24:0] x4_imag_data_w;
wire        fifo_rdreq;
wire        fifo_rd_empty;

wire        fft_rst_n;
wire        fft_ready;
wire        fft_sop;
wire        fft_eop;
wire        fft_valid;

wire        source_sop;
wire        source_eop;
wire        source_valid;
wire [24:0] source_real;
wire [24:0] source_imag;

//*****************************************************
//**                   
//***************************************************** 
    
// x4 data  fifo
fft_fifo fifo_inst(
    .aclr           (~rst_n),
    
    .wrclk          (x4_data_clk),
    .wrreq          (x4_data_valid),
    .data           (x4_real_data),
    .wrfull         (),
        
    .rdclk          (clk_100m),
    .rdreq          (fifo_rdreq),
    .q              (x4_real_data_w),
    .rdempty        (fifo_rd_empty)
    );
	 
fft_imag_fifo fifo_inst2(
    .aclr           (~rst_n),
    
    .wrclk          (x4_data_clk),
    .wrreq          (x4_data_valid),
    .data           (x4_imag_data),
    .wrfull         (),
        
    .rdclk          (clk_100m),
    .rdreq          (fifo_rdreq),
    .q              (x4_imag_data_w),
    .rdempty        ()
    );
                
//FFT control 
fft_ctrl u_fft_ctrl(
    .clk_100m        (clk_100m),
    .rst_n           (rst_n),
                 
    .fifo_rd_empty   (fifo_rd_empty),
    .fifo_rdreq      (fifo_rdreq),
    
    .fft_ready       (fft_ready),
    .fft_rst_n       (fft_rst_n),
    .fft_valid       (fft_valid),
    .fft_sop         (fft_sop),
    .fft_eop         (fft_eop) 
    );

//  FFT IP 
fft FFT_u(
    .clk            (clk_100m),
    .reset_n        (fft_rst_n),

    .sink_ready     (fft_ready),    //FFT ready  
    .sink_real      (x4_real_data_w), // 
    .sink_imag      (x4_imag_data_w),        // 
    .sink_sop       (fft_sop),      // 
    .sink_eop       (fft_eop),      // 
    .sink_valid     (fft_valid),    // 
    .inverse        (1'b0),         // 
    .sink_error     (1'b0),         // 
    
    .source_ready   (1'b1),         // 
    .source_real    (data_real),  // 
    .source_imag    (data_imag),  // 
    .source_sop     (data_sop),   // 
    .source_eop     (data_eop),   // 
    .source_valid   (data_valid), // 
    .source_exp     (),             // 
    .source_error   ()              // 
    );                              // 


endmodule 