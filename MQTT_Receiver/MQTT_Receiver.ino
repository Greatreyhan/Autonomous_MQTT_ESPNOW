#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "infinergy"; 
const char *password = "okeokeoke";  

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "abarobotic";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t msg[6];

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }

  client.publish(topic, "ready");
  client.subscribe(topic);

}

void callback(char *topic, byte *payload, unsigned int length) {
  memcpy(&msg, &payload, length);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for(int i = 0; i< length; i++) Serial.print((uint8_t)payload[i]);
  Serial.println("-----------------------");
}
void loop() {
  client.loop();
//  client.publish("abarobotic/data","ready");
}
