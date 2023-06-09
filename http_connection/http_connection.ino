#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "ELEXPERT_1";
const char* password = "ELEXPERT2022";

const char *host = "http://192.168.1.179:8000/insert/";





void setup(){
    // Sets up WiFi connection
    Serial.begin(9600);
   
    Serial.println("\nWiFi station setting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connection established");
    Serial.print("Device ip address: ");
    Serial.println(WiFi.localIP());    
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



void loop(){
            Serial.println("test");
   
            post_json();
            delay(100);
          
}