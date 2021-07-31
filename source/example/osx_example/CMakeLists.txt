cmake_minimum_required(VERSION 3.5)
project(osxdemo C)


add_definitions(-D_LINUX)

set(CMAKE_C_STANDARD 99)

#SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_SYSTEM_PROCESSOR arm)

include_directories(. .. ../vital_sign ../../arm/dag_scheduler ../../arm/lib)

SET(dag_src
    ../../arm/dag_scheduler/captain.c
    ../../arm/dag_scheduler/task_sync.c)

SET(lib_src
    ../../arm/lib/blas.c
    ../../arm/lib/blas_tasks.c
    ../../arm/lib/radar_data.c
    ../../arm/lib/VMD.cpp)


add_executable(vs ./main.c ../vital_sign/vital_sign.c ${dag_src} ${lib_src})

target_link_libraries(vs /usr/local/lib/libfftw3.a m pthread)



