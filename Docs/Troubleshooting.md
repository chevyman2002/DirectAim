# 🛠️ Advanced Troubleshooting & Common Fixes
## 1. The "9-Flash" SOS (IMU Connection Error)
If the Teensy LED flashes 9 times in a row, pauses, and repeats, the code is hanging because it cannot communicate with the BNO08x sensor.

Check Wiring: Ensure the Blue (SDA) and Yellow (SCL) wires are securely connected to Pins 18 and 19.

The White Wire: Ensure the RST pin of the IMU is connected to Teensy Pin 2. The firmware pulses this pin to "wake up" the sensor; without it, the sensor will fail to initialize 90% of the time.

I2C Address: If you are using a non-SparkFun board, the I2C address might be different. The code checks 0x4B and 0x4A by default.

## 2. "Mouse/Joystick Not Declared" Compilation Error
If the Arduino IDE throws errors during compilation stating that Mouse, Keyboard, or Joystick were not declared in this scope:

The Fix: This happens when the IDE defaults to "Serial" mode. Go to Tools > USB Type and select "Keyboard + Mouse + Joystick". This enables the HID libraries required for the gun to function.

## 3. The 15-Second "Nuclear" Reset
If your Teensy becomes completely unresponsive, won't show up as a COM port, or the "Soft Reset" fails:

The Fix:
1. Hold down the physical button on the Teensy board for exactly 15 seconds.
2. The LED will flash briefly. Release the button.
3. The Teensy will spend 30–60 seconds wiping its memory and restoring the factory "Blink" program. You can then re-upload the DirectAim firmware.

## 4. Teknoparrot / Emulator Mapping Issues
If your Action Buttons (Red/Green) aren't registering in certain arcade emulators:

Middle Click Bug: Many emulators do not recognize the Middle Mouse Button (Mouse 3). In v1.2+, the Green Button is mapped to the Keyboard '1' key and Joystick Button 3.

Recommendation: Map "Start" or "Coin" in your emulator to the '1' key or Joy 3.

Input Mode: Ensure your emulator is set to use Raw Input for the best experience with multiple guns.

## 5. Cursor Accuracy & DPI Scaling
If the cursor "teleports" to corners or doesn't reach the edges of the screen:

DPI Check: Right-click your desktop > Display Settings. Check your Scale (e.g., 150%).

Firmware Sync: Ensure the SCALE variable in your code matches this exactly (e.g., 1.5).

FOV Tuning: If the cursor is perfectly centered but stops short of the edges, decrease the FOV_X or FOV_Y values in the code to stretch the movement.

## 6. Axis Inversion
If the gun moves Left when you point Right, or Up when you point Down:

The Fix: You do not need to flip the sensor. In the loop() section of the code, look for the map() functions. Swap the last two numbers.

Example to flip X: Change RES_X, 0 to 0, RES_X.
