# Getting to Know the Hardware

We offer information about the hardware and tips on how to use the hardware. For more details about the hardware, please refer to the schematics. Here, we just list the key components in the following pictures.

## Antenna Array Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_block.png)

The above figure is the top-level schematic of antenna array board. The PA, LNAs and AMPs of Tx/Rx array are optional components for some special reqirements. 



![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/array_board.png)

- *Tx RF interface*: This interface connects to the Tx radio that allows RF signals to be transmitted at range from 0 GHz and 8 GHz. 
- *Rx RF interface*: This interface connects to the Rx radio that allows RF signals to be received at range from 0 GHz and 8 GHz. 
- *Tx switch I/Os*: The header pins are used for Tx antenna switching. FPGA sends the controlling signals through these I/O pins. 
- *Rx switch I/Os*: The header pins are used for Rx antenna switching. FPGA sends the controlling signals through these I/O pins. 
- *Power*: The power jack is used to connect to the power supply.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## Principle of the MIMO Radar
To understand the working principles of the MIMO radar, we first look at a simple example. In the simple antenna array, we have 1 Tx antenna and 4 Rx antennas, and the Rx antennas are separated by a distance of λ/2, as shown below.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/mimo0.png)

## X4 Evaluation Board

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_eval_block.png)

The top-level schematic of X4 evaluation board is shown in the above figure. The Rx/Tx port of X4 chip is differential output, and then the RF balun is applied to transform it to single Rx/Tx port.  Refer to the ADI [NCR2-123+](https://www.minicircuits.com/pdfs/NCR2-123+.pdf) for full specifications.



![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_board.png)

- *Tx RF interface*: This interface connects to the Tx radio that allows RF signals to be transmitted at 7.29 GHz with 1.4 GHz bandwidth.
- *Rx RF interface*: This interface connects to the Rx radio that allows RF signals to be received at 7.29 GHz with 1.4 GHz bandwidth.
- *X4 control I/Os*: The header pins includes SPI pins, clock synchronization pins, trigger pins, etc. which are connected to FPGA.
- *Power*: The power jack is where the power supply that comes with power source.
- *GND*: The ground is used to connect to other developing boards in ground plane.

## X4 Chip Circuit Overview

X4 chip is a radar transceiver SoC for UWB applications.  The overview of X4 internal circuit is shown in the figure below. The figure also illustrates our hardware design rationale in the previous sections. This figure come from Page 12 of the X4 chip datasheet.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_circuit.jpg)



### Tx Data Path

The Tx data path of the X4 consists of a pulse generator that is capable of generating pulses (max rate of 60.75 MHz). The center frequency and bandwidth of the output pulse is designed in compliance with worldwide RF regulations.

### Rx Data Path

The Rx data path of the X4 includes a Low Noise Amplifier (LNA), a Digital-to-Analog Converter (DAC) for setting thresholds, 1536 parallel comparators/samplers and digital integrators, as well as an output memory buffer, which is accessible through the SPI interface. The Rx data path is closely integrated with the transmitter (Tx) and is designed for coherent integration of the received energy.

### Clock

There are two types of clocks in the X4 chip: internal and external clocks. We shall elaborate on these clocks in the following:

#### Internal Clock

The crystal oscillator (XOSC) is designed for providing the radar transceiver core with a stable and accurate reference clock signal.  In the figure below, we show a typical block schematic when the X4 is clocked using the XOSC.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_interClock.jpg)

#### External Clock

- In XOSC bypass mode the radar core is clocked by an external low frequency clock source. This mode can be useful if there is already a clock signal available on the application PCB. The external clock input pin supports single-ended inputs. In the figure below, we show a typical block schematic when the X4 is clocked using the XOSC bypass mode.

  ![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_exClock1.jpg)

- In Common PLL bypass mode the radar transceiver core is clocked by an external high frequency clock from the LVDS interface on pins IO5 and IO6 and the Common PLL is bypassed. This allows multiple radars to stay synchronized which is required for digital beamforming applications. In the figure below, we show a block diagram of the Common PLL in bypass mode.

  ![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/x4_exClock2.jpg)

For more information, please refer to X4 chip [datasheet](https://www.radartutorial.eu/19.kartei/13.labs/pubs/x4_datasheet_revE_restricted.pdf).

## RF Port

A 50 ohm SMA jack is employed to connect to each RF interface. This connector is a standard-thread and standard-polarity SMA jack.  

## Power Supply

All the boards requires an external 5V DC power supply. 

## Design Files

[*sch/X4_ArrayUnit.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_ArrayUnit.pdf): This  file is the schematic of the whole antenna array board.

[*sch/X4_TestingBoard.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_TestingBoard.pdf): This file describe the testing RF switch board for loopback evaluation.

[*sch/X4_DevBoard.pdf*](https://github.com/DeepWiSe888/Octopus/blob/main/hardware/sch/X4_DevBoard.pdf): This file is the X4 Evaluation Board
