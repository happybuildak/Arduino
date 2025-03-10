
#include <Servo.h>
#include "SR04.h"
#define TRIG_PIN A0
#define ECHO_PIN A1
const int enAPin = 6;  // Left motor PWM speed control
const int in1Pin = 7;  // Left motor Direction 1
const int in2Pin = 5;  // Left motor Direction 2
const int in3Pin = 4;  // Right motor Direction 1
const int in4Pin = 2;  // Right motor Direction 2
const int enBPin = 3;  // Right motor PWM speed control

const int MinDistance = 80;
const int buttonPin = 10;

const int ForwardLed = 12;
const int Leftled = 13;
const int rightled = 11;

// int buttonState = 0;
int turnAngle = 125;
int ledState[3];
enum Motor { LEFT,RIGHT };
String Direction = "Hello World";
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
Servo myservo;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ForwardLed, OUTPUT);
  pinMode(rightled, OUTPUT);
  pinMode(Leftled, OUTPUT);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  //Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
  /* Button Code (In progress);
  while (buttonState == 0) {
    buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
  }
  buttonState = 0;
  */
  Forwards();
  if (Scan(0) < MinDistance) {
    Stop();
    if (Scan(-45) > MinDistance) {
      Right();
      delay(turnAngle);
    } else {
      if (Scan(45) > MinDistance) {
        Left();
        delay(turnAngle);
      } else {
        Left();
        delay(turnAngle);
        /*  Backwards code
        if ((LeftCm >= MinDistance) && (RightCm >= MinDistance)) {
          Backwards();
          digitalWrite(ForwaredLed, HIGH);
          digitalWrite(Leftled, LOW);
          digitalWrite(rightled, HIGH);
          delay(1000);
        }
        */
      }
    }
  }
  //Serial.println(Direction);
  Stop();
}

void Forwards() {
  Direction = "Forwards";
  digitalWrite(ForwardLed, 1);
  digitalWrite(Leftled, 0);
  digitalWrite(rightled, 0);
  go(RIGHT, 255);
  go(LEFT, 255);
}

void Backwards() {
  go(RIGHT, -255);
  go(LEFT, -255);
}

void Left() {
  Direction = "Left";
  digitalWrite(ForwardLed, 0);
  digitalWrite(Leftled, 1);
  digitalWrite(rightled, 0);
  go(RIGHT, 255);
  go(LEFT, -255);
}

void Right() {
  Direction = "Right";
  digitalWrite(ForwardLed, 0);
  digitalWrite(Leftled, 0);
  digitalWrite(rightled, 1);
  go(RIGHT, -255);
  go(LEFT, 255);
}

void Stop() {
  go(RIGHT, 0);
  go(LEFT, 0);
}

int Scan(int TurnDirection) {
  SetAngle(0);
  delay(100);
  SetAngle(TurnDirection);
  delay(100);
  return sr04.Distance();
}
void TurnOnLeds() {
}

// Helper Functions

int SetAngle(int Angle) {
  myservo.write(Angle + 90);
  delay(500);
  int Cm = sr04.Distance()
  /*
  Serial.print("Angle: ");
  Serial.print(Angle);
  Serial.print("° - Distance: ");
  Serial.println(Cm);
  return Cm;
  */
}

// Set motor speed: 255 full ahead, -255 full reverse, 0 stop
void go(enum Motor m, int speed) {
  // Left motor control
  if (m == LEFT) {
    digitalWrite(in1Pin, speed > 0 ? HIGH : LOW);
    digitalWrite(in2Pin, speed <= 0 ? HIGH : LOW);
    analogWrite(enAPin, speed < 0 ? -speed : speed);
  }
  // Right motor control
  else if (m == RIGHT) {
    digitalWrite(in3Pin, speed > 0 ? HIGH : LOW);
    digitalWrite(in4Pin, speed <= 0 ? HIGH : LOW);
    analogWrite(enBPin, speed < 0 ? -speed : speed);
  }
}
