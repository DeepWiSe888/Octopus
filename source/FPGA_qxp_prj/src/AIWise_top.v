/************************************************************************
 * Author        : yebin
 * Email         : 390931229@qq.com
 * Create time   : 2022-3-4
 * Last modified : 2022-3-4
 * Filename      : AIWise_top.v
 * Description   : AIWise top 
 * *********************************************************************/

module  AIWise_top(
input                       g_clk                                       ,
input                       gmii_rx_clk                                 ,
input                       s_rst_n                                     ,
//key
input                       s2_in                                       ,
input                       s3_in                                       ,
//gmii
output  wire                gmii_tx_clk                                 ,
output  wire                gmii_tx_en                                  ,
output  wire                gmii_tx_er                                  ,
output  wire    [ 7: 0]     gmii_tx_data                                ,
input                       gmii_rx_dv                                  ,
input                       gmii_rx_er                                  ,
input           [ 7: 0]     gmii_rx_data                                ,
output  wire                phy_rst_n                                   ,

output                      x4_mosi                                     ,         // output  x4_mosi_sig
output                      x4_sclk                                     ,         // output  x4_sclk_sig
output                      x4_cs                                       ,         // output  x4_cs_sig
input                       x4_miso                                     ,         // input   x4_miso_sig

output                      x4_enable                                   ,
input                       x4_isr                                      ,

//antenna receive SWITCH
output                      r_switch_A                                  ,
output                      r_switch_B                                  ,
output                      r_switch_C                                  ,
//antenna transfer SWITCH
output                      t_switch_A                                  ,
output                      t_switch_B                                  ,
output                      t_switch_C                                  ,

//GFRADAR03
output                      adc_clk                                     ,
input                       adc_otr                                     ,
input           [ 7: 0]     adc_data                                    ,

output                      dac_mode                                    ,
output                      dac_clka                                    ,
output   wire   [ 7:0]      dac_da                                      ,
output                      dac_wra                                     ,
output                      dac_sleep                                   ,
output          [ 1: 0]     tx_v                                        ,
output          [ 6: 0]     rx_v                                        ,
input                       sync_in                                     ,

//60G
output                      BGT60_mosi                                  ,
output                      BGT60_sclk                                  ,
output                      BGT60_cs                                    ,
input                       BGT60_miso                                  ,

output                      BGT60_rst                                   ,
output                      BGT60_led                                   ,
input                       BGT60_isr                                   ,
output                      BGT60_scl                                   ,
inout                       BGT60_sda
);


/************************************************************************/
//                           clk module
/***********************************************************************/
assign  phy_rst_n           =   1'b1;
assign  gmii_tx_clk         =   clk;



wire                        clk                                         ;
wire                        rst_n                                       ;


global_clk  global_clk_inst(
        .inclk              (gmii_rx_clk                                ),
        .outclk             (clk                                        )
);

pll         u_my_pll(
        .inclk0             (g_clk                                      ),
        .c0                 (adc_clk                                    )
);

syn_rst_n   syn_rst_n_inst(
        .clk                (clk                                        ),
        .rst_n              (s_rst_n                                    ),
        .o_rst_n              (rst_n                                      )
);



dac     dac_inst(
        .clk                (g_clk                                      ),
        .rst_n              (rst_n                                      ),
        .dac_mode           (dac_mode                                   ),
        .dac_sleep          (dac_sleep                                  ),
        .dac_wra            (dac_wra                                    ),
        .dac_clka           (dac_clka                                   ),
        .dac_da             (dac_da                                     )
);


/************************************************************************/
//                           key   module
/***********************************************************************/
// arp_debounce    arp_debounce_inst(
//         .clk                (clk                                        ),
//         .rst_n              (rst_n                                      ),
//         //key
//         .key_in             (s2_in                                      ),
//         .key_out            (arp_req_tx                                 ) //
// );

// udp_debounce    udp_debounce_inst(
//         .clk                (clk                                        ),
//         .rst_n              (rst_n                                      ),
//         //key
//         .key_in             (s3_in                                      ),
//         .key_out            (udp_wr                                     )
// );

wire                        dl_fifo_rdreq                               ;
wire    [ 9: 0]             dl_fifo_rdusedw                             ;
wire    [ 7: 0]             dl_fifo_rddata                              ;
wire                        dl_fifo_rdempty                             ;

wire                        tx_data_en                                  ;
wire    [15: 0]             tx_data_len                                 ;
wire                        upl_fifo_wrreq                              ;
wire    [31: 0]             upl_fifo_wrdata                             ;
wire                        upl_fifo_wrfull                             ;

//eth module
eth_top eth
(

        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        .gmii_tx_en         (gmii_tx_en                                 ),
        .gmii_tx_er         (gmii_tx_er                                 ),
        .gmii_tx_data       (gmii_tx_data                               ),
        .gmii_rx_dv         (gmii_rx_dv                                 ),
        .gmii_rx_er         (gmii_rx_er                                 ),
        .gmii_rx_data       (gmii_rx_data                               ),
        .arp_req            (arp_req                                    ),

        .tx_data_len        (tx_data_len                                ),
        .tx_data_en         (tx_data_en                                 ),
        .upl_fifo_wrreq     (upl_fifo_wrreq                             ),
        .upl_fifo_wrdata    (upl_fifo_wrdata                            ),
        .upl_fifo_wrfull    (upl_fifo_wrfull                            ),

        .cmd_flag           (                                           ),
        .cmd_data           (                                           ),
        .dl_fifo_rdreq      (dl_fifo_rdreq                              ),
        .dl_fifo_rdempty    (dl_fifo_rdempty                            ),
        .dl_fifo_rdusedw    (dl_fifo_rdusedw                            ),
        .dl_fifo_rddata     (dl_fifo_rddata                             ),
        .send_finish        (send_finish                                )
);

write_upl_fifo_ctrl write_fifo_inst(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),
        .tx_data_len        (tx_data_len                                ),
        .tx_data_en         (tx_data_en                                 ),
        .upl_fifo_wrreq     (upl_fifo_wrreq                             ),
        .upl_fifo_wrdata    (upl_fifo_wrdata                            ),
        .upl_fifo_wrfull    (upl_fifo_wrfull                            ),
        .send_finish        (send_finish                                ),

        //cmd control

        .cmd_tx_data_len    (cmd_tx_data_len                            ),
        .cmd_tx_data_done   (cmd_tx_data_done                           ),
        .cmd_fifo_rdreq     (cmd_fifo_rdreq                             ),
        .cmd_fifo_rddata    (cmd_fifo_rddata                            ),
        .cmd_fifo_rdusedw   (cmd_fifo_rdusedw                           ),
        .cmd_fifo_rdempty   (cmd_fifo_rdempty                           ),
        //data from x4
        .x4_tx_data_len     (x4_tx_data_len                             ),
        .x4_tx_data_done    (x4_tx_data_done                            ),
        .x4_fifo_rdreq      (x4_fifo_rdreq                              ),
        .x4_fifo_rddata     (x4_fifo_rddata                             ),
        .x4_fifo_rdempty    (x4_fifo_rdempty                            ),
        .x4_fifo_rdusedw    (x4_fifo_rdusedw                            ),
        //gf radar data
        .gf_tx_data_len     (gf_tx_data_len                             ),
        .gf_tx_data_done    (gf_tx_data_done                            ),

        .gf_fifo_rdreq      (gf_fifo_rdreq                              ),
        .gf_fifo_rddata     (gf_fifo_rddata                             ),
        .gf_fifo_rdusedw    (gf_fifo_rdusedw                            ),
        .gf_fifo_rdempty    (gf_fifo_rdempty                            ),




);

wire    [15: 0]             read_frame_size                             ;

wire    [15: 0]             cmd_tx_data_len                             ;
wire                        cmd_tx_data_done                            ;
wire                        cmd_fifo_rdreq                              ;
wire    [31: 0]             cmd_fifo_rddata                             ;
wire    [ 7: 0]             cmd_fifo_rdusedw                            ;
wire                        cmd_fifo_rdempty                            ;

wire    [ 3: 0]             x4_tx_chanle_en                             ;
wire    [15: 0]             x4_rx_chanle_en                             ;
wire    [ 3: 0]             gf_tx_chanle_en                             ;
wire    [15: 0]             gf_rx_chanle_en                             ;
wire    [15: 0]             gf_fps                                      ;
wire    [15: 0]             gf_sampleRate                               ;


cmd_ctrl u4_pc_cmd(
        .clk                (clk                                        ),
        .reset_n            (rst_n                                      ),

//read cmd from fifo
        .dl_fifo_rdreq      (dl_fifo_rdreq                              ),
        .dl_fifo_rddata     (dl_fifo_rddata                             ),
        .dl_fifo_rdempty    (dl_fifo_rdempty                            ),
        .dl_fifo_rdusedw    (dl_fifo_rdusedw                            ),
//to spi module
        .spi_tx_data        (cmd_spi_data_in                            ),
        .spi_rx_data        (spi_data_out                               ),
        .spi_cs             (cmd_spi_cs                                 ),
        .spi_tx_en          (cmd_spi_tx_en                              ),
        .spi_rx_en          (cmd_spi_rx_en                              ),
        .spi_tx_done        (spi_tx_done                                ),
        .spi_rx_done        (spi_rx_done                                ),

//write to fifo and send to PC
        .tx_data_len        (cmd_tx_data_len                            ),
        .tx_data_done       (cmd_tx_data_done                           ),

        .fifo_rdreq         (cmd_fifo_rdreq                             ),
        .fifo_rddata        (cmd_fifo_rddata                            ),
        .fifo_rdusedw       (cmd_fifo_rdusedw                           ),
        .fifo_rdempty       (cmd_fifo_rdempty                           ),
        //cmd frome  pc
        .x4_enable          (x4_enable                                  ),
        .x4_init_done       (x4_init_done                               ),
        .read_frame_size    (read_frame_size                            ),
        .arp_req            (arp_req                                    ),
        .x4_tx_chanle_en    (x4_tx_chanle_en                            ),
        .x4_rx_chanle_en    (x4_rx_chanle_en                            ),
        .gf_tx_chanle_en    (gf_tx_chanle_en                            ),
        .gf_rx_chanle_en    (gf_rx_chanle_en                            ),
        .gf_fps             (gf_fps                                     ),
        .gf_sampleRate      (gf_sampleRate                              ),
        .gf_enable          (gf_enable                                  ),
);

//spi_driver
wire    [ 7: 0]             spi_data_in                                 ;
wire    [ 7: 0]             spi_data_out                                ;
wire                        spi_tx_en                                   ;
wire                        spi_rx_en                                   ;
wire                        spi_tx_done                                 ;
wire                        spi_rx_done                                 ;
wire                        spi_cs                                      ;
wire    [ 7: 0]             cmd_spi_data_in                             ;
wire    [ 7: 0]             fpga_spi_data_in                            ;
wire    [ 7: 0]             fpga_data_in                                ;
wire                        miso                                        ;

spi_module u5_spi_driver(
        .I_clk              (clk                                        ),
        .I_rst_n            (rst_n                                      ),
        .I_rx_en            (spi_rx_en                                  ),
        .I_tx_en            (spi_tx_en                                  ),
        .I_data_in          (spi_data_in                                ),
        .I_cs               (spi_cs                                     ),

        .O_data_out         (spi_data_out                               ),
        .O_tx_done          (spi_tx_done                                ),
        .O_rx_done          (spi_rx_done                                ),

        .I_spi_miso         (miso                                       ),    // SPI miso
        .O_spi_sck          (x4_sclk                                    ),    // SPI clk
        .O_spi_cs           (x4_cs                                      ),    // SPI cs
        .O_spi_mosi         (x4_mosi                                    ),    // SPI mosi

);


assign  BGT60_mosi                       =   x4_mosi                                             ;
assign  BGT60_sclk                       =   x4_sclk                                             ;
assign  BGT60_cs                         =   x4_cs                                               ;

parameter                       BGT60_EN = 0                                                     ;
assign  miso                             =   BGT60_EN     ? BGT60_miso        :  x4_miso         ;
assign  BGT60_rst                        =   rst_n                                               ;
assign  spi_cs                           =   x4_init_done ? fpga_spi_cs       :  cmd_spi_cs      ;
assign  spi_tx_en                        =   x4_init_done ? fpga_spi_tx_en    :  cmd_spi_tx_en   ;
assign  spi_rx_en                        =   x4_init_done ? fpga_spi_rx_en    :  cmd_spi_rx_en   ;
assign  spi_data_in                      =   x4_init_done ? fpga_spi_data_in  :  cmd_spi_data_in ;


// X4 drive
wire    [15: 0]             x4_tx_data_len                              ;
wire                        x4_tx_data_done                             ;
wire                        x4_fifo_rdreq                               ;
wire    [31: 0]             x4_fifo_rddata                              ;
wire                        x4_fifo_rdempty                             ;
wire    [15: 0]             x4_fifo_rdusedw                             ;
uwb_top uwb_radar(
        .clk                (clk                                        ),
        .rst_n              (rst_n                                      ),

        .tx_chanle_en       (                                           ),
        .rx_chanle_en       (                                           ),

        .spi_cs             (fpga_spi_cs                                ),
        .spi_tx_en          (fpga_spi_tx_en                             ),
        .spi_rx_en          (fpga_spi_rx_en                             ),
        .spi_data_in        (fpga_spi_data_in                           ),
        .spi_data_out       (spi_data_out                               ),
        .spi_tx_done        (spi_tx_done                                ),
        .spi_rx_done        (spi_rx_done                                ),

        .x4_isr             (x4_isr                                     ),
        .tx_sw_a            (t_switch_A                                 ),
        .tx_sw_b            (t_switch_B                                 ),
        .tx_sw_c            (t_switch_C                                 ),

        .rx_sw_a            (r_switch_A                                 ),
        .rx_sw_b            (r_switch_B                                 ),
        .rx_sw_c            (r_switch_C                                 ),

        .x4_init_done       (x4_init_done                               ),
        .read_frame_size    (read_frame_size                            ),


        .tx_data_len        (x4_tx_data_len                             ),
        .tx_data_done       (x4_tx_data_done                            ),

        .fifo_rdreq         (x4_fifo_rdreq                              ),
        .fifo_rddata        (x4_fifo_rddata                             ),
        .fifo_rdusedw       (x4_fifo_rdusedw                            ),
        .fifo_rdempty       (x4_fifo_rdempty                            ),

);

//gf_radar driver


wire    [15 : 0]            gf_tx_data_len                              ;
wire                        gf_tx_data_done                             ;
wire                        gf_fifo_rdreq                               ;
wire                        gf_fifo_rdempty                             ;
wire    [15: 0]             gf_fifo_rdusedw                             ;
wire    [31: 0]             gf_fifo_rddata                              ;
wire    [ 3: 0]             gf_tx_chanle                                ;
wire    [15: 0]             gf_rx_chanle                                ;
//assign gf_tx_chanle = 4'b1111;
//assign gf_rx_chanle = 16'b1111111111111111;
fmcw_radar_controler fmcw_radar(

        .adc_clk                   (adc_clk                             ),  // Clock
        .clk                       (clk                                 ),
        .rst_n                     (rst_n                               ),  // Asynchronous reset active low

        .sync                      (sync_in                             ),
        .tx_chanle_en              (gf_tx_chanle_en                     ),
        .rx_chanle_en              (gf_rx_chanle_en                     ),
        .gf_enable                    (gf_enable                        ),
        .gf_sampleRate             (gf_sampleRate                       ),
        .gf_fps                    (gf_fps                              ),
        .ad_data                   (adc_data                            ),
        .ad_otr                    (adc_otr                             ),

        .tx_sw                     (tx_v                                ),
        .rx_sw                     (rx_v                                ),

        .tx_data_len               (gf_tx_data_len                      ),
        .tx_data_done              (gf_tx_data_done                     ),    

        .fifo_rdreq                (gf_fifo_rdreq                       ),
        .fifo_rddata               (gf_fifo_rddata                      ),
        .fifo_rdusedw              (gf_fifo_rdusedw                     ),
        .fifo_rdempty              (gf_fifo_rdempty                     ),

);

// assign tx_v = tx_sw;
// assign rx_v = rx_sw;
endmodule
