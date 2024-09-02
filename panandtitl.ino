#include <Servo.h>

// Define joystick and button pins
const int joyX = A0;
const int joyY = A1;
const int joyButton = 2;  // The button on the joystick to switch modes

// Servos for pan and tilt
Servo panServo;
Servo tiltServo;

// Mode flag
bool autoTrackingMode = false;  // Start in manual mode

void setup() {
  panServo.attach(9);
  tiltServo.attach(10);
  pinMode(joyButton, INPUT_PULLUP);  // Set the joystick button as input
  Serial.begin(9600);
}

void loop() {
  // Check the joystick button state
  int joyButtonState = digitalRead(joyButton);

  // Toggle mode if the joystick button is pressed
  if (joyButtonState == LOW) {
    delay(200);  // Debounce delay
    autoTrackingMode = !autoTrackingMode;
    while (digitalRead(joyButton) == LOW);  // Wait for button release
  }

  if (autoTrackingMode) {
    // Automatic object tracking mode
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      int commaIndex = data.indexOf(',');
      int error_x = data.substring(0, commaIndex).toInt();
      int error_y = data.substring(commaIndex + 1).toInt();

      int panPos = constrain(map(error_x, -320, 320, 0, 180), 0, 180);
      int tiltPos = constrain(map(error_y, -240, 240, 0, 180), 0, 180);

      panServo.write(panPos);
      tiltServo.write(tiltPos);
    }
  } else {
    // Manual control with joystick
    int xVal = analogRead(joyX);
    int yVal = analogRead(joyY);

    int panPos = map(xVal, 0, 1023, 0, 180);
    int tiltPos = map(yVal, 0, 1023, 0, 180);

    panServo.write(panPos);
    tiltServo.write(tiltPos);
  }

  delay(15);
}
