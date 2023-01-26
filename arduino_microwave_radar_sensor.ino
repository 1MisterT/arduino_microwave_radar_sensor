//Wifi and MQTT Libary
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//privat Wifi and MQTT Credentials
#include <credentials.h>

//define Pins for the left and right led wire and Frequency Input
#define RADAR_PIN 4

//set variables for wifi and mqtt login
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqttServer = MQTT_SERVER;
const int   mqttPort = MQTT_PORT;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;

//MQTT Topics, for discovery and publish
const uint32_t unique_id = ESP.getChipId();
String discovery_topic = "homeassistant/binary_sensor/radar_" + String(unique_id) + "/config";
String state_topic = "home/sensor/radar/" + String(unique_id);

//global variables (will be changed at mqtt request)
uint8_t radar_input = 0;
uint8_t value = 0;
uint8_t prev_value = 0;

//initialize Wifi and MQTT Clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //Set Pin Mode for Input and Output Pins
  pinMode(RADAR_PIN, INPUT);

  Serial.begin(9600);

  //Start and connect Wifi
  WiFi.begin(ssid, password);

  //Wait for wifi to connect
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(500);
  }

  //Start and connect MQTT
  client.setServer(mqttServer, mqttPort);

  //Wait for MQTT to connect
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connectet to MQTT!");
      delay(100);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);
    }
  }

  //Discovery Message
  String discovery_string = "{\"name\":\"Radar\",\"device_class\":\"motion\",\"state_topic\":\"" + state_topic +  "\",\"payload_on\":\"1.00\",\"payload_off\":\"0.00\",\"unique_id\":\"radar_" + String(unique_id) + "\"}";
  client.publish(discovery_topic.c_str(), discovery_string.c_str() , true);
}

void reconnect() {
  //Start and connect Wifi
  WiFi.begin(ssid, password);

  //Wait for wifi to connect
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(500);
  }
  //Start and connect MQTT
  client.setServer(mqttServer, mqttPort);

  //Wait for MQTT to connect
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connectet to MQTT!");
      delay(100);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

// the loop function runs over and over again forever
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  long last_millis = millis();
  float sum = 0.0;
  float num = 0.0;

  while (millis() - last_millis < 1000) {
    sum = sum + digitalRead(RADAR_PIN);
    num += 1;
  }
  prev_value = value;
  if ((sum / num) > 0.75)
    value = 1;
  else
    value = 0;
  Serial.println(value);
  if (prev_value != value) {

    char radar_string[10];
    dtostrf(value, 4, 2, radar_string);
    client.publish(state_topic.c_str(), radar_string, true);
    Serial.println(radar_string);
  }
  client.loop();
  delay(100);
}
