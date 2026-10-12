// Atomic library calls for Cortex-M0+ boards whose core doesn't provide them, from fprime-baremetal. Compiled into
// the patch library, which is linked at the end of every target, since F´ framework libraries call them.
//
// Enabled by FPRIME_ARDUINO_SYNTHETIC_ATOMICS. Don't enable it on a core that already provides these calls.
#if defined(FPRIME_ARDUINO_SYNTHETIC_ATOMICS)
#include <fprime-baremetal/Os/StdAtomic/libatomic_armv6m.c>
#endif
