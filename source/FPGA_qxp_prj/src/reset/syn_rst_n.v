/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : syn_rst_n.v
 * Description   : syn_rst_n
 * *********************************************************************/
module  syn_rst_n(
        input                   clk                             ,
        input                   rst_n                           ,
		  
        output  wire            o_rst_n									 ,
        input [1:0]avs_address,
		  input avs_write  ,
		  input [7:0]avs_writedata		
	              
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
reg     [ 1: 0]                 rst_n_r                         ; 

//======================================================================\
//**************************** Main Code *******************************
//======================================================================/



//异步释放，同步复位
always  @(posedge clk or negedge rst_n)begin
    if(rst_n==1'b0)begin
        rst_n_r <=  0;
    end
    else begin
        rst_n_r <=  {rst_n_r[0],1'b1};
    end
end
 
assign  o_rst_n   =   rst_n_r[1];


endmodule
