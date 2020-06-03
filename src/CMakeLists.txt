cmake_minimum_required(VERSION 3.16)
project(lightdelay)

aux_source_directory(. DIR_SRCS)
add_executable(lightdelay ${DIR_SRCS})