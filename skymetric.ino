void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

  Serial.println("ESP32 hidup bro");
}

void loop() {
  digitalWrite(2, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);

  Serial.println("LED ON");
  delay(1000);

  digitalWrite(2, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);

  Serial.println("LED OFF");
  delay(1000);
}