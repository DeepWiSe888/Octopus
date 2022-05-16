/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : eth_top.v
 * Description   : eth ctrl
 * *********************************************************************/
module eth_top
(
input                       clk                                         ,
input                       rst_n                                       ,
output  wire                gmii_tx_clk                                 ,
output  wire                gmii_tx_en                                  ,
output  wire                gmii_tx_er                                  ,
output  wire  [ 7: 0]       gmii_tx_data                                ,
input                       gmii_rx_dv                                  ,
input                       gmii_rx_er                                  ,
input         [ 7: 0]       gmii_rx_data                                ,
input                       arp_req                                     ,

input         [15: 0]       tx_data_len                                 ,
input                       tx_data_en                                  ,

input   wire                upl_fifo_wrreq                              ,
input   wire  [31: 0]       upl_fifo_wrdata                             ,
output  wire                upl_fifo_wrfull                             ,

output                      cmd_flag                                    ,
output        [ 7: 0]       cmd_data                                    ,   

input                       dl_fifo_rdreq                               ,
output                      dl_fifo_rdempty                             ,
output        [ 9: 0]       dl_fifo_rdusedw                             ,
output        [ 7: 0]       dl_fifo_rddata                              ,

output                      send_finish

);

/************************************************************************/
//                           Define Parameter and Internal Signals 
/***********************************************************************/
//src
parameter     src_mac        =   48'h00_0a_35_01_fe_c0                  ;
parameter     src_ip         =   32'hc0_a8_00_02                        ;
parameter     src_port       =   16'd5000                               ; 
//dst
//parameter   dst_mac         =   48'h30_9c_23_60_14_6d           ;
parameter     dst_ip         =   32'hc0_a8_00_03                        ;
parameter     dst_port       =   16'd6000                               ; 
//eth
parameter     udp_type       =   16'h0800                               ;
parameter     arp_type       =   16'h0806                               ;
//parameter     udp_data_len   =   16'd1282                               ;



wire    [31: 0]             crc                                         ;
wire                        en                                          ;
wire                        init                                        ;
wire                        udp_tx                                      ;
wire                        arp_ack_tx                                  ;
wire                        arp_req_tx                                  ;
wire                        udp_wr                                      ;
wire                        udp_trig                                    ;
wire                        arp_ack_trig                                ;
wire                        arp_req_trig                                ;
wire    [15: 0]             ip_len                                      ;
wire    [ 7: 0]             ip_data                                     ;
wire    [ 7: 0]             arp_data                                    ;
//arp
wire    [31: 0]             arp_dst_ip                                  ;
wire                        arp_ack                                     ;
wire    [47: 0]             dst_mac                                     ;

//upload fifo
wire    [ 7: 0]             upl_fifo_rddata                             ;

/************************************************************************/
//                           
/***********************************************************************/


eth_mac_send    eth_mac_send_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        .udp_tx             (tx_data_en                                 ),
        .arp_ack_tx         (arp_ack_tx                                 ),
        .arp_req            (arp_req                                    ),
        //Eth
        .ip_len             (ip_len                                     ),
        .src_mac            (src_mac                                    ),
        .dst_mac            (dst_mac                                    ),
        .udp_type           (udp_type                                   ),
        .arp_type           (arp_type                                   ),
        .ip_data            (ip_data                                    ),
        .arp_data           (arp_data                                   ),
        .udp_trig           (udp_trig                                   ),
        .arp_ack_trig       (arp_ack_trig                               ),
        .arp_req_trig       (arp_req_trig                               ),
        //crc
        .crc                (crc                                        ),
        .init               (init                                       ),
        .en                 (en                                         ),
        //gmii
        .gmii_tx_en         (gmii_tx_en                                 ),
        .gmii_tx_er         (gmii_tx_er                                 ),
        .gmii_tx_data       (gmii_tx_data                               )
);

eth_udp_send    eth_udp_send_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        //Eth
        .src_port           (src_port                                   ),
        .dst_port           (dst_port                                   ),
        .src_ip             (src_ip                                     ),
        .dst_ip             (dst_ip                                     ),
        .udp_data_len       (tx_data_len                                ),
        .udp_trig           (udp_trig                                   ),
        .ip_len             (ip_len                                     ),
        .ip_data            (ip_data                                    ),
         //wfifo
        .wfifo_rd_data      (upl_fifo_rddata                            ),
        .wfifo_rd_en        (upl_fifo_rdreq                             ),
        .send_finish        (send_finish                                )   
);

eth_arp_send    eth_arp_send_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        //Eth
        .arp_ack_trig       (arp_ack_trig                               ),
        .arp_req_trig       (arp_req_trig                               ),
        .arp_src_ip         (src_ip                                     ),
        .arp_dst_ip         (dst_ip                                     ),
        .arp_src_mac        (src_mac                                    ),
        .arp_dst_mac        (dst_mac                                    ),
        .arp_data           (arp_data                                   )
);

crc crc_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        .din                (gmii_tx_data                               ),
        .en                 (en                                         ),
        .init               (init                                       ),
        .crc                (crc                                        )
);

eth_receive eth_receive_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        //gmii
        .gmii_rx_dv         (gmii_rx_dv                                 ),
        .gmii_rx_data       (gmii_rx_data                               ),
        .gmii_rx_er         (gmii_rx_er                                 ),
        .arp_ack_tx         (arp_ack_tx                                 ),
        .dst_mac            (dst_mac                                    ),
        .cmd_flag           (cmd_flag                                   ),
        .cmd_data           (cmd_data                                   ),        

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

/************************************************************************/
/******************fifo ctrl*********************************************/
/***********************************************************************/

upl_fifo upl_fifo(

        .rdclk                  ( clk                           ),
        .rdreq                  ( upl_fifo_rdreq                ),
        .q                      ( upl_fifo_rddata               ),

        .wrclk                  ( clk                           ),
        .wrreq                  ( upl_fifo_wrreq                ),
        .data                   ( upl_fifo_wrdata               ),
        
        .rdusedw                ( upl_fifo_rdusedw              ),        
        .rdempty                ( upl_fifo_rdempty              ),
        .wrfull                 ( upl_fifo_wrfull               ),
        .wrusedw                (                               ),
);



endmodule
