# ESP32 Dev Module, ESP32-S3 Dev Module and SparkFun Thing Plus ESP32-C6

Set `<TOOLCHAIN>`, `<CHIP>` and `<BOOTLOADER_ADDR>` in the commands below for your board:

| Board                        | `<TOOLCHAIN>`      | `<CHIP>`  | `<BOOTLOADER_ADDR>` |
|------------------------------|--------------------|-----------|---------------------|
| ESP32 Dev Module             | `esp32`            | `esp32`   | `0x1000`            |
| ESP32-S3 Dev Module          | `esp32s3`          | `esp32s3` | `0x0`               |
| SparkFun Thing Plus ESP32-C6 | `thingplusesp32c6` | `esp32c6` | `0x0`               |

On boards with two USB ports (e.g. ESP32-S3 dev kits), connect the one labeled `UART` (or `COM`), not `USB`.

The ESP32-C6 needs `esp32:esp32` 3.x (tested with 3.3.12): use that version's `boot_app0.bin` in the commands below.
The Thing Plus ESP32-C6's USB-C port is the chip's own USB serial, so it shows up as `/dev/ttyACM0` on Linux and
`/dev/cu.usbmodem*` on macOS.

Hold down the `BOOT` button on the ESP32. Don't let go. Then run the commands below:

```sh
chmod 0777 /dev/ttyACM0

# Linux
python3 ~/.arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool.py --chip <CHIP> --port /dev/ttyUSB0 --baud 921600  --before default_reset --after hard_reset write_flash -e -z --flash_mode dio --flash_freq 80m --flash_size 4MB <BOOTLOADER_ADDR> build-fprime-automatic-<TOOLCHAIN>/arduino-cli-sketch/tmp8lcl76bw.ino.bootloader.bin 0x8000 build-fprime-automatic-<TOOLCHAIN>/arduino-cli-sketch/tmp8lcl76bw.ino.partitions.bin 0xe000 ~/.arduino15/packages/esp32/hardware/esp32/2.0.9/tools/partitions/boot_app0.bin 0x10000 build-artifacts/<TOOLCHAIN>/<YOUR_DEPLOYMENT>/bin/<YOUR_DEPLOYMENT>.elf.bin

# MacOS
~/Library/Arduino15/packages/esp32/tools/esptool_py/4.5.1/esptool --chip <CHIP> --port /dev/tty.usbmodem12345 --baud 921600  --before default_reset --after hard_reset write_flash -e -z --flash_mode dio --flash_freq 80m --flash_size 4MB <BOOTLOADER_ADDR> build-fprime-automatic-<TOOLCHAIN>/arduino-cli-sketch/tmp8lcl76bw.ino.bootloader.bin 0x8000 build-fprime-automatic-<TOOLCHAIN>/arduino-cli-sketch/tmp8lcl76bw.ino.partitions.bin 0xe000 ~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.9/tools/partitions/boot_app0.bin 0x10000 build-artifacts/<TOOLCHAIN>/<YOUR_DEPLOYMENT>/bin/<YOUR_DEPLOYMENT>.elf.bin
```
> Change `<YOUR_DEPLOYMENT>` to the name of your F Prime deployment (e.g. `LedBlinker_LedBlinkerDeployment`).
> 
> Change `/dev/ttyACM0` (`/dev/tty.usbmodem12345` for MacOS) to the correct serial device connected to the ESP32.
>
> Note: Change `tmp8lcl76bw` to the actual temporary file name located in `build-fprime-automatic-<TOOLCHAIN>/arduino-cli-sketch`

Once you see that data is being written (example shown below), you can let go of the button.

```
Writing at 0x00010000... (10 %)
Writing at 0x0001bbae... (20 %)
Writing at 0x0002425b... (30 %)
Writing at 0x000298e1... (40 %)
Writing at 0x0002eb67... (50 %)
Writing at 0x00033f5b... (60 %)
Writing at 0x0003ca7a... (70 %)
Writing at 0x000465db... (80 %)
Writing at 0x0004bc47... (90 %)
Writing at 0x00051160... (100 %)
```
