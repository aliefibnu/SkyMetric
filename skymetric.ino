#include <DHT.h>

const int PIN_LED_ONBOARD = 2;
const int PIN_LED_GREEN = 25;
const int PIN_LED_RED = 26;
const int PIN_DHT = 4;
const int PIN_GUVA = 34;

const int DHT_TYPE = DHT22;
DHT dht(PIN_DHT, DHT_TYPE);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_LED_ONBOARD, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  dht.begin();

  Serial.println("ESP32 start");
}

void loop()
{
  digitalWrite(PIN_LED_ONBOARD, HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED, HIGH);

  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();
  int guvaRaw = analogRead(PIN_GUVA);
  float guvaVolt = (guvaRaw / 4095.0f) * 3.3f;

  Serial.println("=== LED ON ===");
  if (isnan(humidity) || isnan(temperatureC))
  {
    Serial.println("DHT22 gagal dibaca");
  }
  else
  {
    Serial.print("Suhu (C): ");
    Serial.println(temperatureC);
    Serial.print("Kelembapan (%): ");
    Serial.println(humidity);
  }

  Serial.print("GUVA raw: ");
  Serial.println(guvaRaw);
  Serial.print("GUVA volt: ");
  Serial.println(guvaVolt, 3);
  Serial.println();

  delay(1000);

  digitalWrite(PIN_LED_ONBOARD, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  Serial.println("=== LED OFF ===");

  delay(1000);
}