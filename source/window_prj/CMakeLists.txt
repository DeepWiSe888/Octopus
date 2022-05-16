cmake_minimum_required(VERSION 3.5)
project(mimo-udp C)
set(CMAKE_C_STANDARD 99 )
set(CMAKE_EXE_LINKER_FLAGS -static)

set(x4driver_src
        x4driver/radar.c
        x4driver/radar_hal.c
        x4driver/x4driver.c
        x4driver/get_data.c
        )
set(gf_src
        gfradar/gf_radar.c
        )
set(main_src
        main/main.c
#        main/cmd_ctrl_fpga.c
        main/recvdata.c
        main/udp.c
        )

include_directories(.)
include_directories(x4driver)
include_directories(main)
include_directories(gfradar)

link_directories(lib)

add_executable(mimo-udp
        ${x4driver_src}
        ${gf_src}
        ${main_src}
        )

target_link_libraries(mimo-udp
        pthread
        -lm
        -fno-stack-protector
        )

if(WIN32)

    target_link_libraries(mimo-udp
            -lWs2_32
            cmd_win32_fpga
            )
else()
    target_link_libraries(mimo-udp
            cmd_linux_fpga
            )
endif()