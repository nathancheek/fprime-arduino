#include <Fw/Types/BasicTypes.hpp>
#include <Arduino/Drv/HardwareRateDriver/HardwareRateDriver.hpp>
#include <Arduino.h>
#include <ATmega/vendor/libraries/TimerOne/TimerOne.h>

namespace Arduino {

// Ticks counted by the Timer1 interrupt and not yet dispatched by cycle(). In .noinit and not static so a
// reset handler can read the count from before a reset (a high count means the main loop had stalled).
volatile U8 s_pendingTicks __attribute__((section(".noinit")));

void HardwareRateDriver::start() {
    s_pendingTicks = 0;
    Timer1.initialize(m_interval * 1000);
    Timer1.attachInterrupt(HardwareRateDriver::s_timerISR);
    Timer1.start();
}

void HardwareRateDriver::stop() {
    Timer1.stop();
    Timer1.detachInterrupt();
}

// Called from the main loop (USE_BASIC_TIMER), so rate groups run outside of interrupt context
void HardwareRateDriver::cycle() {
    noInterrupts();
    const bool pending = (s_pendingTicks > 0);
    if (pending) {
        s_pendingTicks--;
    }
    interrupts();
    if (pending) {
        s_timer(s_driver);
    }
}

void HardwareRateDriver::s_timerISR() {
    if (s_pendingTicks < 0xFF) {
        s_pendingTicks++;
    }
}

};
