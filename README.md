# Octopus: A Practical and Versatile Wideband MIMO Sensing Platform
Radio frequency (RF) technologies have achieved a great success in data communication. In recent years, pervasive RF signals are further exploited for sensing; *RF sensing* has since attracted attentions from both academia and industry. Existing developments mainly employ commodity Wi-Fi hardware or rely on sophisticated SDR platforms. While promising in many aspects, there still remains a gap between lab prototypes and real-life deployments. On one hand, due to its narrow bandwidth and communication-oriented design, Wi-Fi sensing offers a coarse sensing granularity and its performance is very unstable in harsh real-world environments. On the other hand, SDR-based designs may hardly be adopted in practice due to its large size and high cost. To this end, we propose, design, and implement Octopus, a compact and flexible wideband MIMO sensing platform, built using commercial-grade low-power impulse radio. Octopus provides a standalone and fully programmable RF sensing solution; it allows for quick algorithm design and application development, and it specifically leverages the wideband radio to achieve a competent and robust performance in practice. Using Octopus allows us to investigate various aspects of *RF sensing* in realistic environments. Octopus supports various representative RF sensing applications including localization, vibration sensing, and human/object imaging.

# Requirements
STM32CubeIDE, STM32CubeMX, Intel Quartus II

# System Architecture

Octopus's system architecture is shown as below. The architecture has two major parts: a large-scale MIMO and a digital processing module. The former employs multiple XeThru X4 Ultra Wide Band (UWB) radar SoC chips[4] and Analog Devices RF switches to transmit and receive RF signals. The latter includes a heterogeneous ARM-FPGA architecture for configuring and controlling the MIMO radio, as well as for accelerating digital processing. We use Intel FPGA which provides rich I/Os for controlling the hybrid array, and use IP cores to complement our application’s performance such as computing and interfacing. To run the applications in a standalone mode, we leverage an ARM-based CPU to run applications and communicate with FPGA via FMC interface. We also use an lightweight Directed Acyclic Graph (DAG)-based scheduler in C/C++ to deploy applications easily.

![arch](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/arch.png)

Below Figure shows more details about Antenna Board Controller (ABC). 

![abc](https://raw.githubusercontent.com/DeepWiSe888/Octopus/main/image/abc.png)

ABC includes three components: Hybrid Array Controller (HAC), Antenna Array Controller (AAC) and Multiple SPI Controller (MSC).  HAC is the key component to schedule antenna array and multiple X4 chips, and it also collects the RX data from MSC to construct packets with specific format. AAC utilizes multiple GPIOs to control RF switch to obtain a *time-division* antenna array.  MSC is used to communicate with X4 chips via SPI buses, and reconfigure X4 chips according to application requirements.  The following documentation describes where the different components belong to.

# Design Files & Codes

This repository includes some folders.

[hardware](https://github.com/DeepWiSe888/Octopus/tree/main/hardware): this folder contains the schematics of antenna array design. To be used together with source.<br>
[source](https://github.com/DeepWiSe888/Octopus/tree/main/source): this folder contains the source code of ARM and FPGA.<br>
[source/arm](https://github.com/DeepWiSe888/Octopus/tree/main/source/arm): this subfolder contains a lightweight DAG-scheduler based on FreeRTOS.<br>
[source/arm/lib](https://github.com/DeepWiSe888/Octopus/tree/main/source/arm/lib): this subfolder contains basic DSP and MISC for ARM signal processing. More details of ARM code can be found in it.<br>
[source/fpga](https://github.com/DeepWiSe888/Octopus/tree/main/source/fpga): this subfolder contains DSP and CNN for FPGA signal processing. More details of FPGA code can be found in it.<br>
[source/example](https://github.com/DeepWiSe888/Octopus/tree/main/source/example): this subfolder contains two sensing examples of vital sign estimation and occupancy awareness.<br>

# Reference Our Project
Any use of the Software which results in an academic publication or other publication which includes a bibliography must include citations to the Octopus project.  The project should be cited as follows:

[1] Zhe Chen, Tianyue Zheng, and Jun Luo. 2021. Octopus: A Practical and Versatile Wideband MIMO Sensing Platform. In Proc. of the 27th ACM MobiCom. 1–14.

And the bibtex is:

```
@inproceedings{chen2021octopus,
  title={{Octopus: A Practical and Versatile Wideband MIMO Sensing Platform}},
  author={Chen, Zhe and Zheng, Tianyue and Luo, Jun},
  booktitle={Proc. of the 27th ACM MobiCom},
  pages={1--14},
  year={2021}
}
```

# Applications of Octopus

More than the platform itself, we are excited to announce our related works using Octopus to advance the state of the art. We are proud to list the following published papers from our group using Octopus.

```
@article{zheng2021enhancing,
	title={Enhancing RF Sensing with Deep Learning: A Layered Approach},
	author={Zheng, Tianyue and Chen, Zhe and Ding, Shuya and Luo, Jun},
	journal={IEEE Communications Magazine},
	volume={59},
	number={2},
	pages={70--76},
	year={2021}
}
```


```
@inproceedings{zheng2020v2ifi,
	title={V$^2$iFi: in-Vehicle Vital Sign Monitoring via Compact RF Sensing},
	author={Zheng, Tianyue and Chen, Zhe and Cai, Chao and Luo, Jun and Zhang, Xu},
	booktitle={Proc. of the 22nd ACM UbiComp},
	pages={70:1--27},
	year = {2020}
}
```

```
@inproceedings{ding2020rfnet,
	author = {Ding, Shuya and Chen, Zhe and Zheng, Tianyue and Luo, Jun},
	title = {RF-Net: A Unified Meta-Learning Framework for RF-Enabled One-Shot Human Activity Recognition},
	year = {2020},
	booktitle = {Proc. of the 18th ACM SenSys},
	pages = {517–530},
	numpages = {14},
}
```

```
@inproceedings{movifi,
	author={Zhe Chen and Tianyue Zheng and Chao Cai and Jun Luo},
	title = {{MoVi-Fi: Motion-robust Vital Signs Waveform Recovery via Deep Interpreted RF Sensing}},
	year = {2021},
	pages={1--14},
	booktitle = {Proc. of the 27th ACM MobiCom}
}
```


```
@inproceedings{zheng2021siwa,
	author={Tianyue Zheng and Zhe Chen and Jun Luo and Lin Ke and Chaoyang Zhao and Yaowen Yang},
	title = {{SiWa: See into Walls via Deep UWB Radar}},
	year = {2021},
	pages={1--14},
	booktitle = {Proc. of the 27th ACM MobiCom}
}  
```


# Credits
This application uses open-source components. You can find the source code of their open source projects along with license information below. We acknowledge and are grateful to these developers for their contributions to open source.

[1]https://github.com/DodgeHo/VMD_cpp<br>
[2]https://eigen.tuxfamily.org/<br>
[3]https://github.com/ZFTurbo/Verilog-Generator-of-Neural-Net-Digit-Detector-for-FPGA<br>[4]https://novelda.com/ <br>[5]https://taskflow.github.io/<br>

