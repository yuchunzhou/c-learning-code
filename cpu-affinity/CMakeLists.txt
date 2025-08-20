cmake_minimum_required(VERSION 3.10)
project(cpu-affinity)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(cpu-affinity main.c log.c)