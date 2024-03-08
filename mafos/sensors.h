#include <dht11.h>

struct SensorData {
  float temperature;
  float humidity;
  int lightIntensity;
};

dht11 DHT11;


SensorData readSensors() {
  SensorData data;

  int chk = DHT11.read(DHT11PIN);
  delay(2000);

  data.temperature = (float)DHT11.temperature;
  data.humidity = (float)DHT11.humidity;
  data.lightIntensity = analogRead(LIGHT_RESISTOR_PIN);

  Serial.print("Temperature  (C): ");
  Serial.println(data.temperature, 2);

  Serial.print("Humidity (%): ");
  Serial.println(data.humidity, 2);

  Serial.print("Light intensity: ");
  Serial.println(data.lightIntensity);
  
  return data;
}

