#define SensorPin 0
#define LedPin    2

void setup() {
  // put your setup code here, to run once:
  pinMode(SensorPin, INPUT);
  pinMode(LedPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LedPin,digitalRead(SensorPin));
}
