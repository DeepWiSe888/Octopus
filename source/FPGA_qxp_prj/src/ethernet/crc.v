/************************************************************************
 * Author        : Wen Chunyang
 * Email         : 1494640955@qq.com
 * Create time   : 2018-11-19 22:27
 * Last modified : 2018-11-19 22:27
 * Filename      : crc.v
 * Description   : 
 * *********************************************************************/
module  crc(
    input                       clk                             ,
    input                       rst_n                           ,
    input         [ 7: 0]       din                             ,
    input                       en                              ,
    input                       init                            ,
    output  reg   [31: 0]       crc                             
);
//======================================================================\
//************** Define Parameter and Internal Signals *****************
//======================================================================/
wire    [ 7: 0]                 data                            ;
wire    [31: 0]                 crc_next                        ;
//======================================================================\
//**************************** Main Code *******************************
//======================================================================/
assign data={din[0],din[1],din[2],din[3],din[4],din[5],din[6],din[7]};

assign crc_next[0]  = crc[24] ^ crc[30] ^ data[0] ^ data[6];
assign crc_next[1]  = crc[24] ^ crc[25] ^ crc[30] ^ crc[31] ^ data[0] ^ data[1] ^ data[6] ^ data[7];
assign crc_next[2]  = crc[24] ^ crc[25] ^ crc[26] ^ crc[30] ^ crc[31] ^ data[0] ^ data[1] ^ data[2] ^ data[6] ^ data[7];
assign crc_next[3]  = crc[25] ^ crc[26] ^ crc[27] ^ crc[31] ^ data[1] ^ data[2] ^ data[3] ^ data[7];
assign crc_next[4]  = crc[24] ^ crc[26] ^ crc[27] ^ crc[28] ^ crc[30] ^ data[0] ^ data[2] ^ data[3] ^ data[4] ^ data[6];
assign crc_next[5]  = crc[24] ^ crc[25] ^ crc[27] ^ crc[28] ^ crc[29] ^ crc[30] ^ crc[31] ^ data[0] ^ data[1] ^ data[3] ^ data[4] ^ data[5] ^ data[6] ^ data[7];
assign crc_next[6]  = crc[25] ^ crc[26] ^ crc[28] ^ crc[29] ^ crc[30] ^ crc[31] ^ data[1] ^ data[2] ^ data[4] ^ data[5] ^ data[6] ^ data[7];
assign crc_next[7]  = crc[24] ^ crc[26] ^ crc[27] ^ crc[29] ^ crc[31] ^ data[0] ^ data[2] ^ data[3] ^ data[5] ^ data[7];
assign crc_next[8]  = crc[0]  ^ crc[24] ^ crc[25] ^ crc[27] ^ crc[28] ^ data[0] ^ data[1] ^ data[3] ^ data[4];
assign crc_next[9]  = crc[1]  ^ crc[25] ^ crc[26] ^ crc[28] ^ crc[29] ^ data[1] ^ data[2] ^ data[4] ^ data[5];
assign crc_next[10] = crc[2]  ^ crc[24] ^ crc[26] ^ crc[27] ^ crc[29] ^ data[0] ^ data[2] ^ data[3] ^ data[5];
assign crc_next[11] = crc[3]  ^ crc[24] ^ crc[25] ^ crc[27] ^ crc[28] ^ data[0] ^ data[1] ^ data[3] ^ data[4];
assign crc_next[12] = crc[4]  ^ crc[24] ^ crc[25] ^ crc[26] ^ crc[28] ^ crc[29] ^ crc[30] ^ data[0] ^ data[1] ^ data[2] ^ data[4] ^ data[5] ^ data[6];
assign crc_next[13] = crc[5]  ^ crc[25] ^ crc[26] ^ crc[27] ^ crc[29] ^ crc[30] ^ crc[31] ^ data[1] ^ data[2] ^ data[3] ^ data[5] ^ data[6] ^ data[7];
assign crc_next[14] = crc[6]  ^ crc[26] ^ crc[27] ^ crc[28] ^ crc[30] ^ crc[31] ^ data[2] ^ data[3] ^ data[4] ^ data[6] ^ data[7];
assign crc_next[15] = crc[7]  ^ crc[27] ^ crc[28] ^ crc[29] ^ crc[31] ^ data[3] ^ data[4] ^ data[5] ^ data[7];
assign crc_next[16] = crc[8]  ^ crc[24] ^ crc[28] ^ crc[29] ^ data[0] ^ data[4] ^ data[5];
assign crc_next[17] = crc[9]  ^ crc[25] ^ crc[29] ^ crc[30] ^ data[1] ^ data[5] ^ data[6];
assign crc_next[18] = crc[10] ^ crc[26] ^ crc[30] ^ crc[31] ^ data[2] ^ data[6] ^ data[7];
assign crc_next[19] = crc[11] ^ crc[27] ^ crc[31] ^ data[3] ^ data[7];
assign crc_next[20] = crc[12] ^ crc[28] ^ data[4];
assign crc_next[21] = crc[13] ^ crc[29] ^ data[5];
assign crc_next[22] = crc[14] ^ crc[24] ^ data[0];
assign crc_next[23] = crc[15] ^ crc[24] ^ crc[25] ^ crc[30] ^ data[0] ^ data[1] ^ data[6];
assign crc_next[24] = crc[16] ^ crc[25] ^ crc[26] ^ crc[31] ^ data[1] ^ data[2] ^ data[7];
assign crc_next[25] = crc[17] ^ crc[26] ^ crc[27] ^ data[2] ^ data[3];
assign crc_next[26] = crc[18] ^ crc[24] ^ crc[27] ^ crc[28] ^ crc[30] ^ data[0] ^ data[3] ^ data[4] ^ data[6];
assign crc_next[27] = crc[19] ^ crc[25] ^ crc[28] ^ crc[29] ^ crc[31] ^ data[1] ^ data[4] ^ data[5] ^ data[7];
assign crc_next[28] = crc[20] ^ crc[26] ^ crc[29] ^ crc[30] ^ data[2] ^ data[5] ^ data[6];
assign crc_next[29] = crc[21] ^ crc[27] ^ crc[30] ^ crc[31] ^ data[3] ^ data[6] ^ data[7];
assign crc_next[30] = crc[22] ^ crc[28] ^ crc[31] ^ data[4] ^ data[7];
assign crc_next[31] = crc[23] ^ crc[29] ^ data[5];

//crc 这里用下降沿采集，是为了crc能提前半拍送到eth_mac_send模块中，这个很重要
//否则crc高8位会发送出错，自己可以修改代码试试看
always  @(negedge clk or negedge rst_n)begin
    if(rst_n == 1'b0)begin
        crc <=  {32{1'b1}};
    end
    else if(init)begin
        crc <=  {32{1'b1}};
    end
    else if(en)begin
        crc <=  crc_next;
    end
end


endmodule
