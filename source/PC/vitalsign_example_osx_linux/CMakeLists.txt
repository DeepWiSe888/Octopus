cmake_minimum_required(VERSION 3.5)
project(osxdemo CXX C)


add_definitions(-D_LINUX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_STANDARD 99)

#SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_SYSTEM_PROCESSOR arm)

include_directories(. .. ../../example/vital_sign ../../arm/dag_scheduler ../../arm/lib ../../arm/lib/blas /usr/local/include)

link_directories(/usr/local/lib)

SET(dag_src
    ../../arm/dag_scheduler/captain.c
    ../../arm/dag_scheduler/task_sync.c)

SET(lib_src
    ../../arm/lib/blas/blas.c
    ../../arm/lib/blas_tasks.c
    ../../arm/lib/radar_data.c
    ../../arm/lib/blas/VMD.cpp
    ../../arm/lib/blas/linear_math.c
    ../../arm/lib/blas/filter.c)


add_executable(vs ./main.c ../../example/vital_sign/vital_sign.c ${dag_src} ${lib_src})

target_link_libraries(vs m pthread fftw3)



