# Embedded OS source codes for Octopus Project
Including dag scheduler system, basic linear algorithms, radar Hal driver, and user-code example for STM32F7 series.

# Dag scheduler
Dag task flow system kernel. You can find task examples at lib/blas_tasks.* .
Task memory pool can be dynamic memory by os(mostly we use FreeRTOS), or PHY sram address.
Remember destroying task flow at the end of task flow works.

#Lib
Basic algothrms and algorithm tasks.
Embedded Eigen is using for matrix calculations.
YOU SHOULD modify fpga_define.h as your hardware env, if using fpga .

#stm32f7
User-Code for STM32F7 * FreeRTOS sys.