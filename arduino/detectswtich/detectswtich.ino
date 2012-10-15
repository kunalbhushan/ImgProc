int led = 13;
int in = A5;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(in, INPUT);
  Serial.println("HELO");
}

// the loop routine runs over and over again forever:
void loop() {
  int read = digitalRead(in);
  if (read == 1) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
