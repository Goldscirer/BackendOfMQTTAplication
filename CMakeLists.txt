cmake_minimum_required(VERSION 3.7)
project(BackendOfMQTTAplication C)

set(CMAKE_C_STANDARD 99)

add_executable(BackendOfMQTTAplication main.c callback.h callback.c)

target_link_libraries(BackendOfMQTTAplication paho-mqtt3c)