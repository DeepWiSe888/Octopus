# Getting to Know the Hardware

We offer information about the hardware and tips on how to use the hardware. More detail about the hardware, please refer to schematics. Here, we just list the key components in the following pictures.

## Antenna Array Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_block.png)

Above figure is the top level schematic of antenna array board. The LNAs and AMPs of Rx array are the option components for some special reqirements. 

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_board.png)

- *Tx RF interface*: This interface provides the Tx radio that allows RF signals can be transmitted at range from 0 GHz and 8 GHz. 
- *Rx RF interface*: This interface provides the Rx radio that allows RF signals can be received at range from 0 GHz and 8 GHz. 
- *Tx switch I/Os*: The header pins are used for Tx antenna switching, and connected to FPGA for controlling. 
- *Rx switch I/Os*: The header pins are used for Rx antenna switching, and connected to FPGA for controlling. 
- *Power*: The power jack is where the power supply that comes with power source.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## X4 Testing Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_board.png)

- *Tx RF interface*: This interface provides the Tx radio that allows RF signals can be transmitted at 7.29 GHz with 1.4 GHz bandwidth.
- *Rx RF interface*: This interface provides the Rx radio that allows RF signals can be received at 7.29 GHz with 1.4 GHz bandwidth.
- *X4 control I/Os*: The header pins includes SPI pins, clock synchronization pins, trigger pins, etc. which are connected to FPGA.
- *Power*: The power jack is where the power supply that comes with power source.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## Design Files

[*sch/X4_ArrayUnit.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_ArrayUnit.pdf): This  file is the schematic of the whole antenna array board.

[*sch/X4_TestingBoard.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_TestingBoard.pdf): This file describe the testing RF switch board for loopback evaluation.

