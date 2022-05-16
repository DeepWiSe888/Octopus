module bgt60_top (
input   clk,    // Clock
input   rst_n,  // Asynchronous reset active low
    
);



//A1----U12---CS---TH_INT
//A2----Y13---ISR---TH_SCL
//A3----W14---LED---LCD_PWM
//A4----Y17---MISO---GND
//A5----R21---MOSI---LCD_HSYNC
//A6----R22---RST---LCD_DCLK
//A7----P22---SCLK---LCD_B7
//A8----N22---   ---LCD_B5



endmodule : bgt60_top