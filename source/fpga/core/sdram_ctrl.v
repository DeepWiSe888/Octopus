module sdram_wr
(
   input    clk,
	input    rst_n,
	output	reg rd_en,
	output	reg wd_en,
	output	wire rd_avalid,
	output	reg  rd_avalid_r,
	output	reg  sdram_cs,
	output	reg  [25:0]sdram_addr,
	output	reg  [15:0]sdram_wdata,
	input	   [15:0]sdram_rdata

		

);



reg  [7:0]sdram_state;

always@(posedge clk or negedge rst_n)
if(!rst_n)
begin
  rd_en <= 0;
  wd_en <= 0;
  rd_avalid_r <= 0;
  sdram_cs    <= 0;
  sdram_addr  <= 0;
  sdram_wdata <= 0;
  sdram_state <= 0;
end
else
begin
 
   case(sdram_state)
	0, 1, 2:
	begin
	 sdram_cs  <= 1;
	 sdram_addr <= 10;
	// sdram_wdata <= 77;
	  rd_en <= 0;
	  wd_en <= 1;
	  sdram_state <= sdram_state + 1;
	end
	
	3, 4, 5:
	begin
	 	
	  rd_avalid_r <= rd_avalid;
	  if(rd_avalid_r)
	  begin   
	    rd_en <= 1;
	    wd_en <= 0;
		  if(sdram_rdata == sdram_wdata)
	         sdram_wdata <= sdram_wdata + 1;
	  end
	  sdram_state <= sdram_state + 1;
	end
	6:
	begin
		  sdram_state <= 0;
	end
	endcase
end


endmodule
