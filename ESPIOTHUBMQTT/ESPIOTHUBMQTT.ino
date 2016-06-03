#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// START: Sensors and display
#include <Wire.h> 
#include "DHT.h"

#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22 or DHT11

DHT dht(DHTPIN, DHTTYPE);


const char *ssid = "Microsoft-IoT";
const char *password = "MicrosoftAzure";

// change PubSubClient.h #define MQTT_MAX_PACKET_SIZE 256
const char* mqtt_server = "[todo].azure-devices.net";
const char* SAS = "SharedAccessSignature sr=[todo]";
                   
const char* DEVICE_NAME = "[todo]";

WiFiClientSecure espClient;

PubSubClient client(espClient);


long lastMsg = 0;
char msg[50];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Entered callback");
  // handle message arrived
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.println(msg);
}

void setup() {
  Serial.begin(9600);

  // START: Sensor and display init
  //Wire.begin(5, 4);  
  
  dht.begin();
  // END: Sensor and display init  
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(DEVICE_NAME, ((String)mqtt_server + "/" + (String)DEVICE_NAME).c_str(), SAS)) {
        Serial.println("Connected to MQTT server");
        client.subscribe(("devices/"+(String)DEVICE_NAME+"/messages/devicebound/#").c_str());
      }
      else {
        Serial.println("Could not connect to MQTT server");
      }
    }

    if (client.connected())
    {
      client.loop();
    }

    if (millis() % 5000 == 0) {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
    
      String data = "{'DeviceId':'"+(String)DEVICE_NAME+"', 'Temperature':" + String(t) + ", 'Humidity':" + String(h) + "}";
      Serial.println(data);
      client.publish(("devices/"+(String)DEVICE_NAME+"/messages/events/").c_str(), data.c_str());
    }
  }
}
