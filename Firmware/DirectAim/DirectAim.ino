/*
 * ======================================================================================
 * PROJECT: DirectAim IMU Gun
 * VERSION: 1.2.1 (Soft-Reset Update)
 * ======================================================================================
 */

#include <Wire.h>
#include <math.h>
#include <SparkFun_BNO08x_Arduino_Library.h>

#define TRIGGER_PIN 0
#define CENTER_PIN 1
#define BNO08X_RST 2
#define BUTTON_1_PIN 3
#define BUTTON_2_PIN 4
#define LED_PIN 13

float FOV_X = 45.0;
float FOV_Y = 35.0;
float NATIVE_X = 2560;  
float NATIVE_Y = 1600;  
float SCALE = 1.5;      

int RES_X = (int)(NATIVE_X / SCALE); 
int RES_Y = (int)(NATIVE_Y / SCALE);

BNO08x myIMU;
float yawOffset = 0, pitchOffset = 0;
bool triggerHeld = false, b1Held = false, b2Held = false;

// Reset Logic
unsigned long resetTimer = 0;
const unsigned long RESET_THRESHOLD = 5000; 

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(CENTER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  
  pinMode(BNO08X_RST, OUTPUT);
  digitalWrite(BNO08X_RST, LOW);
  delay(100);
  digitalWrite(BNO08X_RST, HIGH);
  delay(1000); 

  Wire.begin();
  Wire.setClock(100000); 

  if (myIMU.begin(0x4B, Wire) == false) {
    if (myIMU.begin(0x4A, Wire) == false) {
      while (1) { digitalWrite(LED_PIN, HIGH); delay(100); digitalWrite(LED_PIN, LOW); delay(100); }
    }
  }

  myIMU.enableGameRotationVector(10);
  Mouse.screenSize(RES_X, RES_Y);
  Joystick.useManualSend(true);
}

void loop() {
  // --- 1. THE SOFT RESET LOGIC (TOP PRIORITY) ---
  // Check if ALL 4 buttons are physically held LOW
  bool triggerP = (digitalRead(TRIGGER_PIN) == LOW);
  bool centerP  = (digitalRead(CENTER_PIN) == LOW);
  bool redP     = (digitalRead(BUTTON_1_PIN) == LOW);
  bool greenP   = (digitalRead(BUTTON_2_PIN) == LOW);

  if (triggerP && centerP && redP && greenP) {
    if (resetTimer == 0) {
      resetTimer = millis(); 
      Serial.println("Reset Timer Started...");
    } 
    
    // Visual Feedback: Blink the LED faster as we get closer to 5s
    unsigned long elapsed = millis() - resetTimer;
    if (elapsed > 100) digitalWrite(LED_PIN, (millis() / 100) % 2); 

    if (elapsed > RESET_THRESHOLD) {
      Serial.println("Jumping to Bootloader!");
      // Final fast strobe to confirm success
      for(int i=0; i<20; i++) { digitalWrite(LED_PIN, !digitalRead(LED_PIN)); delay(20); }
      _reboot_Teensyduino_(); 
    }
    
    // IMPORTANT: We return here so we don't send mouse/keyboard 
    // movements while trying to reset.
    return; 
  } else {
    resetTimer = 0; 
    // If not resetting, make sure LED is off (or stays on for power indicator)
    digitalWrite(LED_PIN, LOW); 
  }

  // --- 2. IMU / MOUSE MOVEMENT ---
  if (myIMU.wasReset()) { myIMU.enableGameRotationVector(10); }

  if (myIMU.getSensorEvent() == true) {
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_GAME_ROTATION_VECTOR) {
      float qI = myIMU.getQuatI();
      float qJ = myIMU.getQuatJ();
      float qK = myIMU.getQuatK();
      float qR = myIMU.getQuatReal();

      float yaw = atan2(2.0 * (qR * qK + qI * qJ), 1.0 - 2.0 * (qJ * qJ + qK * qK));
      float pitch = asin(2.0 * (qR * qJ - qK * qI));
      yaw *= (180.0 / M_PI);
      pitch *= (180.0 / M_PI);

      // Normal Recenter (Only if not holding all buttons)
      if (centerP) {
        yawOffset = yaw;
        pitchOffset = pitch;
      }

      float dX = yaw - yawOffset;
      float dY = pitch - pitchOffset;
      if (dX > 180) dX -= 360; if (dX < -180) dX += 360;

      long mX = map(dX * 100, -FOV_X * 50, FOV_X * 50, RES_X, 0);
      long mY = map(dY * 100, -FOV_Y * 50, FOV_Y * 50, RES_Y, 0);
      Mouse.moveTo(constrain(mX, 0, RES_X), constrain(mY, 0, RES_Y));

      Joystick.X(map(dX * 100, -FOV_X * 50, FOV_X * 50, 1023, 0));
      Joystick.Y(map(dY * 100, -FOV_Y * 50, FOV_Y * 50, 1023, 0));
    }
  }

  // --- 3. STANDARD BUTTON OUTPUTS ---
  handleButton(TRIGGER_PIN, triggerHeld, MOUSE_LEFT, 0, 1);
  handleButton(BUTTON_1_PIN, b1Held, MOUSE_RIGHT, 0, 2);
  handleButton(BUTTON_2_PIN, b2Held, 0, '1', 3);

  Joystick.send_now();
}

void handleButton(int pin, bool &state, int mouseBtn, char keyChar, int joyBtn) {
  if (digitalRead(pin) == LOW) {
    if (!state) {
      if (mouseBtn != 0) Mouse.press(mouseBtn);
      if (keyChar != 0) Keyboard.press(keyChar);
      Joystick.button(joyBtn, 1);
      state = true;
    }
  } else {
    if (state) {
      if (mouseBtn != 0) Mouse.release(mouseBtn);
      if (keyChar != 0) Keyboard.release(keyChar);
      Joystick.button(joyBtn, 0);
      state = false;
    }
  }
}