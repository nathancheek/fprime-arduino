####
# ATSAMD21J17D.cmake:
#
# Microchip ATSAMD21J17D (Cortex-M0+, 48 MHz, 128 KB flash, 16 KB SRAM) on the Fab SAM core's "Generic x21J" board,
# MattairTech's generic 64-pin SAMD21J variant. Its SAMD21J17A option has the J17D's memory sizes and peripherals.
# The board options in the FQBN:
#
# - bootloader=0kb: no bootloader, so the program starts at address 0 and is flashed over SWD
# - clock=crystal_32k: the clock is locked to a 32.768 kHz crystal on PA00/PA01. clock=internal runs without one.
# - usb=none: USB is left out to save RAM and flash
# - serial=three_uart: Serial1 (SERCOM1, TX PA18, RX PA31), Serial2 (SERCOM0, TX PA06, RX PA07) and Serial3
#   (SERCOM4, TX PB08, RX PB09). PA31 is also SWDIO, so starting Serial1 stops SWD until the next reset.
#
# LED_BUILTIN is PB02.
####
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "arm")
set(CMAKE_CROSSCOMPILING 1)
set(FPRIME_PLATFORM "ArduinoFw")
set(FPRIME_USE_BAREMETAL_SCHEDULER ON)
# The core assembles .S files without -mcpu and -mthumb, so they don't link with the rest. With no bootloader file,
# arduino-cli doesn't try to merge the program with one (the core has none for this chip).
set(ARDUINO_BUILD_PROPERTIES
    "compiler.S.flags=-mcpu={build.mcu} -mthumb -c -g -x assembler-with-cpp -MMD"
    "bootloader.file="
)

set(ARDUINO_FQBN "Fab_SAM_Arduino:samd:x21j:cpu=samd21j17a,bootloader=0kb,clock=crystal_32k,usb=none,serial=three_uart")
# Cortex-M0+ has no atomic instructions, and the core provides no atomic library calls
add_compile_options(-D_BOARD_ATSAMD21J17D -DUSE_BASIC_TIMER -DFPRIME_ARDUINO_SYNTHETIC_ATOMICS)
# The core has no sized operator delete (C++14), which virtual destructors call by default
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-sized-deallocation>)
# Run the base arduino setup which should detect settings!
include("${CMAKE_CURRENT_LIST_DIR}/support/arduino-support.cmake")
