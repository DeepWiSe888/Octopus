# Octopus: A Practical and Versatile Wideband MIMO Sensing Platform
Radio frequency (RF) technologies have achieved a great success in data communication. In recent years, pervasive RF signals are further exploited for sensing; *RF sensing* has since attracted attentions from both academia and industry. Existing developments mainly employ commodity Wi-Fi hardware or rely on sophisticated SDR platforms. While promising in many aspects, there still remains a gap between lab prototypes and real-life deployments. On one hand, due to its narrow bandwidth and communication-oriented design, Wi-Fi sensing offers a coarse sensing granularity and its performance is very unstable in harsh real-world environments. On the other hand, SDR-based designs may hardly be adopted in practice due to its large size and high cost. To this end, we propose, design, and implement Octopus, a compact and flexible wideband MIMO sensing platform, built using commercial-grade low-power impulse radio. Octopus provides a standalone and fully programmable RF sensing solution; it allows for quick algorithm design and application development, and it specifically leverages the wideband radio to achieve a competent and robust performance in practice. Using Octopus allows us to investigate various aspects of *RF sensing* in realistic environments. Octopus supports various representative RF sensing applications including localization, vibration sensing, and human/object imaging.

# Requirements
STM32CubeIDE, STM32CubeMX, Intel Quartus II

# About Folders
hardware: The schematics of antenna array design.<br>
source: The source code of ARM and FPGA.<br>
source/arm: A lightweight DAG-scheduler based on FreeRTOS.<br>
source/lib: Basic DSP and MISC. for ARM signal process. More description of ARM code is located in this folder.<br>
source/FPGA: DSP and CNN for FPGA signal process. More description of FPGA code is located in this folder.<br>
source/example: Two examples for vital sign estimation and occupancy.<br>


# Reference Works
We learn and use some nice open source works in our project. The reference works are listed below. If you find your code or project are involved by us, please let us know. Thanks!

[1]https://github.com/DodgeHo/VMD_cpp<br>
[2]https://eigen.tuxfamily.org/<br>
[3]https://github.com/ZFTurbo/Verilog-Generator-of-Neural-Net-Digit-Detector-for-FPGA<br>

