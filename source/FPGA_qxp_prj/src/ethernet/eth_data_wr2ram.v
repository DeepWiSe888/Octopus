module eth_data_wr2ram
(
	

        input         [ 8: 0]   tx_data_len                     ,
        input                   tx_data_en                      ,
        output        [ 8: 0]   upload_ram_rdaddr               ,
        input         [31: 0]   upload_ram_rddata               ,

        output        [ 8: 0]   download_ram_len                ,
        output                  download_ram_done               ,
        output        [31: 0]   download_ram_wdata              ,
        output        [ 8: 0]   download_ram_waddr              ,
        output                  download_ram_wren         


	
);
endmodule