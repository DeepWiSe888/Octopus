# Embedded OS Source Codes for Octopus
This folder includes DAG scheduler system, basic linear algorithms, radar Hal driver, and user-code examples for STM32F7 series.

# DAG Scheduler
DAG scheduler system is used for parallelization and scheduling of different tasks. You can find task examples at lib/blas_tasks.* .
Task memory pool can be dynamic memory allocated by OS (we mainly use FreeRTOS), or PHY SRAM address.
Remember to destroy task flow at the end of task flow works. For more information, please refer to DAG_INTRODUCE.md. 

# Lib
Basic algothrithms and algorithm tasks.
Embedded Eigen is using for matrix calculations.
YOU SHOULD modify fpga_define.h according to your hardware environment, if you are using FPGA.

# STM32F7
User-Code for STM32F7 * FreeRTOS system. 

[STM32F767IG](https://www.st.com/en/microcontrollers-microprocessors/stm32f767ig.html) is selected in our system. It is a high-performance ARM Cortex-M7 32-bit RISC processor operating at up to 216 MHz frequency with a floating point unit (FPU).  It also implements a full set of DSP instructions and a memory protection unit (MPU) which enhances the application security.  

## Programing Tools

The [ST-LINK/V2](https://www.st.com/en/development-tools/st-link-v2.html) is an in-circuit debugger and programmer for the STM8 and STM32 microcontrollers. The single-wire interface module (SWIM) and JTAG/serial wire debugging (SWD) interfaces are used to communicate with any STM8 or STM32 microcontroller located on an application board. In addition to providing the same functionalities as the ST-LINK/V2, the ST-LINK/V2-ISOL also features digital isolation between the PC and the target application board.

## Software Requirements

### Pre-requisites

[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (STM32CubeProg) is an all-in-one multi-OS software tool for programming STM32 products. STM32CubeProgrammer PictureIt provides an easy-to-use and efficient environment for reading, writing and verifying device memory through both the debug interface (JTAG and SWD) and the bootloader interface (UART, USB DFU, I2C, SPI, and CAN). STM32CubeProgrammer offers a wide range of features to program STM32 internal memories (such as Flash, RAM, and OTP) as well as external memories.

[STM32CubeIDE]( https://www.st.com/en/development-tools/stm32cubeide.html) is an all-in-one multi-OS development tool, which is part of the STM32Cube software ecosystem. STM32CubeIde Board PhotoSTM32CubeIDE is an advanced C/C++ development platform with peripheral configuration, code generation, code compilation, and debug features for STM32 microcontrollers and microprocessors. More feautures please refer to [STM32CubeIDE Websites]( https://www.st.com/en/development-tools/stm32cubeide.html).

[STM32CubeMX](https://www.freertos.org/FreeRTOS-Plus/BSP_Solutions/ST/STM32CubeMX.html) is a free graphical configuration and low-level code generation tool for STM32 ARM Cortex-M microcontrollers. STM32CubeMX facilitates MCU selection, suggests pin assignments, checks pin assignments for consistency, creates start-up code, generates configuration files for middleware, and generates IDE-ready projects that include STM32Cube drivers and middlware. 

### Operating Systems

- Windows
- Linux
- Mac OS

## Build the ARM Project 

In the design workflow of Octopus, the ARM embedded system serves as the logic control of the main program process. In most cases, the ARM processor also assumes part of computation tasks, such as regression, modal decomposition, or large parameter size neural network predictions.

The ARM system requires the correct configuration of clock, I/O interface and interrupt logic (especially ARM is actually sync with FPGA module through GPIO interrupt),  FMC and memory access. We provide a configured .ioc file that can be opened by STM32CubeIDE. However, due to the difference in development environment, you still need to modify some code to integrate it to your project.



1. Open STM32CubeIDE 
2. Click *File>New>STM32 Project from an Existing STM32CubeMx Configuration*, and find bbb.ioc in this project. Now you can see the clock and I / O interface and the Software Packs configurations, remember to modify the configurations such as USART number and I/O interrupt pins, or connect hardware according to our configuration. We use the PH2 pulling up as a synchronization signal from FPGA, and USART2 for printing output.
3. Add arm/lib directory to project, which contains the DAG system code, and implementations of matrix structures and the basic linear algebra subsystem.
4. Add the vital_sign or occupancy example directory to project.
5. Refer to the / * user code * / blocks in arm/stm32f7/Core/Src/main.c and stm32f7xx_it.c,  modify your corresponding c file, mainly USART and GPIO interrupts and semaphore logics. We do not recommend you to copy files directly because there may be differences in source code distributions corresponding to different versions of IDE.
6. Build and run the project.
7. If your host PC is connected to the USART interface through the USB TTL connector, you can check the program outputs in UART tool.



We also give general building steps here:

1. Create new project using STM32CubeMX
2. Pinout configuration
3. Clock configuration
4. GPIO configuration
5. Configure project and generate source Code
6. Edit main.c to toggle the LED
7. Build the project
8. Debug the project


## 

