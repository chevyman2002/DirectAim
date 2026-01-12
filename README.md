🎯 DirectAim: The Driverless, Absolute-Positioning Spatial Gun
Created by: Aaron Merritt

Hardware: Teensy 4.0 + BNO086 IMU

📖 The Story
DirectAim was born out of a simple desire: to play light gun games with the family without the headache. Standard IR-based light guns (like the Wiimote or AimTrak) require sensor bars, calibration for different screen sizes, and struggle with large projectors or small laptop screens. I wanted a "grab-and-go" solution that worked directly on a laptop, connected a TV in the living room, or a 120-inch projector screen—without custom drivers or expensive proprietary hardware.

DirectAim uses high-fidelity spatial orientation to provide Absolute Mouse Positioning, meaning where you point is where you shoot, regardless of the display technology.

❔ Why DirectAim?
Traditional light guns are expensive and finicky. Most DIY solutions use IR LEDs which are a nightmare to set up on different displays. DirectAim is different. It uses inertial measurement to calculate your aim, making it truly display-agnostic.

No IR Bars: Play on a laptop, a TV, or a giant projector screen.

Absolute Accuracy: No "floating" cursor; it maps 1:1 to your screen pixels.

Zero Calibration: Once your DPI and Resolution are set in the firmware, just hit the "Recenter" button and play. If it ever feels "off center", just press the button again and it instantly re-centers.

Tactile Feedback: Designed for a limit-switch trigger for that authentic arcade "click."

🧠 How It Works
DirectAim operates differently than a standard mouse or a traditional IR light gun. Here’s the breakdown:

1. Orientation, Not Motion
Most "Air Mice" use relative motion—they tell the computer "move 5 pixels left." This causes "drift," where the gun is pointed at the screen but the cursor is stuck in a corner. DirectAim uses the BNO08x IMU to track Absolute Orientation in 3D space. It calculates the gun's Yaw (left/right) and Pitch (up/down) angles relative to a "Center" point you define.

2. Angular Mapping
Instead of telling Windows to "move," DirectAim tells Windows "I am at these exact coordinates."

The code takes your gun's angle (e.g., 10° Left, 5° Up).

It maps those angles to a Virtual Resolution box that matches your screen's pixel grid.

It then uses the Absolute Mouse HID protocol to "teleport" the cursor to that exact pixel.

3. The "DPI-Aware" Logic
Windows "Scaling" (125%, 150%, etc.) often breaks absolute pointers because it lies to the hardware about how many pixels are actually on the screen. DirectAim includes a Scale Factor in the firmware to math-out this offset, ensuring that your physical point-of-aim and the digital cursor stay perfectly aligned even on high-DPI laptop displays.

4. Hardware Stability
The BNO08x is a powerful sensor, but it is notoriously finicky during startup. This project utilizes a dedicated Hardware Reset (Pin 2). On every power-up, the Teensy pulses the sensor's reset pin to clear the I2C bus. This eliminates the "Initialization Failed" errors common in other DIY builds, making the gun truly plug-and-play.

🛠️ The Hardware Build
Brain: Teensy 4.0 (Small, powerful, and native HID support).

Eyes: BNO08x (BNO085/086) 9-axis IMU for drift-free orientation.

Shell: A heavily modified Halo Pistol 3D model, redesigned for secure component mounting.

Trigger: A custom-engineered mechanism that utilizes a limit switch with a lift-arm design for a tactile, "clicky" arcade feel.

Bill of Materials
Component              Link                           Note
Teensy 4.0             https://amzn.to/4qgSd0F    High-speed HID-capable brain.
SparkFun BNO086 IMU    https://amzn.to/458Fchj    The "eyes" of the gun. 9-axis drift-free tracking.
I2C QWIIC Cable Kit    https://amzn.to/45rZCCb    Cut one end off to solder directly to Teensy pins 18, 19, 3.3V, and GND.
6pcs Micro Limit Switch https://amzn.to/4sv4IqV   Use COM and NO blades. Designed for the custom "lift-arm" trigger.
6-Color Wire Spools    https://amzn.to/3YzlOX2    You could do with a smaller gauge wire, this is just what I had on hand from a year+ ago.
Soldering Station      https://amzn.to/3NheQDA    I've had this one for over a year and used it quite a bit without any issues so far.
3-Pack 10ft USB-Micro Cables - https://amzn.to/4jy5Gi1 - I don't have these but may eventually buy them since I'm using a 6ft FireStick cable for now.

Buttons - I haven't figured out what I'm going to use yet. I had some breadboard tactile buttons I used when building this, but I've only just reached a point where the 3D gun fits everything and works. I'll figure out what buttons are good and add this later.
Machine Screws - I had some small screws lying around that I used to mount the limit switch temporarily. I'm currently printing another slide part where this section gets a little thicker, so I'll figure out what length I need and paste a link to what I go with, soon.
Trigger Pin - I haven't figured out what I'm going to do for this. I tried 3D printing pins to snugly fit, and even tried printing-in-place the trigger with a solid pin going through holes - but it would snap every time I tried to use it. For now, I have a 1/16" hole going all the way through and I took a cotter pin I had lying around and secured it with that. I'll change this eventually, I think, to avoid getting scratched by it in the future (hasn't happened yet, but it will eventually).

Micro Limit Switch notes (in case the link doesn't work or you want more details on them). These have a roller tip on them as well.
Model: V-156-1C25
Rating: 15A, 1/2Hp, 125/250V AC; 0.6A,125V DC; 0.3A, 250V DC; 0.6A,125V DC; 0.3A, 250V DC
Features: long hinge lever actuator, 1NO+1NC contact configurations, 3 terminals.
Size of Switch: 1.1× 0.63× 0.39in
Diameter of Fixing Hole: 0.13in
Height of Button: 0.12in
Actuator Size: 2.0× 0.16in
Actuator Action: momentary snap action switch
