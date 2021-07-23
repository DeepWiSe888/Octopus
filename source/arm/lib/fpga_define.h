/*
 * Project Octopus
 *
 */


#ifndef INC_FPGA_DEFINE_H_
#define INC_FPGA_DEFINE_H_


#define _FPGA_RAWDATA
#define _FPGA_FIR
#define _FPGA_FFT
//#define _FPGA_VMD

#define _FPGA_CNN



//TODO : fix your FSMC addr
#define fpga_read(addr)	          (*((unsigned short*)(0x60000000+addr)))
#define fpga_write(addr,data)     {*((unsigned short*)(0x60000000+addr)) = data;}

#define FMC_ADDR_DATA				(101)

#define FMC_ADDR_RADAR_DATA_FLAG		(53)
#define FMC_ADDR_CNN_DATA_FLAG			(54)



#endif /* INC_FPGA_DEFINE_H_ */
