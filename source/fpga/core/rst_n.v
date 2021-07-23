/* 

 * 
 * --------------------
 */
//-------------------------Timescale----------------------------//
`timescale 1 ns / 1 ps 
//-----------------------Module rst_n---------------------------//
module rst_n(
	input clk_25m,
	output rst_n
);

reg [5:0]rst_cnt;
always@(posedge clk_25m)
	if(rst_cnt == 4'd50)
		begin 
			rst_cnt <= 4'd50;
		end 
	else 
		rst_cnt <= rst_cnt + 1'd1;
		
assign rst_n = (rst_cnt == 4'd50) ? 1'd1 : 1'd0;

endmodule 
		