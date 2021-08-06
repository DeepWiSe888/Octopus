# Vital sign detecting example for  mac/linux platform

To help undestanding the radar data processing, and the dag scheduling system, we provide this PC version of vital sign detecion.

This example does not involve Arm or FPGA H/S system, you can run it on your host PC system, both mac osx or linux can work. or maybe windows, if you modify the thread functions as WinAPI.

This example contains several processing modules, as simulate radar data, signal filter, fourier transforms, and VMD. Some of these modules are working in parallel, collaborating throuth the dag scheduling system.

It is not our intention that all processing modules running on host computer, but you can run it up easily without any embedded hardware. This example just helping you to quickly understand radar data processing system.

This example is written in c/c++ language.



# HOW-TO-RUN

## Dependences

CMake (version 3.5 or above),  gcc and g++.

- ### Command Line Style

  `$vitalsign_example_osx_linux$ mkdir build`

  `$vitalsign_example_osx_linux$ cd build`

  `$vitalsign_example_osx_linux\build$ cmake ..`

  `$vitalsign_example_osx_linux$ make`

  

  now you can run that example:

  `$vitalsign_example_osx_linux$ ./vs`

  output likes these :

  ```
  create vital sign task flow...
  run vital sign tasks...
  task [findVitalBin] wait semaphore...
  task [findVitalBin] running...
  task [FIR] wait semaphore...
  task [FFT1D] wait semaphore...
  task [VMD] wait semaphore...
  task [getVital] wait semaphore...
  task [findVitalBin] done...
  task [VMD] running...
  task [FIR] running...
  before_fir = [0.001531+0.001016i 0.035824+0.035072i 0.068689+0.068415i 0.101233+0.100604i 0.130824+0.130588i ... ];
  after_fir = [0.839280+0.838769i 0.873256+0.872743i 0.907757+0.907247i 0.942518+0.942012i 0.977132+0.976638i ... ];
  task [FIR] done...
  task [FFT1D] running...
  before_fft = [0.839280+0.838769i 0.873256+0.872743i 0.907757+0.907247i 0.942518+0.942012i 0.977132+0.976638i  ];
  after_fft = [0.000000+0.000000i 0.000000+0.000000i 0.000000+0.000000i -24.204668+156.769165i -105.077080+62.257458i  ];
  task [FFT1D] done...
  task [VMD] done...
  task [getVital] running...
  rpm=11.718750, bpm=70.312500
  rpm=[0.327265 0.325350 0.322413 0.318348 0.312918 0.305737 0.296246 0.284117 0.269803  ];
  bpm=[0.013153 0.025524 0.039173 0.052883 0.065196 0.074490 0.079095 0.077908 0.071094  ];
  task [getVital] done...
  vital sign tasks done.
  
  ```

  You can copy data values for each step into MATLAB and plot it, this can help you understand radar data processing.

- ### Using IDE

  CMakeLists.txt canbe opened by c/c++ IDEs.

  