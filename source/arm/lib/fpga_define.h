/*
 * Project Octopus
 *
 */


#ifndef INC_FPGA_DEFINE_H_
#define INC_FPGA_DEFINE_H_

#ifdef _USE_FPGA

#include "fsmc.h"


#define _FPGA_RAWDATA
#define _FPGA_FIR
#define _FPGA_FFT
//#define _FPGA_VMD

#define _FPGA_CNN



#define FMC_ADDR_DATA				(101)

#define FMC_ADDR_RADAR_DATA_FLAG		(53)
#define FMC_ADDR_CNN_DATA_FLAG			(54)

#endif


#endif /* INC_FPGA_DEFINE_H_ */
