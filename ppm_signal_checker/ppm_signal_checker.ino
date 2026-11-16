/*
 * ------------------------------------------------------------------
 *  PPM Signal Checker - RC receiver PPM -> Serial Monitor + USB joystick
 * ------------------------------------------------------------------
 *  Board  : Teensy 3.x / 4.x (uses the PulsePosition library)
 *           Arduino IDE: Tools > USB Type > "Serial + Keyboard + Mouse + Joystick"
 *  Wiring : pin 6 -> receiver PPM output
 *           GND   -> receiver ground
 *           5V/VIN-> receiver power (check your receiver's voltage)
 *  Author : Swayam Khantwal (github.com/Swayam7024)
 *  License: MIT
 *
 *  Prints every decoded channel (in µs) to the Serial Monitor and
 *  sends channels 1-6 to the PC as joystick axes, so the transmitter
 *  can be checked in any game-controller test tool or simulator.
 * ------------------------------------------------------------------
 */

#include <PulsePosition.h>

// PPM input pin. It must be a PulsePosition-capable pin:
//   Teensy 4.x: 6, 9, 10, 11, 12, 13, 14, 15, 18, 19
//   Teensy 3.x: 5, 6, 9, 10, 20, 21, 22, 23
// Pin 6 works on both.
const int PPM_PIN = 6;

const int MAX_CHANNELS   = 8;     // channels printed to the Serial Monitor
const int PULSE_MIN      = 1000;  // µs
const int PULSE_MAX      = 2000;  // µs
const unsigned long SIGNAL_TIMEOUT_MS = 100;  // no frame for this long -> signal lost
const unsigned long PRINT_INTERVAL_MS = 100;  // Serial Monitor update rate

// Create a PulsePosition input object
PulsePositionInput ppm;

float channel[MAX_CHANNELS + 1];  // channel[1..8], in µs
int numChannels = 0;
unsigned long lastFrameTime = 0;
unsigned long lastPrintTime = 0;

// Convert a channel pulse (1000-2000 µs) to a joystick axis value (0-1023)
int toAxis(float pulse) {
  int value = map((int)pulse, PULSE_MIN, PULSE_MAX, 0, 1023);
  return constrain(value, 0, 1023);
}

void centreJoystick() {
  Joystick.X(512);
  Joystick.Y(512);
  Joystick.Z(512);
  Joystick.Zrotate(512);
  Joystick.sliderLeft(512);
  Joystick.sliderRight(512);
  Joystick.send_now();
}

void setup() {
  Serial.begin(115200);

  // Initialize the PulsePosition library (attach the PPM signal pin)
  ppm.begin(PPM_PIN);

  // Send all axes together with send_now() instead of one USB packet per axis
  Joystick.useManualSend(true);
  centreJoystick();
}

void loop() {
  // ppm.available() returns the number of channels when a new frame has arrived
  int n = ppm.available();
  if (n > 0) {
    numChannels = n;
    for (int i = 1; i <= MAX_CHANNELS; i++) {
      channel[i] = (i <= n) ? ppm.read(i) : 0;
    }
    lastFrameTime = millis();

    // Set joystick axes (only for channels that are present)
    if (n >= 1) Joystick.X(toAxis(channel[1]));
    if (n >= 2) Joystick.Y(toAxis(channel[2]));
    if (n >= 3) Joystick.Z(toAxis(channel[3]));
    if (n >= 4) Joystick.Zrotate(toAxis(channel[4]));
    if (n >= 5) Joystick.sliderLeft(toAxis(channel[5]));
    if (n >= 6) Joystick.sliderRight(toAxis(channel[6]));
    Joystick.send_now();
  }

  bool signalOk = (lastFrameTime != 0) && (millis() - lastFrameTime < SIGNAL_TIMEOUT_MS);

  // Signal checker output on the Serial Monitor
  if (millis() - lastPrintTime >= PRINT_INTERVAL_MS) {
    lastPrintTime = millis();
    if (!signalOk) {
      Serial.println("NO PPM SIGNAL");
      centreJoystick();
    } else {
      Serial.print(numChannels);
      Serial.print(" ch |");
      for (int i = 1; i <= MAX_CHANNELS && i <= numChannels; i++) {
        Serial.print(" CH");
        Serial.print(i);
        Serial.print(": ");
        Serial.print((int)channel[i]);
      }
      Serial.println();
    }
  }
}
