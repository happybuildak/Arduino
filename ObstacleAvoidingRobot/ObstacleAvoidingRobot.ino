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

const int MinDistance = 60;

const int Led_Foward = 12;
const int Led_Left = 13;
const int Led_Right = 11;

const int Moter_Left = 1;
const int Moter_Right = 2;

const int Turn_Angle = 125;

long distanceRight = 0;
long distanceLeft = 0;

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
Servo myservo;

void setup() {
  pinMode(Led_Foward, OUTPUT);
  pinMode(Led_Right, OUTPUT);
  pinMode(Led_Left, OUTPUT);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  myservo.attach(9);

  delay(3000);
  Forwards();
}

void loop() {
  if (sr04.Distance() < MinDistance) {
    Stop();

    distanceRight = Scan(-45);
    if (distanceRight > MinDistance) {
      TurnRight();
    } else {
      distanceLeft = Scan(45);
      if (distanceLeft > MinDistance) {
        TurnLeft();
      } else {
        if (distanceLeft > distanceRight) {
          TurnLeft();
        } else {
          TurnRight();
        }
      }
    }

    Forwards();
  }
}

void Forwards() {
  digitalWrite(Led_Foward, 1);
  digitalWrite(Led_Left, 0);
  digitalWrite(Led_Right, 0);
  Go(Moter_Right, 255);
  Go(Moter_Left, 255);
}

void TurnLeft() {
  digitalWrite(Led_Foward, 0);
  digitalWrite(Led_Left, 1);
  digitalWrite(Led_Right, 0);
  Go(Moter_Right, 255);
  Go(Moter_Left, -255);
  delay(Turn_Angle);
}

void TurnRight() {
  digitalWrite(Led_Foward, 0);
  digitalWrite(Led_Left, 0);
  digitalWrite(Led_Right, 1);
  Go(Moter_Right, -255);
  Go(Moter_Left, 255);
  delay(Turn_Angle);
}

void Stop() {
  Go(Moter_Right, 0);
  Go(Moter_Left, 0);
}

int Scan(int turnAngle) {
  // Turn sensor to Angle and wait to take reading.
  myservo.write(turnAngle + 90);
  delay(600);
  long usDistance = sr04.Distance();

  // Turn sensor back to straight
  myservo.write(90);
  delay(600);
  return usDistance;
}

// Helper Functions

// Set motor speed: 255 full ahead, -255 full reverse, 0 stop
void Go(int MoterState, int speed) {
  // Left motor control
  if (MoterState == 1) {
    digitalWrite(in1Pin, speed > 0 ? HIGH : LOW);
    digitalWrite(in2Pin, speed <= 0 ? HIGH : LOW);
    analogWrite(enAPin, speed < 0 ? -speed : speed);
  }
  // Right motor control
  else if (MoterState == 2) {
    digitalWrite(in3Pin, speed > 0 ? HIGH : LOW);
    digitalWrite(in4Pin, speed <= 0 ? HIGH : LOW);
    analogWrite(enBPin, speed < 0 ? -speed : speed);
  }
}
