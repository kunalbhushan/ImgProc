int led = 13;
int in = A5;
int motorDir = 8;
int motorSpeed = 10;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(in, INPUT);
  pinMode(motorDir, OUTPUT);
  pinMode(motorSpeed, OUTPUT);
  digitalWrite(motorDir, 1);
  digitalWrite(motorSpeed, HIGH);
  Serial.println("HELO");
}

void loop() {
  int read = digitalRead(in);
  if (read == HIGH) {
    digitalWrite(motorSpeed, LOW);
  } else {
    digitalWrite(motorSpeed, HIGH);
  }
}
