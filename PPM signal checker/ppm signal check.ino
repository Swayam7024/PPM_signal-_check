#include <PulsePosition.h>
#include <Joystick.h>

// Define CLOCKS_PER_MICROSECOND manually if not available
#ifndef CLOCKS_PER_MICROSECOND
#define CLOCKS_PER_MICROSECOND (F_CPU / 1000000)
#endif

// Create a PulsePosition input object
PulsePositionInput ppm;

// Create a Joystick object
// Replace 0, 1, 2 with actual configuration values if needed
Joystick joystick(0, 1, 2);

void setup() {
  // Initialize the PulsePosition library (attach the PPM signal pin)
  ppm.begin(2);

  // No joystick.begin() method; initialize joystick settings here if needed
}

void loop() {
  // Read PPM values
  int channel1 = ppm.read(1);
  int channel2 = ppm.read(2);
  int channel3 = ppm.read(3);
  int channel4 = ppm.read(4);

  // Check if channels have valid values before mapping
  if (channel1 >= 0 && channel2 >= 0 && channel3 >= 0 && channel4 >= 0) {
    // Map PPM values to joystick ranges (example: 1000 to 2000 to 0 to 255)
    int joyX = map(channel1, 1000, 2000, 0, 255);
    int joyY = map(channel2, 1000, 2000, 0, 255);
    int joyZ = map(channel3, 1000, 2000, 0, 255);
    int joyR = map(channel4, 1000, 2000, 0, 255);

    // Set joystick axes
    joystick.printX("X", joyX, 0);
    joystick.printY("Y", joyY, 0);
    joystick.printZ("Z");
    joystick.print("X", "Y", "Z", joyX, joyY);
  }

  // Add a small delay for stability
  delay(10);
}
