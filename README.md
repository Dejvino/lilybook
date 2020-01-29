
# LilyBook - ePaper Reader for ESP32

Ebook reader firmware designed for LilyGO T5 v2.4 (ESP32 with 2.9" eInk display).

## How to build

Configure your esp32 build environment as for [**esp-idf examples**](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/)

Clone the repository

`git clone https://github.com/Dejvino/lilybook.git`

Execute menuconfig and configure your Serial flash config and other settings. Included *sdkconfig.defaults* sets some defaults to be used.

`make menuconfig`

Make and flash the firmware.

`make all && make flash`

Optionally run a serial output monitor to see debug messages.

`make monitor`

Note: pres `CTRL + ]` to exit the monitor.

---

## Built Thanks To
* Display Driver: https://github.com/loboris/ESP32_ePaper_example
* SD Card Sample: https://github.com/espressif/esp-idf/tree/master/examples/storage/sd_card
* Pinout: https://github.com/Xinyuan-LilyGO/T5-Ink-Screen-Series
