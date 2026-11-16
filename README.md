<div align="center">

# PPM Signal Checker

**Check your RC receiver's PPM output on a Teensy – live channel values on the Serial Monitor and as a USB joystick on your PC.**

![Teensy](https://img.shields.io/badge/Teensy-3.x%20%2F%204.x-5C8AAB?style=for-the-badge)
![C++](https://img.shields.io/badge/C%2B%2B-Arduino-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![USB](https://img.shields.io/badge/USB-HID%20Joystick-2ECC71?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

[Features](#features) · [Hardware](#hardware) · [Getting Started](#getting-started)

</div>

---

## Overview

This project checks the PPM (Pulse Position Modulation) output of an RC receiver. A Teensy decodes the PPM stream with the `PulsePosition` library, prints every channel value (in µs) to the Serial Monitor, and sends the first six channels to the PC as USB joystick axes using the built-in Teensy `Joystick` API. That way the transmitter sticks and switches can be checked in the Serial Monitor, in any game-controller test tool, or used directly in an RC simulator.

## Features

- Decodes a combined PPM signal on a single input pin
- **Signal checker:** prints up to 8 channels to the Serial Monitor (115200 baud) 10 times per second
- **Signal-loss detection:** prints `NO PPM SIGNAL` and centres the joystick if no frame arrives for 100 ms
- Reads only when a new frame has arrived (`ppm.available()`)
- **USB joystick output** (Teensy `Joystick` API), each channel mapped from 1000–2000 µs to 0–1023:

| PPM channel | Joystick axis |
|---|---|
| CH1 | `Joystick.X()` |
| CH2 | `Joystick.Y()` |
| CH3 | `Joystick.Z()` |
| CH4 | `Joystick.Zrotate()` |
| CH5 | `Joystick.sliderLeft()` |
| CH6 | `Joystick.sliderRight()` |

Example Serial Monitor output:

```
8 ch | CH1: 1500 CH2: 1500 CH3: 1012 CH4: 1500 CH5: 1000 CH6: 2000 CH7: 1500 CH8: 1500
```

## Hardware

| Component | Purpose |
|---|---|
| Teensy 3.x or 4.x board | Decodes the PPM signal, USB joystick |
| RC transmitter + receiver with PPM output | Signal source |
| USB cable | Power, programming, Serial Monitor and joystick connection to a PC |

## Pin Connections

| Teensy Pin | Connected To |
|---|---|
| 6 | Receiver PPM output |
| GND | Receiver ground |
| 5V / VIN | Receiver power (check your receiver's voltage) |

> The PPM input must be on a pin supported by `PulsePositionInput`. Pin 6 works on both Teensy 3.x and 4.x. Other options – Teensy 4.x: 9, 10, 11, 12, 13, 14, 15, 18, 19 · Teensy 3.x: 5, 9, 10, 20, 21, 22, 23. Change `PPM_PIN` in the sketch if you use another pin.

## How It Works

1. `ppm.begin(PPM_PIN)` starts PPM decoding.
2. Each time `ppm.available()` reports a new frame, all channels are read with `ppm.read()`.
3. Channels 1–6 are mapped from 1000–2000 µs to 0–1023 and sent together with `Joystick.send_now()`.
4. Every 100 ms the channel values (or `NO PPM SIGNAL`) are printed to the Serial Monitor.

## Getting Started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and the [Teensyduino](https://www.pjrc.com/teensy/td_download.html) board package. `PulsePosition` and the USB `Joystick` support are included with Teensyduino – no extra libraries needed.
2. Open `ppm_signal_checker/ppm_signal_checker.ino`.
3. Select your Teensy board under **Tools → Board**.
4. Set **Tools → USB Type → "Serial + Keyboard + Mouse + Joystick"**. Without it the sketch fails with `'Joystick' was not declared`.
5. Click **Upload**.
6. Open the Serial Monitor at **115200 baud** to see the channel values, and/or open your PC's game-controller settings to see the joystick axes move.

PlatformIO users: add `build_flags = -D USB_SERIAL_HID` to `platformio.ini` for the same USB type.

## Project Structure

```
PPM_signal-_check/
├── ppm_signal_checker/
│   └── ppm_signal_checker.ino   # PPM decoding, serial checker and USB joystick sketch
├── LICENSE
└── README.md
```

## Future Improvements

- Map channels 7–8 to joystick buttons
- Show a frame-rate counter and channel min/max values for receiver diagnostics
- SBUS / iBUS input support

## Author

**Swayam Khantwal** – [GitHub @Swayam7024](https://github.com/Swayam7024)

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
