#define WIFI_SSID "XXX"
#define WIFI_PASSWORD "XXX"

#define MQTT_USERNAME "XXX"
#define MQTT_PASSWORD "XXX"
#define MQTT_SERVER "XXX"
#define MQTT_SERVER_PORT 1883

#define BME280_ADDRESS (0x77) // Default on LaskaKit module
#define SLEEP_SEC 15 * 60     // Measurement interval (seconds)
#define ADC_PIN 0             // ADC pin on LaskaKit Meteo mini
#define deviderRatio                                                           \
  1.7693877551 // Voltage devider ratio on ADC pin 1M + 1.3MOhm
#define SDA 19
#define SCL 18
#define PIN_ON 3
