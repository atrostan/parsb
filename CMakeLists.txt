cmake_minimum_required(VERSION 3.16)
project(par_slashburn)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
  GIT_PROGRESS TRUE)

# set(BSIZE 512) set(BWIDTH 512)
add_compile_definitions(__TBB_DYNAMIC_LOAD_ENABLED=0)
FetchContent_MakeAvailable(fmt)
# set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -pedantic -fopenmp -DBSIZE=${BSIZE}
# -DBWIDTH=${BWIDTH}
# -DCMAKE_C_COMPILER=/opt/intel/oneapi/compiler/latest/linux/bin/icx
# -DCMAKE_CXX_COMPILER=/opt/intel/oneapi/compiler/latest/linux/bin/icpx
# -march=native -mcx16")
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pedantic -march=native -D_REENTRANT=1")
message(${CMAKE_CXX_COMPILER_ID})

FetchContent_Declare(
  spray
  GIT_REPOSITORY https://github.com/atrostan/spray.git
  GIT_TAG master)

FetchContent_GetProperties(spray)
if(NOT spray_POPULATED)
  FetchContent_Populate(spray)
endif()

message(spray_SOURCE_DIR="${spray_SOURCE_DIR}")

FetchContent_MakeAvailable(spray)

add_library(spray INTERFACE)
target_include_directories(spray INTERFACE ${spray_SOURCE_DIR}/include)

set(PARSB_HEADERS
    include/benchmark.h
    include/bitmap.h
    include/builder.h
    include/command_line.h
    include/generator.h
    include/graph.h
    include/platform_atomics.h
    include/pvector.h
    include/reader.h
    include/timer.h
    include/writer.h
    include/SlashBurn.h
    include/util.h)

set(IPS4O_USE_OPENMP "true")
set(ONEDPL_PAR_BACKEND "openmp")
set(IPS4O_DISABLE_PARALLEL OFF)
set(ips4o_SOURCE_DIR "./ips4o")
# set(absl_DIR "./install/abseil-cpp/") find_package(oneDPL REQUIRED)
add_subdirectory(${ips4o_SOURCE_DIR})
# find_package(absl REQUIRED)
find_package(TBB REQUIRED)

add_subdirectory(install/abseil-cpp)

add_executable(parsb main.cpp src/SlashBurn.cpp ${PARSB_HEADERS})

target_include_directories(parsb PRIVATE include/)

target_compile_definitions(parsb PUBLIC BSIZE=${BSIZE})
target_compile_definitions(parsb PUBLIC BWIDTH=${BWIDTH})

target_link_libraries(parsb absl::flat_hash_set absl::flat_hash_map fmt::fmt
                      ips4o spray)
