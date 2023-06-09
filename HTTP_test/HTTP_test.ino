#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

// Remplacez les informations suivantes par les informations de votre réseau Wi-Fi
const char* ssid = "ELEXPERT_1";
const char* password = "ELEXPERT2022";
const char* serverURL = "http://localhost:8000/http_communication/sensor_input_database";
WiFiClient client; 


unsigned long previousMillis = 0;
const unsigned long interval = 3000; // Intervalle de 3 secondes

void setup() {
  Serial.begin(115200);

  // Connexion au réseau Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi...");
  }
  Serial.println("Connecté au réseau Wi-Fi");
}

void loop() {
  unsigned long currentMillis = millis();

  // Vérifie si l'intervalle de temps est écoulé
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Met à jour le dernier temps de déclenchement

    // Effectue l'envoi de requête HTTP
    sendHttpRequest();
  }

  // Votre code principal
}

void sendHttpRequest() {
  // Créez un objet HTTPClient
  HTTPClient http;

  // Configurez l'URL du serveur
  http.begin(client, serverURL);

  // Définissez le contenu de la requête POST
  String payload = "{\"energy\": \"1\",\"fluid\": \"1\",\"device_name\": \"device_1\"}";

  // Envoyez la requête POST
  int httpResponseCode = http.POST(payload);

  // Vérifiez le code de réponse HTTP
  if (httpResponseCode > 0) {
    Serial.print("Réponse du serveur: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Erreur lors de l'envoi de la requête. Code d'erreur: ");
    Serial.println(httpResponseCode);
  }

  // Libérez les ressources
  http.end();
}