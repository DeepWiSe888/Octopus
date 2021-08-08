# FPGA Project for Octopus

FPGA is one of the key component of the Octopus system. We shall need to know the specific FPGA device on our board to build custom projects using corresponding design tools. For the version 1.0 prototype of Octopus, we leverage Intel [Cyclone IV EP4CE15 FPGA](https://ark.intel.com/content/www/us/en/ark/products/210469/cyclone-iv-ep4ce15-fpga.html) to build it, and hence, choose the Intel Quartus Prime design tools. Intel Quartus Prime is programmable logic device design software produced by Intel; prior to Intel's acquisition of Altera the tool was called Altera Quartus Prime, earlier Altera Quartus II. Quartus Prime enables analysis and synthesis of HDL designs, which enables the developer to compile their designs, perform timing analysis, examine RTL diagrams, simulate a design's reaction to different stimuli, and configure the target device with the programmer. Quartus Prime includes an implementation of VHDL and Verilog for hardware description, visual editing of logic circuits, and vector waveform simulation. 

*PS*: In the future, we may upgrade the Octopus using Xilinx Zynq UltraScale+ MPSoC solution to replace this ARM+FPGA board.

## Design Flow

The standard FPGA design flow starts with design entry using schematics or a hardware description language (HDL), such as Verilog HDL or VHDL.  The digital circuit is implemented inside the FPGA. The flow then proceeds through compilation, simulation, programming, and verification in the FPGA hardware. The following figure shows the design flow of FPGA.

<p align="center">
   <img src="https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/design_flow.png" alt="[YOUR_ALT]"/>
</p>

## Software Requirements

### The Version of Design Tools

We use the Intel FPGA Complete Design Suite v13.1 in our project. To fully install the Intel FPGA Complete Design Suite v13.1, 31GB of available disk space on the drive or partition is required to install the hardware. For more details about Quartus II software and device, please refer to the following links:

-  [Quartus II Handbook](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/hb/qts/archives/quartusii_handbook_archive_131.pdf)
-  [Intel FPGA Software Installation and Licensing Manual](https://www.altera.com/content/altera-www/global/en_us/index/documentation/esc1425946071433.html)
-  [Quartus II Software and Device Support Release Notes (PDF)](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/rn/archives/rn_qts_131_dev_support.pdf)



### Operating System

- Windows
- Linux



## Development Tools

Intel FPGA download cable: [USB-Blaster](https://www.intel.com/content/dam/www/programmable/us/en/pdfs/literature/ug/ug_usb_blstr.pdf) is shown in the following figure. 

<p align="center">
   <img src="https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/usb_blaster.jpg" alt="[YOUR_ALT]"/>
</p>

The Intel FPGA download cable interfaces a USB port on a host computer to an FPGA mounted on a printed circuit board. The cable sends configuration data from the host computer to a standard 10-pin header connected to the FPGA. You can use the Intel FPGA download cable to iteratively download configuration data to a system during prototyping or to program data into the system during production. For more information about how to use such download cable, please refer to [Intel download cables](https://www.intel.com/content/www/us/en/programmable/products/boards_and_kits/download-cables.html).

## Build the FPGA project:

1. Open the Quartus II software, and select *File > New Project Wizard*.
2. Click *Next*, and enter the following information: *working directory of this project*, *name of this project* and *top-level design entity of this project*.
3. Click *Finish*. When prompted to create the directory, choose *Yes*. 
4. Select *Empty Project,* and then click *Next*.   
5. Add all files in *./FPGA*, and click *Next*. Key verilog files have been already added in this folder, and the top-level entity file is in core/top.v. 
6. Family, Device, and Board Settings. Note: Since in our design, FPGA only uses FMC inferace to communicate with ARM, any FPGA evaluation board can be configured to work.
7. Right click on Analysis and Synthesis”, and then click *Start* to perform a syntax check and synthesis of the Verilog code.
8. Create an SDC File. Before we compile the Verilog code, we need to provide timing constraints for the design. We create an SDC (Synopsis Design Constraints) file that contains commands to let the Intel Quartus software know how to close timing on the design. Without it, we will get warning messages in the compile flow because the Intel Quartus software has no idea how to close timing on the design.  
9. Right click on *Compile Design,* and then click *Start*. The tools will then synthesize, place and route, assemble and perform timing analysis on the design Because there are only have a handful of code lines, the compilation should only take a couple of minutes to complete.
10. Program the FPGA. We connect the board to our computer via the USB blaster port. Right click to open *Program Device*.
11. Select *Hardware Setup*.
12. Add the .sof file. Check the *Program/Configure* column, and then click *Start*.
