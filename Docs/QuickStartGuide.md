# 🚀 DirectAim Quick Start Guide
This guide will get your DirectAim spatial gun flashed and calibrated in minutes.

## 1. Software Setup
Before plugging in your hardware, ensure your environment is ready:

Arduino IDE: Download here (v2.0+ recommended).

Teensy Support: Open the Boards Manager in Arduino IDE, search for Teensy, and install.

BNO08x Library: Open the Library Manager, search for SparkFun BNO08x, and install.

## 2. Prepare the Firmware
Open Firmware/DirectAim/DirectAim.ino in the Arduino IDE.

Configure your Screen: Update the NATIVE_X, NATIVE_Y, and SCALE variables to match your monitor.

Example: If you use a 4K TV at 150% scaling, set NATIVE_X = 3840, NATIVE_Y = 2160, and SCALE = 1.5.

Set USB Type: Go to Tools > USB Type and select "Keyboard + Mouse + Joystick". (This is mandatory for the code to compile).

## 3. Wiring Reference
If you are building your own or troubleshooting, here is the official DirectAim pinout:

|Component|Teensy Pin|HID Output|
|---|---|---|
|Trigger|0|Left Click / Joy 1|
|Recenter (Blue)|1|System Calibration|
|Action 1 (Red)|3|Right Click / Joy 2|
|Action 2 (Green)|4|Keyboard '1' / Joy 3|
|IMU Reset|2|Hardware Pulse (White Wire)|

## 4. Flashing and Calibration
Connect the Teensy to your PC via Micro-USB.

Click Upload in the Arduino IDE.

To Calibrate: Point the gun at the exact center of your screen and tap the Blue Button. Your aim is now locked.

To Reset/Re-flash: No need to open the shell! Hold the Trigger + Blue + Red + Green buttons for 5 seconds. The LED will flash, and the gun will enter programming mode.

## 5. Emulator Setup (Teknoparrot/MAME)
DirectAim is designed to be "plug and play."

Action 2 (Green): This is mapped to the '1' key. In most emulators, this will automatically act as the "Start" or "Coin" button.

Accuracy Tip: If the cursor doesn't reach the corners of your screen, slightly decrease the FOV_X or FOV_Y values in the firmware and re-upload.
