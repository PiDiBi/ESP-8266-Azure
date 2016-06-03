#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "DHT.h"
#include <Wire.h> 


const char *SSID = "Microsoft-IoT";
const char *PASSWORD = "MicrosoftAzure";

WiFiClientSecure client;
// END: WiFi settings


// START: Azure IoT settings
const char* HOST = "[todo].azure-devices.net";
const char* DEVICE_NAME = "[todo]";
const char* SAS = "SharedAccessSignature sr=[todo]";
                  
String urlRequest;
// END: Azure IoT settings


// START: Sensors and display
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 or DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
   // START: Sensor and display init
  
  
  dht.begin();
  // END: Sensor and display init  

  Serial.print("connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  //WiFi.begin(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  urlRequest = "/devices/" + (String)DEVICE_NAME + "/messages/events?api-version=2015-08-15-preview";
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("ERROR 01 Read from DHT sensor");
    return;
  }

  
  Serial.print(h);
  Serial.print(";");
  Serial.println(t);
  
  
  String data = "{'DeviceId':'" + String(DEVICE_NAME) + "', 'Temperature':" + String(t) + ", 'Humidity':" + String(h) + "}";
  String request = String("POST ") + urlRequest + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Authorization: " + SAS + "\r\n" +                
               "Content-Type: application/json\r\n" + 
               "Connection: close\r\n" +
               "Content-Length: " + data.length() + "\r\n\r\n" +
               data;
               
  // Use WiFiClientSecure class to create TLS connection
  if (!client.connect(HOST, 443)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.println(request);
  client.print(request);

  String response = "";
  while (client.connected()) {
    response += client.readStringUntil('\n');
  }

  Serial.print("Client connected: ");
  Serial.println(client.connected());

  Serial.println(response);

  Serial.println();
  Serial.print("Response code: ");
  Serial.println(response.substring(9, 12));
  Serial.println("---------------------------");

  delay(5000);
}
