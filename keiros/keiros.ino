#include "conf.h" // Config should be imported as first!!!
#include <WiFi.h>
#include <MQTT.h>

#include "sensors.h"
#include "ota.h"


WiFiClient wiFiClient;
MQTTClient client;

bool OTAEnabled = false;

bool connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    return false;
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(getIp());

  return true;
}

IPAddress getIp() {
  return WiFi.localIP();
}

WiFiClient getWiFiClient() {
  return wiFiClient;
}

void checkOTAStatus(String status) {
  if (status == "1") {
    Serial.println("Device should switch to OTA ");
    OTAEnabled = true;
    setupOTA();
  } else {
    Serial.println("Device is in normal status ");
    OTAEnabled = false;
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "/outside/kairos/config/ota") {
      checkOTAStatus(payload);
  }
}

void connectToMQTT() {
  client.begin(MQTT_SERVER, MQTT_SERVER_PORT, wiFiClient);
  client.onMessage(messageReceived);
  Serial.print("\nConnecting to mqtt...");
  while (!client.connect("kairos", MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void subscribeToConfigChannel() {
  client.subscribe("/outside/kairos/config/ota");
}

void publishSensorsData(SensorData &data) {
  client.publish("/outside/kairos/weather/temperature", String(data.temperature));
  client.publish("/outside/kairos/weather/humidity", String(data.humidity));    
  client.publish("/outside/kairos/weather/pressure", String(data.pressure));
  client.publish("/outside/kairos/battery/voltage", String(data.batteryVoltage));
}

void goToSleep(){
  WiFi.disconnect(true);
  delay(1);
  // ESP Deep Sleep 
  digitalWrite(PIN_ON, LOW);   // Turn off the uSUP power
  Serial.println("ESP in sleep mode");
  esp_sleep_enable_timer_wakeup(SLEEP_SEC * 1000000);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  
  if (!connectToWiFi()) {
    Serial.println("Connection Failed! Going to sleep mode...");
    goToSleep();
  }

  connectToMQTT();
  subscribeToConfigChannel();
}

void loop() {
  // Listening to the MQTT channels is configuring the device
  client.loop();
  delay(2);
  

  if (OTAEnabled == true) {
    OTAHandler();
    Serial.print("Listening on: http://"); Serial.print(getIp());Serial.println(":3232");
    delay(2);
  } else {
    SensorData data;
    data = readSensors();
    publishSensorsData(data);
    // Do couple of loops to listen to subscribed channels
    client.loop();
    delay(3000);
    client.loop();

    // If OTA got enabled, it has to be handled 
    if (OTAEnabled == true) {
      OTAHandler();
      delay(2);
    } else {
      goToSleep();
    }
  }  
}




