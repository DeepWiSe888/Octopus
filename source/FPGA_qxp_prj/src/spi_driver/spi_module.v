/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-6
 * Last modified : 2022-3-6
 * Filename      : spi_module.v
 * Description   : spi driver
 * *********************************************************************/
module spi_module
(
input                       I_clk                                       , // 
input                       I_rst_n                                     , //  
input                       I_rx_en                                     , // 
input                       I_tx_en                                     , //  
input        [7:0]          I_data_in                                   , // 
input                       I_cs,
output  reg  [7:0]          O_data_out                                  , // 
output  reg                 O_tx_done                                   , //  
output  reg                 O_rx_done                                   , //  


input                       I_spi_miso                                  , //  
output  reg                 O_spi_sck                                   , // 
output  reg                 O_spi_cs                                    , //  
output  reg                 O_spi_mosi                                   
);

reg     [4:0]               R_tx_state                                  ; 
reg     [4:0]               R_rx_state                                  ;
reg     [7:0]               wdata                                       ;     


/************************************************************************/
//                           get the tx rising edge
/***********************************************************************/
reg     tx_en1, tx_en2;
wire    tx_en;
always @(posedge I_clk or negedge I_rst_n)
if(!I_rst_n)
begin
    tx_en1  <= 0;
    tx_en2  <= 0;
end
else
begin
    tx_en1  <= I_tx_en;
    tx_en2  <= tx_en1;
end
assign tx_en = (!tx_en2) & tx_en1;

/************************************************************************/
//                           get the rt rising edge
/***********************************************************************/
reg     rx_en1, rx_en2;
wire    rx_en;
always @(posedge I_clk or negedge I_rst_n)
if(!I_rst_n)
begin
    rx_en1  <= 0;
    rx_en2  <= 0;
end
else
begin
    rx_en1 <= I_rx_en;
    rx_en2 <= rx_en1;
end
assign rx_en = (!rx_en2) & rx_en1;

/************************************************************************/
//                           tx && rx loop
/***********************************************************************/
always @(posedge I_clk or negedge I_rst_n)
begin
    if(!I_rst_n)
    begin
        R_tx_state  <=  5'd17   ;
        R_rx_state  <=  5'd17   ;
        O_spi_cs    <=  1'b1    ;
        O_spi_sck   <=  1'b0    ;
        O_spi_mosi  <=  1'b0    ;
        O_tx_done   <=  1'b0    ;
        O_rx_done   <=  1'b0    ;
        O_data_out  <=  8'd0    ;
        wdata       <=  8'd0    ;
    end 
    else if( I_cs == 0)             //  
    begin           
        O_spi_cs       <=  1'b0    ; //               
        if(tx_en) R_tx_state <= 0;
        if(rx_en) R_rx_state <= 0;
        case(R_tx_state)
            5'd1, 5'd3 , 5'd5 , 5'd7  , 
            5'd9, 5'd11, 5'd13 ,5'd15: 
                begin                            
                    O_spi_sck   <=  1'b1                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end
            5'd0:    //  
                begin
                         
                    O_spi_mosi  <=  I_data_in[7]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end
            5'd2:    // 
                begin
                    O_spi_mosi  <=  I_data_in[6]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end
            5'd4:    // 
                begin
                    O_spi_mosi  <=  I_data_in[5]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end 
            5'd6:    //  
                begin
                    O_spi_mosi  <=  I_data_in[4]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end 
            5'd8:    // 
                begin
                    O_spi_mosi  <=  I_data_in[3]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end                            
            5'd10:    //  
                begin
                    O_spi_mosi  <=  I_data_in[2]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end 
            5'd12:    // 
                begin
                    O_spi_mosi  <=  I_data_in[1]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end 
            5'd14:    //  
                begin
                    O_spi_mosi  <=  I_data_in[0]        ;
                    O_spi_sck   <=  1'b0                ;
                    R_tx_state  <=  R_tx_state + 1'b1   ;
                    O_tx_done   <=  1'b0                ;
                end
    
            5'd16:
                begin
                    O_spi_sck   <=  1'b0                ;                         
                    O_tx_done   <=  1'b1                ;
                end
            5'd17:
            begin

            end
            default:R_tx_state  <=  5'd17               ;   
        endcase

        case(R_rx_state)
            5'd0, 5'd2 , 5'd4 , 5'd6  , 
            5'd8, 5'd10, 5'd12, 5'd14 : // 
                begin 
                    O_spi_sck       <=  1'b1                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                end
            5'd1:     
                begin                       
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[7]   <=  I_spi_miso          ;   
                end
            5'd3:     
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[6]   <=  I_spi_miso          ; 
                end
            5'd5:     
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[5]   <=  I_spi_miso          ; 
                end 
            5'd7:     
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[4]   <=  I_spi_miso          ; 
                end 
            5'd9:      
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[3]   <=  I_spi_miso          ; 
                end                            
            5'd11:      
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[2]   <=  I_spi_miso          ; 
                end 
            5'd13:      
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    O_rx_done       <=  1'b0                ;
                    O_data_out[1]   <=  I_spi_miso          ; 
                end 
            5'd15:      
                begin
                    O_spi_sck       <=  1'b0                ;
                    R_rx_state      <=  R_rx_state + 1'b1   ;
                    
                    O_data_out[0]   <=  I_spi_miso          ; 
                end
            5'd16:
                begin
                    O_spi_sck       <=  1'b0                ;
                    O_rx_done       <=  1'b1                ;
                      
                end  
            5'd17:
                begin
                
                end
        
                     
            default:R_rx_state  <=  5'd17                   ;   
        endcase 
    end    
    else
        begin
          
            O_spi_cs    <=  1'b1                            ; 
            R_rx_state  <=  5'd17                           ;
            R_tx_state  <=  5'd17                           ;
            O_tx_done   <=  1'b0                            ;
            O_rx_done   <=  1'b0                            ;
          
            O_spi_sck   <=  1'b0                            ;
            O_spi_mosi  <=  1'b0                            ;

        end      
end

endmodule         


