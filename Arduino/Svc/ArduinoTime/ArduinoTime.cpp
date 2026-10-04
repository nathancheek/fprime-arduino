/*
 * ArduinoTime.cpp:
 *
 * An implementation of ArduinoTime used on Arduino so that standard system components can be run as
 * expected. The time format is (U32 seconds, U32 microseconds): the Arduino raw time (seconds and
 * microseconds since boot) plus an offset set by SET_TIME. Both parts come from the raw time, so they
 * always agree. Raw time needs a call at least every 71.6 minutes (see Arduino/Os/RawTime.cpp); this
 * component's own getTime calls count.
 *
 * @author lestarch
 */
#include <Arduino/Svc/ArduinoTime/ArduinoTime.hpp>
#include <Arduino/Os/RawTime.hpp>
#include <TimeLib.h>
#include <Arduino/config/FprimeArduino.hpp>

namespace Arduino {

constexpr U32 MICROSECONDS_PER_SECOND = 1000000;

ArduinoTime::ArduinoTime(const char* name)
    : ArduinoTimeComponentBase(name), m_offsetSeconds(0), m_offsetMicroseconds(0) {}
ArduinoTime::~ArduinoTime() {}

//! The Arduino raw time: seconds and microseconds since boot
static Os::Arduino::ArduinoRawTimeHandle sinceBoot() {
    Os::RawTime raw;
    (void)raw.now();
    return *static_cast<Os::Arduino::ArduinoRawTimeHandle*>(raw.getHandle());
}

void ArduinoTime::getTime_handler(FwIndexType portNum, /*!< The port number*/
                                      Fw::Time& time           /*!< The time to set */
) {
    const Os::Arduino::ArduinoRawTimeHandle since = sinceBoot();
    U32 seconds = since.m_seconds + this->m_offsetSeconds;
    U32 microseconds = since.m_micros + this->m_offsetMicroseconds;
    // Each part was under one second, so the sum is under two seconds: one carry is enough
    if (microseconds >= MICROSECONDS_PER_SECOND) {
        microseconds -= MICROSECONDS_PER_SECOND;
        seconds++;
    }
    TimeBase base = (::timeStatus() == timeStatus_t::timeNeedsSync) ? TimeBase::TB_PROC_TIME : TimeBase::TB_WORKSTATION_TIME;
    time.set(base, seconds, microseconds);
}

void ArduinoTime ::setTime(U32 year, U8 month, U8 day, U8 hour, U8 minute, U8 second) {
    year = (year > std::numeric_limits<int>::max()) ? std::numeric_limits<int>::max() : year;
    Fw::Time before_set = this->getTime();
    // TimeLib converts the date (and sets its own clock, which timeStatus() reports on)
    ::setTime(hour, minute, second, day, month, year);
    const time_t target = ::now();
    // Look for seconds overflow
    FW_ASSERT(target < std::numeric_limits<U32>::max());
    // Choose the offset so that the time is exactly the requested second now
    const Os::Arduino::ArduinoRawTimeHandle since = sinceBoot();
    if (since.m_micros == 0) {
        this->m_offsetSeconds = static_cast<U32>(target) - since.m_seconds;
        this->m_offsetMicroseconds = 0;
    } else {
        this->m_offsetSeconds = static_cast<U32>(target) - since.m_seconds - 1;
        this->m_offsetMicroseconds = MICROSECONDS_PER_SECOND - since.m_micros;
    }
    Fw::Time after_set = this->getTime();
    this->log_ACTIVITY_HI_TimeUpdate(before_set.getSeconds(), before_set.getUSeconds(), before_set.getTimeBase(),
                                  after_set.getSeconds(), after_set.getUSeconds(), after_set.getTimeBase());
}

void ArduinoTime ::setTime_handler(FwIndexType portNum, U32 year, U8 month, U8 day, U8 hour, U8 minute, U8 second) {
    this->setTime(year, month, day, hour, minute, second);
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void ArduinoTime ::SET_TIME_cmdHandler(FwOpcodeType opCode,
                                       U32 cmdSeq,
                                       U32 year,
                                       U8 month,
                                       U8 day,
                                       U8 hour,
                                       U8 minute,
                                       U8 second) {
    this->setTime(year, month, day, hour, minute, second);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
}  // namespace Arduino
