// sdram_sdram_0.v

// Generated using ACDS version 13.1 162 at 2021.07.21.23:39:31

`timescale 1 ps / 1 ps
module sdram_sdram_0 (
		input  wire        clk_1_clk,        //   clk_1.clk
		input  wire        reset_1_reset_n,  // reset_1.reset_n
		input  wire [24:0] s1_address,       //      s1.address
		input  wire [1:0]  s1_byteenable_n,  //        .byteenable_n
		input  wire        s1_chipselect,    //        .chipselect
		input  wire [15:0] s1_writedata,     //        .writedata
		input  wire        s1_read_n,        //        .read_n
		input  wire        s1_write_n,       //        .write_n
		output wire [15:0] s1_readdata,      //        .readdata
		output wire        s1_readdatavalid, //        .readdatavalid
		output wire        s1_waitrequest,   //        .waitrequest
		output wire [12:0] wire_1_addr,      //  wire_1.addr
		output wire [1:0]  wire_1_ba,        //        .ba
		output wire        wire_1_cas_n,     //        .cas_n
		output wire        wire_1_cke,       //        .cke
		output wire        wire_1_cs_n,      //        .cs_n
		inout  wire [15:0] wire_1_dq,        //        .dq
		output wire [1:0]  wire_1_dqm,       //        .dqm
		output wire        wire_1_ras_n,     //        .ras_n
		output wire        wire_1_we_n       //        .we_n
	);

	sdram_sdram_0_new_sdram_controller_0 new_sdram_controller_0 (
		.clk            (clk_1_clk),        //   clk.clk
		.reset_n        (reset_1_reset_n),  // reset.reset_n
		.az_addr        (s1_address),       //    s1.address
		.az_be_n        (s1_byteenable_n),  //      .byteenable_n
		.az_cs          (s1_chipselect),    //      .chipselect
		.az_data        (s1_writedata),     //      .writedata
		.az_rd_n        (s1_read_n),        //      .read_n
		.az_wr_n        (s1_write_n),       //      .write_n
		.za_data        (s1_readdata),      //      .readdata
		.za_valid       (s1_readdatavalid), //      .readdatavalid
		.za_waitrequest (s1_waitrequest),   //      .waitrequest
		.zs_addr        (wire_1_addr),      //  wire.export
		.zs_ba          (wire_1_ba),        //      .export
		.zs_cas_n       (wire_1_cas_n),     //      .export
		.zs_cke         (wire_1_cke),       //      .export
		.zs_cs_n        (wire_1_cs_n),      //      .export
		.zs_dq          (wire_1_dq),        //      .export
		.zs_dqm         (wire_1_dqm),       //      .export
		.zs_ras_n       (wire_1_ras_n),     //      .export
		.zs_we_n        (wire_1_we_n)       //      .export
	);

endmodule