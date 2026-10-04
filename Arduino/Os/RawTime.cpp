// ======================================================================
// \title Arduino/Os/RawTime.cpp
// \brief Arduino implementation for Os::RawTime, based on micros()
// ======================================================================
#include "Arduino/Os/RawTime.hpp"
#include "Arduino/config/FprimeArduino.hpp"
namespace Os {
namespace Arduino {

namespace {
constexpr U32 MICROSECONDS_PER_SECOND = 1000000;

// Seconds and microseconds since boot, both counted from micros(). Seconds can't come from millis(): the
// two are read at different instants, and micros() wraps every 2^32 us (about 71.6 minutes), which isn't
// a whole number of seconds, so after a wrap micros() % 1000000 no longer lines up with millis() / 1000.
//
// Each now() adds micros() - (micros() at the previous call). U32 subtraction is modulo 2^32, so this is
// right even if micros() wrapped in between.
//
// Trade-off: now() must be called at least once every 71.6 minutes. Each wrap missed is silently lost,
// putting raw time 71.6 minutes behind. F Prime deployments usually call it far more often (every rate
// group cycle, for example).
//
// Shared and unguarded: call now() from one thread, not from interrupt handlers.
U32 s_lastMicros = 0;
U32 s_seconds = 0;
U32 s_subsecondMicros = 0;
}  // namespace

//! \brief check if a is newer than b
bool isNewer(const ArduinoRawTimeHandle& a, const ArduinoRawTimeHandle& b) {
    return ((a.m_seconds > b.m_seconds) ||
           ((a.m_seconds == b.m_seconds) && (a.m_micros >= b.m_micros)));
}

RawTimeHandle* ArduinoRawTime::getHandle() {
    return &this->m_handle;
}

RawTime::Status ArduinoRawTime::now() {
    const U32 microsNow = ::micros();
    const U32 elapsed = microsNow - s_lastMicros;  // modulo 2^32, see above
    s_lastMicros = microsNow;

    if (elapsed < MICROSECONDS_PER_SECOND) {
        // Usual case: no division needed
        s_subsecondMicros += elapsed;
    } else {
        s_seconds += elapsed / MICROSECONDS_PER_SECOND;
        s_subsecondMicros += elapsed % MICROSECONDS_PER_SECOND;
    }
    // Each part added was under one second, so the sum is under two seconds: one carry is enough
    if (s_subsecondMicros >= MICROSECONDS_PER_SECOND) {
        s_subsecondMicros -= MICROSECONDS_PER_SECOND;
        s_seconds++;
    }

    this->m_handle.m_seconds = s_seconds;
    this->m_handle.m_micros = s_subsecondMicros;
    return Status::OP_OK;
}

RawTime::Status ArduinoRawTime::getTimeInterval(const Os::RawTime& other, Fw::TimeInterval& interval) const {
    interval.set(0, 0);
    const ArduinoRawTimeHandle& my_handle = this->m_handle;
    const ArduinoRawTimeHandle& other_handle = static_cast<const ArduinoRawTimeHandle&>(*const_cast<Os::RawTime&>(other).getHandle());

    const ArduinoRawTimeHandle& newer = isNewer(my_handle, other_handle) ? my_handle : other_handle;
    const ArduinoRawTimeHandle& older = isNewer(my_handle, other_handle) ? other_handle : my_handle;

    if (newer.m_micros < older.m_micros) {
        interval.set(newer.m_seconds - older.m_seconds - 1, 1000000 + newer.m_micros - older.m_micros);
    } else {
        interval.set(newer.m_seconds - older.m_seconds, newer.m_micros - older.m_micros);
    }

    return Status::OP_OK;
}

Fw::SerializeStatus ArduinoRawTime::serializeTo(Fw::SerialBufferBase& buffer, Fw::Endianness mode) const {
    Fw::SerializeStatus status = Fw::SerializeStatus::FW_SERIALIZE_OK;
    status = buffer.serializeFrom(this->m_handle.m_seconds, mode);
    if (status == Fw::FW_SERIALIZE_OK) {
        status = buffer.serializeFrom(this->m_handle.m_micros, mode);
    }
    return status;
}

Fw::SerializeStatus ArduinoRawTime::deserializeFrom(Fw::SerialBufferBase& buffer, Fw::Endianness mode) {
    Fw::SerializeStatus status = Fw::SerializeStatus::FW_SERIALIZE_OK;
    status = buffer.deserializeTo(this->m_handle.m_seconds, mode);
    if (status == Fw::FW_SERIALIZE_OK) {
        status = buffer.deserializeTo(this->m_handle.m_micros, mode);
    }
    return status;
}
}  // namespace Arduino
}  // namespace Os
