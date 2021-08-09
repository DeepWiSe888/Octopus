# Embedded OS Source Codes for Octopus
This folder includes DAG scheduler system, basic linear algorithms, radar Hal driver, and user-code example for STM32F7 series.

# DAG Scheduler
DAG task flow system kernel. You can find task examples at lib/blas_tasks.* .
Task memory pool can be dynamic memory by os(mostly we use FreeRTOS), or PHY SRAM address.
Remember destroying task flow at the end of task flow works. For more information, please refer to DAG_INTRODUCE.md. 

# Lib
Basic algothrms and algorithm tasks.
Embedded Eigen is using for matrix calculations.
YOU SHOULD modify fpga_define.h as your hardware env, if using fpga.

# STM32F7
User-Code for STM32F7 * FreeRTOS sys. 

[STM32F767IG](https://www.st.com/en/microcontrollers-microprocessors/stm32f767ig.html) is selected in our system, which is a high-performance ARM Cortex-M7 32-bit RISC core operating at up to 216 MHz frequency with a floating point unit (FPU).  It also implements a full set of DSP instructions and a memory protection unit (MPU) which enhances the application security.  

## Programing Tools

The [ST-LINK/V2](https://www.st.com/en/development-tools/st-link-v2.html) is an in-circuit debugger and programmer for the STM8 and STM32 microcontrollers. The single-wire interface module (SWIM) and JTAG/serial wire debugging (SWD) interfaces are used to communicate with any STM8 or STM32 microcontroller located on an application board. In addition to providing the same functionalities as the ST-LINK/V2, the ST-LINK/V2-ISOL features digital isolation between the PC and the target application board.

## Software Requirements

### Pre-requisites

[STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (STM32CubeProg) is an all-in-one multi-OS software tool for programming STM32 products. STM32CubeProgrammer PictureIt provides an easy-to-use and efficient environment for reading, writing and verifying device memory through both the debug interface (JTAG and SWD) and the bootloader interface (UART, USB DFU, I2C, SPI, and CAN). STM32CubeProgrammer offers a wide range of features to program STM32 internal memories (such as Flash, RAM, and OTP) as well as external memories.

[STM32CubeIDE]( https://www.st.com/en/development-tools/stm32cubeide.html) is an all-in-one multi-OS development tool, which is part of the STM32Cube software ecosystem. STM32CubeIde Board PhotoSTM32CubeIDE is an advanced C/C++ development platform with peripheral configuration, code generation, code compilation, and debug features for STM32 microcontrollers and microprocessors. More feautures please refer to [STM32CubeIDE Websites]( https://www.st.com/en/development-tools/stm32cubeide.html).

[STM32CubeMX](https://www.freertos.org/FreeRTOS-Plus/BSP_Solutions/ST/STM32CubeMX.html) is a free graphical configuration and low level code generation tool for STM32 ARM Cortex-M microcontrollers. STM32CubeMX will facilitate MCU selection, suggest pin assignments, check pin assignments for consistency, create start up code, generate configuration files for middleware, and generate IDE ready projects that include STM32Cube drivers and middlware. 

### Operating System

- Windows
- Linux
- Mac OS

## Build ARM 

1. Open STM32CubeIDE 
2. Click *File>New>STM32 Project from an Existing STM32CubeMx Configuration*, and find bbb.ioc in this project. 
3. Edit main.c and add ./example.
4. Build the project.
5. Debug the project.

We also give general building steps here:

1. Create New Project using STM32CubeMX
2. Pinout Configuration
3. Clock Configuration
4. GPIO Configuration
5. Configure project and generate source Code
6. Edit main.c to Toggle the LED
7. Build the Project
8. Debug the Project

















## 

