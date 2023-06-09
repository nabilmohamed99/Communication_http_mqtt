#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "ELEXPERT_1";
const char* password = "ELEXPERT2022";
const char* mqtt_server = "ub1263b2.emqx.cloud";
const int mqtt_port = 1883;
const char* mqtt_username = "meier";
const char* mqtt_password = "123";
const char* device_mac_address = "device";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect(device_mac_address, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe("read_data/device1" );
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle the received MQTT message
  String topicStr = topic;
  if (topicStr == "read_data/" + String(device_mac_address)) {
    // Parse the payload and retrieve the sensor data
    // You will need to adapt this code based on your actual payload structure
    String payloadStr = "";
    for (int i = 0; i < length; i++) {
      payloadStr += (char)payload[i];
    }
    // Retrieve the sensor data (temperature, energy, fluid) from your sensors
 
    float energy = 120.0;
    float fluid = 3.5;

    // Publish the sensor data to the MQTT topic
    StaticJsonDocument<200> jsonDocument;

    jsonDocument["energy"] = energy;
    jsonDocument["fluid"] = fluid;

    String payload;
    serializeJson(jsonDocument, payload);
    mqttClient.publish("sensor_data/device_1", payload.c_str());
 
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(device_mac_address, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe("read_data/device_1");
    }
    else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}