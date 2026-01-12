🛠️ Troubleshooting

1. The Sensor Won't Initialize (LED blinking rapidly)
Check the White Wire: Ensure the BNO08x RST pin is connected to Teensy Pin 2. DirectAim pulses this pin on startup to clear I2C errors. Without this, the sensor will fail to start 90% of the time.

Wiring Check: Double-check that SDA is on Pin 18 and SCL is on Pin 19. If using the Qwiic kit, ensure no stray solder bridges are touching the tiny pins.

2. The Cursor is "Down and to the Right" of Aim
DPI Scaling Mismatch: This happens when Windows Scaling (150%, 200%, etc.) is active.

Right-click your desktop > Display Settings > Scale.

Ensure the SCALE variable in the firmware matches this value (e.g., 1.5 for 150%).

Resolution Check: Ensure NATIVE_X and NATIVE_Y in the code match your actual screen resolution.

3. One or Both Axes are Inverted
If your gun moves left when you point right, or down when you point up:

Go to the map() section in the loop.

To flip X: Swap RES_X, 0 to 0, RES_X.

To flip Y: Swap RES_Y, 0 to 0, RES_Y.

4. The Cursor Stops Before the Edge of the Screen
FOV Adjustment: If your gun is pointing at the physical edge of the monitor but the cursor stopped 2 inches early, your FOV is too high.

Decrease FOV_X or FOV_Y in the code to "stretch" the cursor movement further.

Increase FOV if the cursor hits the edge too quickly.

5. Teensy Not Found / Can't Upload
USB Type: In the Arduino IDE, you must set Tools > USB Type to "Keyboard + Mouse + Joystick".

Cable Check: Ensure you are using a Data-Sync Micro-USB cable, not a "Charging Only" cable.