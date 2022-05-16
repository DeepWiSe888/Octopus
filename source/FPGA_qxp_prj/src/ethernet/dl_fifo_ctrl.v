/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : dl_fifo_ctrl.v
 * Description   : write data to the fifo 
 * *********************************************************************/
module dl_fifo_ctrl(
input                           clk                             ,
input                           rst_n                           ,

input                           cmd_flag                        ,
input   [7:0]                   cmd_data                        ,

input                           dl_fifo_rdreq                   ,
input                           dl_fifo_rdclk                   ,

output  [7:0]                   dl_fifo_rddata                  ,
output  [9:0]                   dl_fifo_rdusedw                 ,
output                          dl_fifo_rdempty


);

/************************************************************************/
/******************fifo ctrl*********************************************/
/***********************************************************************/

download_fifo dl_fifo(

        .rdclk                  ( clk                           ),
        .rdreq                  ( dl_fifo_rdreq                 ),
        .q                      ( dl_fifo_rddata                ),

        .wrclk                  ( clk                           ),
        .wrreq                  ( cmd_flag                      ),
        .data                   ( cmd_data                      ),
        
        .rdusedw                ( dl_fifo_rdusedw               ),        
        .rdempty                ( dl_fifo_rdempty               ),
        .wrfull                 (                               ),
        .wrusedw                (                               ),
);

endmodule
