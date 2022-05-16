/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-4-10
 * Last modified : 2022-4-10
 * Filename      : dac.v
 * Description   : process cmd from the dac
 * *********************************************************************/
module dac (
input                       clk                                         ,    // Clock
input                       rst_n                                       ,  // Asynchronous reset active low
output                      dac_mode                                    ,
output                      dac_clka                                    ,
output  reg     [ 7:0]      dac_da                                      ,
output                      dac_wra                                     ,
output                      dac_sleep                                      
);
 

assign dac_sleep = 0                                                    ;
assign dac_wra   = dac_clka                                             ;
assign dac_clka  = ~clk                                                 ;
assign dac_mode  = 1                                                    ;

reg [31:0]   cnt;
always  @(posedge clk or negedge rst_n)
begin
    if(rst_n==1'b0)
    begin
        cnt <= 0;                 
    end
    else 
    begin            
        if(cnt >= 1000)   cnt <= 0 ;
        else              cnt <= cnt +1 ;
    end
end


always  @(posedge clk or negedge rst_n)begin
    if(rst_n==1'b0)begin
        dac_da <= 178;                 
    end
    else 
    begin         
       
        dac_da <= 178;
    end
end

endmodule 