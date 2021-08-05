# Octopus: A Practical and Versatile Wideband MIMO Sensing Platform
Radio frequency (RF) technologies have achieved a great success in data communication. In recent years, pervasive RF signals are further exploited for sensing; *RF sensing* has since attracted attentions from both academia and industry. Existing developments mainly employ commodity Wi-Fi hardware or rely on sophisticated SDR platforms. While promising in many aspects, there still remains a gap between lab prototypes and real-life deployments. On one hand, due to its narrow bandwidth and communication-oriented design, Wi-Fi sensing offers a coarse sensing granularity and its performance is very unstable in harsh real-world environments. On the other hand, SDR-based designs may hardly be adopted in practice due to its large size and high cost. To this end, we propose, design, and implement Octopus, a compact and flexible wideband MIMO sensing platform, built using commercial-grade low-power impulse radio. Octopus provides a standalone and fully programmable RF sensing solution; it allows for quick algorithm design and application development, and it specifically leverages the wideband radio to achieve a competent and robust performance in practice. Using Octopus allows us to investigate various aspects of *RF sensing* in realistic environments. Octopus supports various representative RF sensing applications including localization, vibration sensing, and human/object imaging.

# Requirements
STM32CubeIDE, STM32CubeMX, Intel Quartus II

# System Architecture

Octopus's system architecture is shown as below. The architecture has two major parts: the large-scale MIMO and digital processing module. The former employs multiple XeThru X4 Ultra Wide Band (UWB) radar SoC radar chips[4] and Analog Devices RF switches to transmit and receive RF signals. The latter includes a heterogeneous ARM-FPGA architecture for configuring and controlling the MIMO radio, as well as for accelerating digital processing. We use Intel FPGA which provides rich I/Os for controlling the hybrid array, and use IP cores to complement our application’s performance such as computing and interfacing. To run the applications in a standalone mode, we leverage an ARM-based CPU to run applications and communicate with FPGA via FMC interface. We also use an lightweight Directed Acyclic Graph (DAG)-based scheduler in C/C++ to deploy applications easily.

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/arch.png)

Below Figure shows more details about Antenna Board Controller (ABC). 

![Aaron Swartz](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/abc.png)

ABC includes three components: Hybrid Array Controller (HAC), Antenna Array Controller (AAC) and Multiple SPI Controller (MSC).  HAC is the key component to schedule antenna array and multiple X4 chips, and it also collects the RX data from MSC to construct packets with specific format. AAC utilizes multiple GPIOs to control RF switch to obtain a *time-division* antenna array.  MSC is used to communicate with X4 chips via SPI buses, and reconfigure X4 chips according to application requirements.  The following documentation describes where the different components belong to.

# Design Files & Codes

This repository includes some folders.

[hardware](https://github.com/DeepWiSe888/Octopus/tree/main/hardware): this folder contains the schematics of antenna array design. To be used together with source<br>
[source](https://github.com/DeepWiSe888/Octopus/tree/main/source): this folder contains the source code of ARM and FPGA.<br>
[source/arm](https://github.com/DeepWiSe888/Octopus/tree/main/source/arm): this subfolder contains a lightweight DAG-scheduler based on FreeRTOS.<br>
[source/lib](https://github.com/DeepWiSe888/Octopus/tree/main/source/lib): this subfolder contains basic DSP and MISC for ARM signal processing. More details of ARM code can be found therein.<br>
[source/fpga](https://github.com/DeepWiSe888/Octopus/tree/main/source/FPGA): this subfolder contains DSP and CNN for FPGA signal processing. More details of FPGA code can be found therein.<br>
[source/example](https://github.com/DeepWiSe888/Octopus/tree/main/source/example): this subfolder contains two examples of vital sign estimation and occupancy awareness.<br>


# Credits
This application uses open-source components. You can find the source code of their open source projects along with license information below. We acknowledge and are grateful to these developers for their contributions to open source.

[1]https://github.com/DodgeHo/VMD_cpp<br>
[2]https://eigen.tuxfamily.org/<br>
[3]https://github.com/ZFTurbo/Verilog-Generator-of-Neural-Net-Digit-Detector-for-FPGA<br>[4]https://novelda.com/ <br>

