# PPM Signal Checker

A sketch that reads a PPM signal from an RC receiver and maps the first four channels to joystick axes.

![Platform](https://img.shields.io/badge/Platform-Teensy-5C8AAB)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/Status-Work%20in%20progress-orange)

## Overview

This project checks the PPM (Pulse Position Modulation) output of an RC receiver. It decodes the PPM stream with the `PulsePosition` library, reads channels 1–4, converts each from the standard 1000–2000 µs range to a 0–255 value and passes the result to a joystick object, so that the RC transmitter's sticks can be checked as joystick axes. The `PulsePosition` library targets Teensy boards.

## Features

- Decodes a combined PPM signal on a single input pin
- Reads the first four RC channels
- Validates the channel values before using them
- Maps each channel from 1000–2000 µs to 0–255
- Sends the mapped values to joystick X / Y / Z axes
- Short 10 ms loop delay

## Hardware

| Component | Purpose |
|---|---|
| Teensy board (supported by the `PulsePosition` library) | Decodes the PPM signal |
| RC transmitter + receiver with PPM output | Signal source |
| USB cable | Power, programming and joystick connection to a PC |

## Pin Connections

| Board Pin | Connected To |
|---|---|
| 2 | Receiver PPM output |
| GND | Receiver ground |
| 5V / VIN | Receiver power (check your receiver's voltage) |

## How It Works

1. `ppm.begin(2)` starts PPM decoding on pin 2.
2. Each loop reads channels 1–4 with `ppm.read()`.
3. If all four values are valid, each is mapped from 1000–2000 to 0–255.
4. The mapped values are written to the joystick axes.

## Getting Started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and the [Teensyduino](https://www.pjrc.com/teensy/td_download.html) board package.
2. Libraries used by the sketch:
   - `PulsePosition` – included with Teensyduino
   - `Joystick` – a joystick library providing the `Joystick` class used in the sketch
3. Open `PPM signal checker/ppm signal check.ino`. The Arduino IDE expects the sketch file and its folder to have the same name, so it may offer to move the file into a matching folder.
4. Select your Teensy board under **Tools → Board** and, if required, a USB type that includes Joystick under **Tools → USB Type**.
5. Click **Upload**.

> **Note:** the joystick calls in the sketch (`printX`, `printY`, `printZ`, `print`) do not match the standard Teensy `Joystick` API, so this part is still being worked on and may need adapting to your joystick library.

## Project Structure

```
PPM_signal-_check/
├── PPM signal checker/
│   └── ppm signal check.ino   # PPM decoding and joystick mapping sketch
├── LICENSE
└── README.md
```

## Future Improvements

- Update the joystick output to the standard Teensy USB Joystick API (`Joystick.X()`, `Joystick.Y()`, etc.)
- Print all channel values to the Serial Monitor for quick signal checking
- Support more than four channels and detect signal loss
- Use `ppm.available()` to read only when a new frame has arrived

## Author

**Swayam Khantwal** – [GitHub @Swayam7024](https://github.com/Swayam7024)

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
