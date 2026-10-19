####
# thingplusesp32c6.cmake:
#
# SparkFun Thing Plus ESP32-C6 support.
####
cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -std=gnu++2a) # Force C++20 standard

set(CMAKE_SYSTEM_NAME "Generic")
# RISC-V, but like the other ESP32 targets it has a full C++ standard library, so use the "arm" platform types
set(CMAKE_SYSTEM_PROCESSOR "arm")
set(CMAKE_CROSSCOMPILING 1)
set(FPRIME_PLATFORM "ArduinoFw")
set(FPRIME_USE_BAREMETAL_SCHEDULER ON)

# Prevent test program compiling
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(ARDUINO_FQBN "esp32:esp32:sparkfun_esp32c6_thing_plus")
add_compile_options(-D_BOARD_THINGPLUSESP32C6 -DUSE_BASIC_TIMER)
# Run the base arduino setup which should detect settings!
include("${CMAKE_CURRENT_LIST_DIR}/support/arduino-support.cmake")
