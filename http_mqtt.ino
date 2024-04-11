#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <iostream>
#include <string>




// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "*********";
const char* password = "******";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "ub1263b2.emqx.cloud";
int  mqtt_port=1883;
const char *host = "http://192.168.1.179:8000/insert/";

unsigned long previousMillis=0;
const long interval=10000;
// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board


// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="read_data/device_1"){
      Serial.print("commencer la lecture");
      if(messageTemp == "read"){
      
        Serial.print("On");
        previousMillis = millis();
      }
      else if(messageTemp == "off"){

        Serial.print("Off");
        previousMillis = 0;
      }
  }
  Serial.println();
}
void post_json(){
    WiFiClient client;
    HTTPClient http;

    String energy= "1";
    String fluid = "1";
    String device_name = "device_1";

    String postData;
    postData = "{\"energy\": \"" + energy + "\",\"fluid\": \"" + fluid + "\",\"device_name\": \"" + device_name + "\"}";

    Serial.print("Request Link:");
    Serial.println(host);

    Serial.print("Post Data:");
    Serial.println(postData);

    http.begin(client, host);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(postData);
    String payload = http.getString();

    Serial.print("Response Code:");
    Serial.println(httpCode);
    Serial.print("Returned data from Server: ");
    Serial.println(payload);
    http.end();
    delay(5000);
}


// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCU","meier","123")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("read_data/device_1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {

    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
   client.connect("ESPO","*","123");

  unsigned long currentMillis = millis();
  if (previousMillis > 0 && currentMillis - previousMillis >= interval) {
    // Temps écoulé, envoi de la requête POST
    post_json();

    previousMillis = currentMillis;
  }
 
}
