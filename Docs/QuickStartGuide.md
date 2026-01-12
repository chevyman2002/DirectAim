🚀 Quick Start Guide: [Project Name]
This project allows you to build a high-performance, absolute-positioning pointer for arcade games and desktop use. It uses an IMU (Inertial Measurement Unit) to track your point-of-aim without the need for infrared bars or light sensors.

1. Prerequisites
Hardware: Teensy 4.0 & BNO08x (BNO085 or BNO086).

Software: Arduino IDE with the Teensyduino add-on installed.

Library: Open the Library Manager in Arduino (Ctrl+Shift+I) and install "SparkFun BNO08x".

2. Wiring Diagram
BNO08x 		Pin		Teensy 4.0 Pin		Description
SDA			18		I2C Data 			(Blue)
SCL			19		I2C Clock 			(Yellow)
RST			RST		2					CRITICAL: Hardware Reset
VIN			3.3V						Power
GND			GND		Ground
Trigger		0		Button to Ground
Recenter	1		Button to Ground

3. Software Configuration
Before uploading, open the code and adjust the following variables to match your setup:

NATIVE_X & NATIVE_Y: Your screen resolution (e.g., 1920, 1080).

SCALE: Your Windows DPI scaling (e.g., 1.0 for 100%, 1.5 for 150%).

FOV_X & FOV_Y: Sensitivity. If you have to move too far to reach the edges, decrease these numbers.

4. Uploading to Teensy
Connect your Teensy via USB.

In the Arduino IDE, go to Tools > Board and select Teensy 4.0.

Go to Tools > USB Type and select "Keyboard + Mouse + Joystick".

Click Upload.

5. First-Time Calibration
Hold your controller and point it at the dead center of your screen.

Press the Recenter Button (Pin 1).

The cursor should snap to the center. Move the gun around to verify the cursor tracks your movement.

If the movement is inverted, see the Troubleshooting section in the main documentation.