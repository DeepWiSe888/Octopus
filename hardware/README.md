# Getting to Know the Hardware

We offer information about the hardware and tips on how to use the hardware. More detail about the hardware, please refer to schematics. Here, we just list the key components in the following pictures.

## Antenna Array Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_block.png)

Above figure is the top level schematic of antenna array board. The PA, LNAs and AMPs of Tx/Rx array are the option components for some special reqirements. 



![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_board.png)

- *Tx RF interface*: This interface provides the Tx radio that allows RF signals can be transmitted at range from 0 GHz and 8 GHz. 
- *Rx RF interface*: This interface provides the Rx radio that allows RF signals can be received at range from 0 GHz and 8 GHz. 
- *Tx switch I/Os*: The header pins are used for Tx antenna switching, and connected to FPGA for controlling. 
- *Rx switch I/Os*: The header pins are used for Rx antenna switching, and connected to FPGA for controlling. 
- *Power*: The power jack is where the power supply that comes with power source.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## X4 Evaluation Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_eval_block.png)

The top  level schematic of X4 evaluation board is shown in the above figure. The Rx/Tx port of X4 chip is differential  output, and then the RF balun is applied to transform it to single Rx/Tx port. 



![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_board.png)

- *Tx RF interface*: This interface provides the Tx radio that allows RF signals can be transmitted at 7.29 GHz with 1.4 GHz bandwidth.
- *Rx RF interface*: This interface provides the Rx radio that allows RF signals can be received at 7.29 GHz with 1.4 GHz bandwidth.
- *X4 control I/Os*: The header pins includes SPI pins, clock synchronization pins, trigger pins, etc. which are connected to FPGA.
- *Power*: The power jack is where the power supply that comes with power source.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## X4 Chip Circuit Overview

X4 chip is a radar transceiver SoC for UWB applications.  The internal circuit overview of X4 is shown in the below figure, and it also illustrates that why we design the hardware in the previous sections. This figure come from Page 12th of the X4 chip datasheet.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_circuit.jpg)



### Tx Data Path

The Tx path of the X4 consists of a pulse generator capable of generating pulses at a rate of up to 60.75 MHz. The output frequency and bandwidth is designed to fit worldwide regulatory requirements.

### Rx Data Path

The Rx path of the X4 consists of a Low Noise Amplifier (LNA), a Digital-to-Analog Converter (DAC) for threshold setting, 1536 parallel comparators/samplers and digital integrators as well as an output memory buffer, accessible through the SPI. The Rx is tightly integrated with the transmitter (Tx) and is designed for coherent integration of the received energy.

### Clock

There are two kinds of clocks: internal and external clocks. 

#### Internal Clock

The crystal oscillator (XOSC) is designed for providing the radar transceiver core with a stable and accurate reference clock signal.  Following figure shows a typical block schematic when the X4 is clocked using the XOSC.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_interClock.jpg)

#### External Clock

- In XOSC bypass mode the radar core is clocked by an external low frequency clock source. This mode can be useful if there is already a clock signal available on the application PCB. The external clock input pin supports single-ended inputs. Following figure shows a typical block schematic when the X4 is clocked using the XOSC bypass mode.

  ![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_exClock1.jpg)

- In Common PLL bypass mode the radar transceiver core is clocked by an external high frequency clock from the LVDS interface on pins IO5 and IO6 and the Common PLL is bypassed. This allows multiple radars to stay synchronized which is a requirement for digital beamforming applications. Following figure shows a block diagram of the Common PLL in bypass mode.

  ![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_exClock2.jpg)

For more information, please refer to X4 chip [datasheet](https://www.radartutorial.eu/19.kartei/13.labs/pubs/x4_datasheet_revE_restricted.pdf).

## RF port

The 50 ohm SMA jack is employed to connect to each RF interface. This connector is a standard thread and standard polarity, SMA jack.  

## Power

All the boards requires an external 5VDC power supply. 

## Design Files

[*sch/X4_ArrayUnit.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_ArrayUnit.pdf): This  file is the schematic of the whole antenna array board.

[*sch/X4_TestingBoard.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_TestingBoard.pdf): This file describe the testing RF switch board for loopback evaluation.

[*sch/X4_DevBoard.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_DevBoard.pdf): This file is the X4 Evaluation Board