#include <Wire.h>
#include <math.h>
#include <SparkFun_BNO08x_Arduino_Library.h>

// --- HARDWARE PIN CONFIGURATION ---
#define TRIGGER_PIN 0
#define CENTER_PIN 1
#define BNO08X_RST 2
#define LED_PIN 13

// SDA is the blue QWIIC wire and it connects to PIN 18.
// SCL is the yellow QWIIC wire and it connects to PIN 19.
// I used a white wire to jumper between the reset RST PIN and the 3.3V PIN. This has now been replaced by connecting the RST PIN to PIN #2 on the Teensy board, with a longer white wire.
// + The reasoning is this reboots the IMU to clear any errors that would otherwise prevent it from starting/functioning. If you don't connect this PIN to anything, it will not work 90%+ of the time.
//The INT PIN is not currently used by the program.

// --- SENSITIVITY SETTINGS ---
float FOV_X = 45.0;
float FOV_Y = 35.0;

// --- RESOLUTION & DPI SETTINGS ---
float NATIVE_X = 2560;  // Physical screen width
float NATIVE_Y = 1600;  // Physical screen height
float SCALE = 1.5;      // 150% Scaling

// Mathematical Logical Resolution for Windows 150% DPI
int RES_X = (int)(NATIVE_X / SCALE); 
int RES_Y = (int)(NATIVE_Y / SCALE);

BNO08x myIMU;

float yawOffset = 0;
float pitchOffset = 0;
bool triggerHeld = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(CENTER_PIN, INPUT_PULLUP);
  
  // 1. HARDWARE RESET SENSOR
  pinMode(BNO08X_RST, OUTPUT);
  digitalWrite(BNO08X_RST, LOW);
  delay(100);
  digitalWrite(BNO08X_RST, HIGH);
  delay(1000); 

  // 2. INITIALIZE I2C
  Wire.begin();
  Wire.setClock(100000); 

  // 3. INITIALIZE BNO08x
  if (myIMU.begin(0x4B, Wire) == false) {
    if (myIMU.begin(0x4A, Wire) == false) {
      while (1) { 
        digitalWrite(LED_PIN, HIGH); delay(100);
        digitalWrite(LED_PIN, LOW); delay(100);
      }
    }
  }

  myIMU.enableGameRotationVector(10);

  // 4. CONFIGURE SCREEN SIZE
  Mouse.screenSize(RES_X, RES_Y);
  Joystick.useManualSend(true);
}

void loop() {
  float yaw, pitch, quatI, quatJ, quatK, quatR;

  if (myIMU.wasReset()) {
    myIMU.enableGameRotationVector(10);
  }

  if (myIMU.getSensorEvent() == true) {
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_GAME_ROTATION_VECTOR) {

      quatI = myIMU.getQuatI();
      quatJ = myIMU.getQuatJ();
      quatK = myIMU.getQuatK();
      quatR = myIMU.getQuatReal();

      // Convert to Euler Angles
      yaw = atan2(2.0 * (quatR * quatK + quatI * quatJ), 1.0 - 2.0 * (quatJ * quatJ + quatK * quatK));
      pitch = asin(2.0 * (quatR * quatJ - quatK * quatI));

      yaw *= (180.0 / M_PI);
      pitch *= (180.0 / M_PI);

      // --- RECENTER LOGIC ---
      if (digitalRead(CENTER_PIN) == LOW) {
        yawOffset = yaw;
        pitchOffset = pitch;
        Serial.println("--- RECENTERED ---");
      }

      float deltaX = yaw - yawOffset;
      float deltaY = pitch - pitchOffset;

      if (deltaX > 180) deltaX -= 360;
      if (deltaX < -180) deltaX += 360;

      // --- 1. MOUSE OUTPUT ---
      // X-Axis: Flipped to RES_X -> 0 to fix inversion
      long mouseX = map(deltaX * 100, -FOV_X * 50, FOV_X * 50, RES_X, 0);
      
      // Y-Axis: Kept as RES_Y -> 0 (user confirmed correct)
      long mouseY = map(deltaY * 100, -FOV_Y * 50, FOV_Y * 50, RES_Y, 0);

      mouseX = constrain(mouseX, 0, RES_X);
      mouseY = constrain(mouseY, 0, RES_Y);

      Mouse.moveTo(mouseX, mouseY);

      // --- 2. JOYSTICK OUTPUT (0-1023) ---
      // Keeping joystick axes matched to mouse axes
      long joyX = map(deltaX * 100, -FOV_X * 50, FOV_X * 50, 1023, 0);
      long joyY = map(deltaY * 100, -FOV_Y * 50, FOV_Y * 50, 1023, 0);

      Joystick.X(constrain(joyX, 0, 1023));
      Joystick.Y(constrain(joyY, 0, 1023));
    }
  }

  // --- BUTTON LOGIC ---
  if (digitalRead(TRIGGER_PIN) == LOW) {
    if (!triggerHeld) {
      Mouse.press(MOUSE_LEFT);
      Joystick.button(1, 1);
      triggerHeld = true;
    }
  } else {
    if (triggerHeld) {
      Mouse.release(MOUSE_LEFT);
      Joystick.button(1, 0);
      triggerHeld = false;
    }
  }
  Joystick.send_now();
}