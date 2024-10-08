####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding files
# MOD_DEPS: (optional) module dependencies
#
####
include(utilities)
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriver.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriver.cpp"
)
starts_with(IS_TEENSY "${ARDUINO_FQBN}" "teensy")
starts_with(IS_ATMEGA "${ARDUINO_FQBN}" "MegaCore")

# Check the Linux build
if (NOT FPRIME_ARDUINO)
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriverLinux.cpp")
elseif (IS_TEENSY)
    target_use_arduino_libraries("IntervalTimer")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriverTeensy.cpp")
elseif (IS_ATMEGA)
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/../../../ATmega/vendor/libraries/TimerOne/TimerOne.cpp")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriverAvr.cpp")
else()
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/HardwareRateDriverBasic.cpp")
endif()
register_fprime_module()

