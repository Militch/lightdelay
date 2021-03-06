cmake_minimum_required(VERSION 3.16)
project(lightdelay)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(src DIR_SRCS)
aux_source_directory(src/thread_pool DIR_SRCS_TP)
aux_source_directory(src/win32 DIR_SRCS_WIN32)
aux_source_directory(src/ini DIR_SRCS_INI)
aux_source_directory(src/ini/cpp DIR_SRCS_INI_CPP)
aux_source_directory(src/util DIR_SRCS_UTIL)
add_subdirectory(3rdparty)
include_directories(include)
find_package(hiredis)
find_package(RdKafka)
if (NOT hiredis_FOUND)
    message(FATAL_ERROR "hiredis Not Found!!!")
endif ()
if (NOT RdKafka_FOUND)
    message(FATAL_ERROR "RdKafka Not Found!!!")
endif ()
set(RdKafka_INCLUDE_DIRS "${RdKafka_DIR}/../../../include")
include_directories(${hiredis_INCLUDE_DIRS})
include_directories(${RdKafka_INCLUDE_DIRS})
link_directories(${RdKafka_DIR}/../../../bin)
add_executable(
        lightdelay
        ${DIR_SRCS}
        ${DIR_SRCS_TP}
        ${DIR_SRCS_WIN32}
        ${DIR_SRCS_INI}
        ${DIR_SRCS_INI_CPP}
        ${DIR_SRCS_UTIL}
)

target_link_libraries(
    lightdelay
    spdlog::spdlog
    ${hiredis_LIBRARIES}
    rdkafka++
)