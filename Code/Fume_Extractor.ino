#include <servo.h>

const int fanPin = 3;
const int potPin = A0;

const int servoPin = 9;
Servo hoodServo;

const int encClkPin = 2;
const int encDtPin = 5;
const int encSwPin = 6;

int fanSpeed = 0;
bool SystemOn = true;
bool lastSwState = HIGH;

volatile int servo angle = 90;
volatile int lastClkState = LOW;

void setup() {
  Serial.begin(9600)

  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);

  pinMode(potPin, INPUT);

  hoodServo.attach(servoPin);
  hoodServo.write(servoAngle);

  pinMode(encClkPin, INPUT);
  pinMode(encDtPin, INPUT);
  pinMode(encSwPin, INPUT_PULLUP);

  lastClkState = digitalRead(encClkPin);

  attachInterrupt(digitalPinToInterrupt(encClkPin), readEncoder, CHANGE);
}

void loop() {
  bool currentSwState = digitalRead(encSwPin);
  if (currentSwState == LOW && lastSwState == HIGH){
    systemOn = !systemOn;
    delay(50);
  }
  lastSwState = currentSwState;

  if (systemOn) {
    int potValue = analogRead(potPin);
    fanSpeed = map(potValue, 0, 1023, 0, 255);
    analogWrite(fanPin, fanSpeed);
  } else{
    analogWrite(fanPin, 0);
  }

  hoodServo.write(servoAngle);
  delay(10)
}

void readEncoder() {
  int currentClkState = digitalRead(encClkPin);

  if (currentClkState != lastClkState && currentClkState == HIGH) {
    if (digitalRead(encDtPin) != currentClkState){
      servoAngle -= 5;
    }else {
      servoAngle +=5;
    }
    
    if (servoAngle < 0) servoAngle = 0;
    if (servoAngle > 180) servoAngle = 180;
  }

  lastClkState = currentClkState;
}