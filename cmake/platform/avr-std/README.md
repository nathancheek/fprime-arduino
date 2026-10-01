# avr-std

Minimal C++ standard library headers for avr-gcc, which ships without libstdc++. These supplement the shims in
`../basic/Platform` and are searched first by AVR toolchains (see `../../toolchain/ATmega128.cmake`). They are kept out
of `basic/Platform` because that directory is also used by non-AVR boards whose toolchains provide the real headers.
If an AVR build fails with `fatal error: <header>: No such file or directory`, add the missing header here.
