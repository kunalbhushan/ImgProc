#include <Servo.h>

int led = 13;
int in = A5;
int motor = 2;
int motorDir = 7;
int motorSpeed = 9;
Servo bucket;
int check = A1;
int iterations = 0;

void setup() {
  pinMode(check, INPUT);
  pinMode(led, OUTPUT);
  pinMode(in, INPUT);
  bucket.attach(2);
  pinMode(motorDir, OUTPUT);
  pinMode(motorSpeed, OUTPUT);
  digitalWrite(motorDir, 1);
  bucket.write(-90);
  delay(1000);
  digitalWrite(led, HIGH);
}

void loop() {
  int readIn = digitalRead(in);
  int checkSwitch = digitalRead(check);
  if (readIn == HIGH) {
    digitalWrite(led, LOW);
    if (checkSwitch == LOW) {
      //analogWrite(motorSpeed, 245);
      digitalWrite(motorSpeed, HIGH);
    } else {
      //analogWrite(motorSpeed, 0);
      digitalWrite(motorSpeed, LOW);
      bucket.write(180);
      delay(2000);
      bucket.write(-90);
      delay(2000);
    }
  } else {
    analogWrite(motorSpeed, 0);
    digitalWrite(led, HIGH);
  }
  iterations++;
}
