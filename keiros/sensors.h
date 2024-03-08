#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>   
#include <ESP32AnalogRead.h>      // ESP32AnalogRead by madhephaestus https://github.com/madhephaestus/ESP32AnalogRead 

struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  float batteryVoltage;
};

Adafruit_BME280 bme;
ESP32AnalogRead adc;

float readBat(){
  float voltage = adc.readVoltage()*deviderRatio;
  Serial.println("Battery voltage " + String(voltage) + "V");

  return voltage;
}

SensorData readSensors() {
  adc.attach(ADC_PIN);  

  while(!Serial) {} // Wait

  // for board version over 3.5 need to turn uSUP ON
  pinMode(PIN_ON, OUTPUT);      // Set EN pin for uSUP stabilisator as output
  digitalWrite(PIN_ON, HIGH);   // Turn on the uSUP power

  // initilizace BME280 | BME280 Initialization
  Wire.begin(SDA,SCL);
  if (! bme.begin(BME280_ADDRESS)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("-- Weather Station Scenario --");
  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );
  delay(10);

  SensorData data;
  data.temperature = bme.readTemperature();
  data.humidity    = bme.readHumidity();
  data.pressure    = bme.readPressure() / 100.0F;  
  data.batteryVoltage = readBat();

    // Print the data
  Serial.print("Temp: "); Serial.print(data.temperature); Serial.println("°C");
  Serial.print("Humidity: "); Serial.print(data.humidity); Serial.println("% RH");
  Serial.print("Pressure: "); Serial.print(data.pressure); Serial.println("hPa");
  Serial.print("Battery voltage: "); Serial.print(data.batteryVoltage); Serial.println("V");
  
  return data;
}