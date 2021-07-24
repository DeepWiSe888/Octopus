//-------------------------Timescale----------------------------//
`timescale 1 ns / 1 ps 
//-----------------------Module dual_port_ram  -------------------//
module arm_cmd_ctrl(
	input clk_25m,
	input clk_100m,
	input rst_n,
	//FMC -port 2 ARM
   input FSMC_CLK,
	input NADV,
	input wrn,
	input rdn,
	input cs0,
	inout [15:0]db,
	input [23:16]ab,
	
	//MCU spi CMD
	
	output reg [7:0]x4_chanle,
	output reg      write_spi_en,
	output reg [7:0]write_spi_addr, 
	output reg [7:0]write_spi_data,
	input       write_spi_done, 
	
	output reg       read_spi_en,
	output reg [7:0]read_spi_addr, 
	input      [7:0]read_spi_data,
	input       read_spi_done, 	
	
	
	output fft_isr2mcu,
	output fir_isr2mcu,
	output x4data_isr2mcu,
	
	 
	input [23:0]fir_real_data,
	input [23:0]fir_imag_data,
	
   input [23:0]fft_real_data,
	input [23:0]fft_imag_data,
	
	input [23:0]x4_real_data,
	input [23:0]x4_imag_data	

);

//--------------------------ram_b_wrd----------------------------//
reg [23:0]address;

always @ (posedge NADV or negedge rst_n)
	begin
		if(!rst_n)
			begin
				address <= 24'd0;
			end
		else 
			begin
				address <= {ab,db};
			end
		
	end

wire rd = (cs0 | rdn);
wire wr = (cs0 | wrn);

reg wr_clk1,wr_clk2;
always@(posedge clk_100m or negedge rst_n)
	if(!rst_n)
		begin
			wr_clk1 <= 1'd1;
			wr_clk2 <= 1'd1;
		end
	else 
		{wr_clk2,wr_clk1} <= {wr_clk1,wr};
		
wire clk_b = (!wr_clk2 | !rd);
wire [15:0]dataout_b;	
assign db = !rd ? dataout_b : 16'hzzzz;
		
//-----------------------------ram-----------------------------//

ram u1(

   .data_b(db),
	.address_b(address),
	.wren_b(!wr),
	.rden_b(!rd),
	.clock_b(clk_b),
	.q_b(dataout_b),
	
	.data_a(data_a),
	.address_a(address_a),
	.wren_a(wren_a),
	.rden_a(rden_a),
	.clock_a(!clk_a),
	.q_a(dataout_a)
	
);
//-------------------------write & read port a-------------------//
reg wren_a;
reg rden_a;
reg [15:0]data_a;           //port a    
reg [15:0]address_a;        //port a  

wire [15:0]dataout_a;
//-------------------------------clk_a-----------------------------//
reg clk1,clk2;
always@(posedge clk_100m or negedge rst_n)
	if(!rst_n)
		begin
			clk1 <= 1'd0;
			clk2 <= 1'd0;
		end
	else 
		{clk2,clk1} <= {clk1,clk_25m};
		
wire clk_a = (clk_25m & clk1);


		

reg [15:0]cmd_data;
reg [15:0]cmd_state;
reg [15:0]cmd_wd_len;
reg [15:0]cmd_rd_len;

reg cmd_w_spi;	
reg cmd_r_spi;	
reg cmd_wr_spi;	


reg [15:0]read_addr;
reg [15:0]write_addr;
reg  buffer_A_usedw;
reg  buffer_B_usedw;
reg  [15:0]wr_cnt;
reg wait_mcu;

always@(posedge clk_a or negedge rst_n)
  if(!rst_n)
  begin
		cmd_state     <= 16'h0;
		wren_a        <= 1'd0;
		rden_a        <= 1'd0;
		data_a        <= 16'd0;
		address_a     <= 16'd0;	

		cmd_wd_len    <= 16'd0;
		cmd_rd_len    <= 16'd0;
		write_addr    <= 16'd50;
		read_addr     <= 16'd50;
		wr_cnt        <= 16'd0;
		buffer_A_usedw<= 0;
		buffer_B_usedw<= 0;
		wait_mcu      <= 0;
  end
  else
   begin	
       case(cmd_state)  
		 16'd0:                               //read arm cmd from ram port b address 1
		 begin
		      wren_a      <= 1'd0;
				rden_a      <= 1'd1;			
				address_a   <= 16'd2;				          
				cmd_state   <= cmd_state + 16'd1;
		 end
		 16'd1:                               //
		 begin		              
				cmd_wd_len  <= dataout_a;             
				cmd_state   <= cmd_state + 16'd1;
		 end		 
       16'd2:                               //
		 begin		     
				address_a   <= 16'd3;			            
				cmd_state   <= cmd_state + 16'd1;
		 end
		 16'd3:                               //
		 begin			            
				cmd_rd_len  <= dataout_a;              
				cmd_state   <= cmd_state + 16'd1;
		 end
		 16'd4:                               //
		 begin			            
				address_a   <= 16'd1;	             
				cmd_state   <= cmd_state + 16'd1;
		 end
		 16'd5:                               //
		 begin			            
				cmd_data    <= dataout_a;         	             
				cmd_state   <= cmd_state + 16'd1;
		 end
		 16'd6:                               //
		 begin					     
				cmd_data    <= dataout_a;       
				if((cmd_data == 16'h11) || (cmd_data == 16'h33) || (cmd_data == 16'h22))
				begin
				    cmd_state   <= 8'd9; //
					 read_addr	 <= 16'd50; 
					 cmd_data    <= 0;
				end	
				else
			   begin
					cmd_state  <= 16'd0;
				end
		 end
		 16'd7:     //clear cmd                           
		 begin
		      wren_a      <= 1'd1;         
				rden_a      <= 1'd0;			
				address_a   <= 16'd1;		     
				data_a      <= 16'd0;   
            cmd_state   <= cmd_state + 1;;
       end	
		 16'd8:   //switch to read cmd
		 begin
		      wren_a      <= 1'd1;          
				rden_a      <= 1'd0;			
				address_a   <= 16'd1;		     
				data_a      <= 16'd0;   
            cmd_state   <= 16'd0;
       end			 
       16'd9:    //read spi data from ram write to x4
		 begin
				  if(read_addr == 16'd50 + cmd_wd_len) //
				  begin					 
						  
						if(cmd_rd_len == 0)           //only write 
						begin
								cmd_state   <= 16'd7;  //
								
						end						
						else 
						  begin	
							  cmd_state    <= 16'd10; //
							  write_addr	<= 16'd50; 
							 
						  end
						
				  end
				  else if(write_spi_done)
				  begin
					  read_addr   <= read_addr + 1'b1;    //				 
				  end						 
				  else
				  begin							 
						wren_a <= 1'b0;
						rden_a <= 1'b1;
						address_a   <= read_addr;	
						write_spi_data <= dataout_a;              //	 							    
				  end
				
				
       end	
		 
		 16'd10:     // read data from x4 spi and write to ram
		 begin		 
					 if(write_addr == 16'd50 + cmd_rd_len) 
					  begin					    
												
						  cmd_state   <= 16'd7; //
							
					  end
					  else if(read_spi_done)
					  begin				  
					  
						  write_addr   <= write_addr + 1'b1;    //
						  
	
					  end						 
					  else
					  begin							 
							wren_a      <= 1'b1;
							rden_a      <= 1'b0;
							address_a   <= write_addr;	
							data_a      <= read_spi_data;                //	 
                     cmd_state   <= 16'd11; //							
					  end
			
		      
		 
		 end 		
		 16'd11:                                            //
		 begin
		 
		      wren_a      <= 1'd1;
				rden_a      <= 1'd0;			
				address_a   <= 16'd49;		           
				data_a      <= 1;    
            cmd_state   <= 10;
		 end

		 
		 
		 default : cmd_state   <= 16'd0;
		 endcase

	end
	



endmodule

