####
# ATSAMD21J17D.cmake:
#
# Microchip ATSAMD21J17D (Cortex-M0+, 48 MHz, 128 KB flash, 16 KB SRAM) on the Arduino SAMD core. No Arduino board
# uses this chip, so this builds the Arduino Zero's core and pin map (variant) for it:
#
# - The core's CMSIS package has no J17D header. The J17A's has the same peripherals and memory sizes.
# - Pins follow the Arduino Zero: LED_BUILTIN is PA17, Serial is SERCOM5 (PB22 TX, PB23 RX), Serial1 is SERCOM0
#   (PA10 TX, PA11 RX). All of them exist on the 64-pin J package.
# - The clock is locked to a 32.768 kHz crystal on PA00/PA01, as on the Zero. Add -DCRYSTALLESS to build.extra_flags
#   for a board without one.
# - USB (SerialUSB) is built in, as the core doesn't build without it.
# - The linker script (support/linker/ATSAMD21J17D.ld) has no bootloader, so the program is flashed over SWD, and
#   reserves 2 KB of RAM for the stack.
####
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "arm")
set(CMAKE_CROSSCOMPILING 1)
set(FPRIME_PLATFORM "ArduinoFw")
set(FPRIME_USE_BAREMETAL_SCHEDULER ON)
set(ARDUINO_BUILD_PROPERTIES "build.extra_flags=-D__SAMD21J17A__ {build.usb_flags}")

set(ARDUINO_FQBN "arduino:samd:arduino_zero_edbg")
# Cortex-M0+ has no atomic instructions, and the core provides no atomic library calls
add_compile_options(-D_BOARD_ATSAMD21J17D -DUSE_BASIC_TIMER -DFPRIME_ARDUINO_SYNTHETIC_ATOMICS)
# Run the base arduino setup which should detect settings!
include("${CMAKE_CURRENT_LIST_DIR}/support/arduino-support.cmake")
# Link with the J17D's memory map instead of the Zero's linker script
string(REGEX REPLACE "-T[^ ]+\\.ld\"?" "-T\"${CMAKE_CURRENT_LIST_DIR}/support/linker/ATSAMD21J17D.ld\""
       CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")
