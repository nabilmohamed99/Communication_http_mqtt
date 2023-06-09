#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// Informations WiFi
const char* WIFI_SSID = "ELEXPERT_1";
const char* WIFI_PASSWORD = "ELEXPERT2022";

// Informations MQTT
const char* MQTT_BROKER = "ub1263b2.emqx.cloud";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "meier";
const char* MQTT_PASSWORD = "123";
const char* MQTT_TOPIC = "devices/device_1/sensor_data";
  // Remplacez "device1" par l'identifiant de votre appareil

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);

  // Connexion au réseau WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");

  // Connexion au broker MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  
  while (!mqttClient.connected()) {
    if (mqttClient.connect("NodeMCU", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying...");
      delay(2000);
    }
  }
  mqttClient.subscribe(MQTT_TOPIC);
}

void loop() {
  // Read sensor data
  float temperature = 5;
  float energy = 150.0;  // Placeholder for energy data
  float fluid = 0.0;   // Placeholder for fluid data

  // Publish the sensor data to the MQTT broker
  publishSensorData(temperature, energy, fluid);

  // Handle MQTT messages if any
  mqttClient.loop();
  
  // Delay before reading and publishing the next set of sensor data
  delay(5000);  // Delay for 1 minute
}


void publishSensorData(float temperature, float energy, float fluid) {
  // Create a JSON payload
  StaticJsonDocument<200> jsonDocument;

  jsonDocument["energy"] = energy;
  jsonDocument["fluid"] = fluid;

  // Serialize JSON to a string
  String payload;
  serializeJson(jsonDocument, payload);
    mqttClient.publish(MQTT_TOPIC, payload.c_str());

  // Publish the sensor data to the MQTT broker

}