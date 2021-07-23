module x4_spi_register
(
   input  clk,
	input  rst_n,
	input [7:0]spi_addr,
	input [7:0]pif_addr,
	input [7:0]xif_addr,
	input set_spi_register,
   input get_spi_register,	
	input set_pif_register,
   input get_pif_register,	
	input set_xif_register,
   input get_xif_register,	
	input [7:0]i_data,
	output reg [7:0]o_data,
	output reg spi_cs,
	output reg [7:0]spi_data_in,
	input  spi_data_out,
	output reg tx_en,
	output reg rx_en,
	input  tx_done,
	input  rx_done,
   output  reg  spi_done
	
);

//set_spi_register
reg [7:0]set_state;
always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
		set_state <= 8'h0;
		spi_done <= 0;
	end
	else
	begin
		case (set_state)
			0:
			begin
				if(set_spi_register)   //
				begin
				   spi_done <= 0;
					spi_data_in <= spi_addr;  //写地址
					spi_cs <= 0;
					tx_en <= 1;
					rx_en <= 0;
					set_state <= 1;
				end
			end
			1:
			begin
				if(tx_done)
				begin
					spi_data_in <= i_data;    //写地址完成
					spi_cs <= 0;
					tx_en <= 0;
					rx_en <= 0;
					set_state <= 2;
				end
			end
			2:
			begin
			      spi_data_in <= i_data; 
					spi_cs <= 0;                  //写数据
					tx_en <= 1;
					rx_en <= 0;
					set_state <= 3;
			end
			3:
			begin
				if(tx_done)
				begin
					spi_data_in <= 0;          //写数据完成
					spi_cs <= 1;
					tx_en <= 0;
					rx_en <= 0;
					set_state <= 0;
					spi_done <= 1;
				end
			end
		default : set_state <= 0;
	   endcase	
		
	end
//set_spi_register
reg [7:0]get_state;
always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
		get_state <= 8'h0;
		spi_done <= 0;
	end
	else
	begin
		case (get_state)
			0:
			begin
				if(get_spi_register)   //
				begin
				   spi_done <= 0;
					spi_data_in <= spi_addr;  //写地址
					spi_cs <= 0;
					tx_en <= 1;
					rx_en <= 0;
					get_state <= 1;
				end
			end
			1:
			begin
				if(tx_done)
				begin
					spi_data_in <= i_data;    //写地址完成
					spi_cs <= 0;
					tx_en <= 0;
					rx_en <= 0;
					get_state <= 2;
				end
			end
			2:
			begin			    
					spi_cs <= 0;                  //读数据
					tx_en <= 0;
					rx_en <= 1;
					get_state <= 3;
			end
			3:
			begin
				if(rx_done)
				begin
					o_data  <= spi_data_out;        //写数据完成
					spi_cs <= 1;
					tx_en <= 0;
					rx_en <= 0;
					get_state <= 0;
					spi_done <= 1;
				end
			end
		default : get_state <= 0;
	   endcase	
		
	end
	
endmodule
