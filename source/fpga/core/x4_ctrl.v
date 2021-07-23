module x4_ctrl
(
   input   clk        , 
   input   rst_n      , 


  	input write_spi_en, 
	input [7:0]write_spi_addr, 
	input [7:0]write_spi_data,
	output write_spi_done, 
	
	input read_spi_en,
	input  [7:0]read_spi_addr, 
	output reg[7:0]read_spi_data,
	output read_spi_done, 
	
	input   x4_isr     ,
	
   output  reg SPI_CS      ,
	output  reg SPI_rx_en   , 
	output  reg SPI_tx_en   , 
	output  reg [7:0]SPI_data_in ,
	
	input   [7:0]SPI_data_out, 
	input   SPI_tx_done , 
	input   SPI_rx_done,
	output  reg x4_init_done
);
 
parameter   ADDR_SPI_FORCE_ZERO_R                      = 0;
parameter   ADDR_SPI_FORCE_ONE_R                       = 1;
parameter   ADDR_SPI_CHIP_ID_DIG_R                     = 2;
parameter   ADDR_SPI_CHIP_ID_SYS_R                     = 3;
parameter   ADDR_SPI_DEBUG_RW                          = 4;
parameter   ADDR_SPI_RADAR_DATA_SPI_RE                 = 5;
parameter   ADDR_SPI_RADAR_DATA_SPI_STATUS_R           = 6;
parameter   ADDR_SPI_SPI_RADAR_DATA_CLEAR_STATUS_WE    = 7;
parameter   ADDR_SPI_SPI_RADAR_DATA0_FIFO_STATUS_R     = 8;
parameter   ADDR_SPI_SPI_RADAR_DATA0_CLEAR_STATUS_WE   = 9;
parameter   ADDR_SPI_SPI_RADAR_DATA1_FIFO_STATUS_R     = 10;
parameter   ADDR_SPI_SPI_RADAR_DATA1_CLEAR_STATUS_WE   = 11;
parameter   ADDR_SPI_RADAR_BIST_CTRL_RW                = 12; 
parameter   ADDR_SPI_RADAR_BIST_STATUS_R               = 13;
parameter   ADDR_SPI_FIRMWARE_VERSION_SPI_R            = 14;
parameter   ADDR_SPI_TO_CPU_WRITE_DATA_WE              = 15;
parameter   ADDR_SPI_SPI_MB_FIFO_STATUS_R              = 16;
parameter   ADDR_SPI_FROM_CPU_READ_DATA_RE             = 17;
parameter   ADDR_SPI_SPI_MB_CLEAR_STATUS_WE            = 18;
parameter   ADDR_SPI_TO_MEM_WRITE_DATA_WE              = 19;
parameter   ADDR_SPI_SPI_MEM_FIFO_STATUS_R             = 20;
parameter   ADDR_SPI_FROM_MEM_READ_DATA_RE             = 21;
parameter   ADDR_SPI_SPI_MEM_CLEAR_STATUS_WE           = 22;
parameter   ADDR_SPI_MEM_MODE_RW                       = 23;
parameter   ADDR_SPI_MEM_FIRST_ADDR_MSB_RW             = 24;
parameter   ADDR_SPI_MEM_FIRST_ADDR_LSB_RW             = 25;
parameter   ADDR_SPI_BOOT_FROM_OTP_SPI_RWE             = 26;
parameter   ADDR_SPI_MCU_BIST_CTRL_RW                  = 27;
parameter   ADDR_SPI_MCU_BIST_STATUS_R                 = 28; 
parameter   ADDR_SPI_SPI_CONFIG_WE                     = 29; 
parameter   ADDR_SPI_CPU_RESET_RW                      = 127;

/*************read data from ram port a*******************/

reg SPI_tx_done1;
reg SPI_tx_done2;	
wire tx_done;
always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
	SPI_tx_done1  <= 1'b0;
	SPI_tx_done2  <= 1'b0;
	end
	else
	begin
		SPI_tx_done1 <= SPI_tx_done;
		SPI_tx_done2 <= SPI_tx_done1;
	end

assign tx_done = SPI_tx_done1 & (!SPI_tx_done2);

reg SPI_rx_done1;
reg SPI_rx_done2;	
wire rx_done;

always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
	SPI_rx_done1  <= 1'b0;
	SPI_rx_done2  <= 1'b0;
	end
	else
	begin
		SPI_rx_done1 <= SPI_rx_done;
		SPI_rx_done2 <= SPI_rx_done1;
	end

assign rx_done = SPI_rx_done1 & (!SPI_rx_done2);


always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
		x4_init_done <= 0;
	end
   else
	begin
		x4_init_done <= 1;
	end
	

reg [7:0]read_state;
reg [7:0]fifo_status;
reg wr_slt;
reg spi_register_en;
always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
		read_state <= 0 ;
		spi_register_en <= 0;
		wr_slt <= 0;
	end
   else
	begin
		if(1)    // 
		begin
		  //read_raw_data
		   //get_framecounter
		  case(read_state)  
				0:              // 
				begin
				   get_spi_addr <= 8'd0;
				   spi_register_en <= 1;
					wr_slt <= 1;
				   read_state <= 1;
				end
				1:
				begin
				 if(spi_done)
				 begin
						fifo_status <= get_spi_data;
						read_state <= 2;
				 end
				end
				2:
				begin
				   get_spi_addr <= 8'd1;
				   spi_register_en <= 1;
					wr_slt <= 0;
				   read_state <= 3;
				end
				3:
				begin
				 if(spi_done)
				 begin
						fifo_status <= get_spi_data;
						read_state <= 0;
				 end
				end
		  endcase
		
		end
	end
	
	

	
	
//*******************spi_write_read***********************/
reg wr_en1;
reg wr_en2;
wire wr_en;
always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin		
		wr_en1   <= 0;
		wr_en2   <= 0;
	end
	else
	begin
		wr_en1 <= spi_register_en;
		wr_en2 <= wr_en1;
   end	
	
assign 	wr_en  = wr_en1 & (!wr_en2);



reg [7:0]set_spi_addr;
reg [7:0]set_spi_data;



reg [7:0]get_spi_addr;
reg [7:0]get_spi_data;
reg [7:0]wr_state;
reg spi_done;

always@(posedge clk or negedge rst_n)
	if(!rst_n)
	begin
		wr_state <= 8'h0;
		spi_done <= 0;
	end
	else
	begin
		case (wr_state)
			0:                           
			begin
			
			if(wr_en)    // 
				begin
				   spi_done    <= 0;
					SPI_data_in <= set_spi_addr;  // 
					SPI_CS      <= 0;
					SPI_tx_en   <= 1;
					SPI_rx_en   <= 0;
					wr_state    <= 1;
				end
			end
			1:
			begin
				if(SPI_tx_done)
				begin
					SPI_data_in  <= set_spi_data;    // 
					SPI_CS       <= 0;
					SPI_tx_en    <= 0;
					SPI_rx_en    <= 0;	
	            if(wr_slt == 1)			 // 
					   wr_state    <= 2;
					else if(wr_slt == 0)		 // 
					   wr_state <= 4;
					else wr_state <= 0;
				end
			end
			2:
			begin
			      SPI_data_in <= set_spi_data; 
					SPI_CS    <= 0;                  // 
					SPI_tx_en <= 1;
					SPI_rx_en <= 0;
					wr_state  <= 3;
					
			end
			3:
			begin
				if(SPI_tx_done)
				begin
					SPI_data_in <= 0;                   // 
			      SPI_CS      <= 1;                  // 
					SPI_tx_en   <= 0;
					SPI_rx_en   <= 0;
					wr_state    <= 0;
					spi_done    <= 1;					
				end
			end
			4:
			begin
			
				 
					SPI_CS    <= 0;                  // 
					SPI_tx_en <= 0;
					SPI_rx_en <= 1;
					wr_state  <= 5;
			end
			
			5:
			begin
				if(SPI_rx_done)
				begin
					get_spi_data <= SPI_data_out;          // 
			      SPI_CS    <= 1;                       // 
					SPI_tx_en <= 0;
					SPI_rx_en <= 0;
					wr_state  <= 0;
					spi_done  <= 1;					
				end
			end
			
		default : wr_state <= 0;
	   endcase	
		
	end
	
	

	
endmodule	
