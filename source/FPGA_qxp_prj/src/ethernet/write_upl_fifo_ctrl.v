/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-11
 * Last modified : 2022-3-11
 * Filename      : write_upl_fifo_ctrl.v    
 * Description   : write data to upload fifo controller
 * *********************************************************************/
 module write_upl_fifo_ctrl (
input                               clk                                 ,    // Clock
input                               rst_n                               ,    // Asynchronous reset active low
//udp ctrl 
output  reg     [15: 0]             tx_data_len                         ,
output  reg                         tx_data_en                          ,  
//upload fifo       
output  reg                         upl_fifo_wrreq                      ,
output  reg     [31: 0]             upl_fifo_wrdata                     ,
input                               upl_fifo_wrfull                     ,
input                               send_finish                         ,
//data frome cmd ctrl
input           [15: 0]             cmd_tx_data_len                     ,
input                               cmd_tx_data_done                    ,        
output  reg                         cmd_fifo_rdreq                      ,
input           [31: 0]             cmd_fifo_rddata                     ,
input           [ 7: 0]             cmd_fifo_rdusedw                    ,
input                               cmd_fifo_rdempty                    ,

//data from x4
input           [15: 0]             x4_tx_data_len                     ,
input                               x4_tx_data_done                    ,        
output  reg                         x4_fifo_rdreq                      ,
input           [31: 0]             x4_fifo_rddata                     ,
input           [ 7: 0]             x4_fifo_rdusedw                    ,
input                               x4_fifo_rdempty                    , 


//data from gf
input           [15: 0]             gf_tx_data_len                     ,
input                               gf_tx_data_done                    ,        
output  reg                         gf_fifo_rdreq                      ,
input           [31: 0]             gf_fifo_rddata                     ,
input           [ 7: 0]             gf_fifo_rdusedw                    ,
input                               gf_fifo_rdempty                     

    
 );
 
//cat cmd tx en rising
reg                                 cmd_data_en1                        ;
reg                                 cmd_data_en2                        ;
wire                                cmd_data_en                         ;
reg                                 read_cmd_fifo_en                    ;
reg             [15 :0]             sent_data_len                       ;

always @(posedge clk or negedge rst_n)
begin 
    if(~rst_n) begin

    end else begin
        cmd_data_en1     <= cmd_tx_data_done                            ;
        cmd_data_en2     <= cmd_data_en1                                ;
    end
end
assign  cmd_data_en     =  ( !cmd_data_en2 ) & cmd_data_en1             ;

reg                                 x4_data_en1                        ;
reg                                 x4_data_en2                        ;
wire                                x4_data_en                         ;
reg                                 read_x4_fifo_en                    ;


always @(posedge clk or negedge rst_n)
begin 
    if(~rst_n) begin

    end else begin
       x4_data_en1     <= x4_tx_data_done                            ;
       x4_data_en2     <= x4_data_en1                                ;
    end
end
assign  x4_data_en     =  ( !x4_data_en2 ) & x4_data_en1             ;

reg                                 gf_data_en1                        ;
reg                                 gf_data_en2                        ;
wire                                gf_data_en                         ;
reg                                 read_gf_fifo_en                    ;


always @(posedge clk or negedge rst_n)
begin 
    if(~rst_n) begin

    end else begin
       gf_data_en1     <= gf_tx_data_done                            ;
       gf_data_en2     <= gf_data_en1                                ;
    end
end
assign  gf_data_en     =  ( !gf_data_en2 ) & gf_data_en1             ;


reg [15:0]cmd_data_len;
reg [15:0]x4_data_len;
reg [15:0]gf_data_len;
always @(posedge clk or negedge rst_n)
begin 
    if(~rst_n) begin

    end else begin
      if(cmd_tx_data_done)
            cmd_data_len    <=   cmd_tx_data_len  ;
       else if (x4_tx_data_done) begin
            x4_data_len    <=   x4_tx_data_len    ;
       end
        else if (gf_tx_data_done) begin
            gf_data_len    <=   gf_tx_data_len    ;
       end
    end
end


//

always @(posedge clk or negedge rst_n) 
begin 
    if(~rst_n) begin
        read_cmd_fifo_en <= 1'b0;
    end else begin
        
        if(cmd_data_en)
            begin
                read_cmd_fifo_en <= 1'b1;
                
            end 
        else if(upl_fifo_data_state == 1)read_cmd_fifo_en <= 1'b0;
    end
end

//
always @(posedge clk or negedge rst_n) 
begin 
    if(~rst_n) begin
        read_x4_fifo_en <= 1'b0;
    end else begin
        
        if(x4_data_en)
            begin
                read_x4_fifo_en <= 1'b1;
              
            end 
        else if(upl_fifo_data_state == 2)read_x4_fifo_en <= 1'b0;
    end
end

//
always @(posedge clk or negedge rst_n) 
begin 
    if(~rst_n) begin
        read_gf_fifo_en <= 1'b0;
    end else begin
        
        if(gf_data_en)
            begin
                read_gf_fifo_en <= 1'b1;
              
            end 
        else if(upl_fifo_data_state == 3)read_gf_fifo_en <= 1'b0;
    end
end

(*noprune*) reg             [5:0]               upl_fifo_data_state                 ;
reg             [15: 0]                         write_fifo_cnt                      ;
always @(posedge clk or negedge rst_n) 
begin 
    if(~rst_n) 
    begin
        upl_fifo_wrreq              <=  1'b0;
        upl_fifo_wrdata             <=  32'd0;
        upl_fifo_data_state         <=  6'd0;
        write_fifo_cnt              <= 16'd0;
    end
    else
    begin
        case(upl_fifo_data_state)

            0:
            begin
                 tx_data_en          <=      0;
                 write_fifo_cnt      <=  16'd0;
                 cmd_fifo_rdreq      <=   1'b0;
                 x4_fifo_rdreq       <=   1'b0;
                 gf_fifo_rdreq       <=   1'b0;
                 upl_fifo_wrreq      <=   1'b0;


                 if(read_cmd_fifo_en)
                 begin
                    upl_fifo_data_state <= upl_fifo_data_state + 1;
                    cmd_fifo_rdreq  <=   1'b1;
                    sent_data_len   <= cmd_data_len;
                 end
                 else  if(read_x4_fifo_en)
                 begin
                    upl_fifo_data_state <= 2;
                    x4_fifo_rdreq   <=   1'b1;
                    sent_data_len   <= x4_data_len;
					  end
                else  if(read_gf_fifo_en)
                 begin
                    upl_fifo_data_state <= 3;
                    gf_fifo_rdreq       <=   1'b1;
                    sent_data_len       <=  gf_data_len;
                 end
            end
            1:
            begin
                upl_fifo_wrreq      <=   1'b1;
                upl_fifo_wrdata     <=   cmd_fifo_rddata;
                if(write_fifo_cnt   <=   sent_data_len )
                begin
                    write_fifo_cnt  <=   write_fifo_cnt  + 1'b1;                  
                  
                end 
                else
                begin
                    cmd_fifo_rdreq  <=   1'b0;                  
                    tx_data_en      <=   1;
                    tx_data_len     <=   (write_fifo_cnt + 1) * 4 + 50;
                    upl_fifo_data_state <=  0;
                end 

            end 
             2:
            begin
                upl_fifo_wrreq  <=   1'b1;
                upl_fifo_wrdata <=   x4_fifo_rddata;
                if(write_fifo_cnt   <=   sent_data_len )
                begin
                    write_fifo_cnt  <=   write_fifo_cnt  + 1'b1;
                   
                    
                end 
                else
                begin
                    x4_fifo_rdreq   <=   1'b0;                   
                    tx_data_en      <=   1;
                    tx_data_len     <=   (write_fifo_cnt  + 1) * 4 ;
                    upl_fifo_data_state <=  0;
                end 

            end

            3:
            begin
                upl_fifo_wrreq  <=   1'b1;
                upl_fifo_wrdata <=   gf_fifo_rddata;
                if(write_fifo_cnt   <=   sent_data_len )
                begin
                    write_fifo_cnt  <=   write_fifo_cnt  + 1'b1;                  
                end 
                else
                begin
                    gf_fifo_rdreq   <=   1'b0;                    
                    tx_data_en      <=   1;
                    tx_data_len     <=  ( write_fifo_cnt + 1) * 4 ;
                    upl_fifo_data_state <=  0;
                end 

            end 
            default:
            begin 
                upl_fifo_data_state  <=  6'd0;
            end 
        endcase 





    end
end





 endmodule
 