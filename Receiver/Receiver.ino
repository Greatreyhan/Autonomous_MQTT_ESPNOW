#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <espnow.h>

uint8_t msg[6];

// WiFi
const char *ssid = "infinergy"; 
const char *password = "okeokeoke";  

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "abarobotic";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

uint8_t pwm = 0;

WiFiClient espClient;
PubSubClient client(espClient);

// ESPNOW
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&msg, incomingData, len);
  for(int i = 0; i < 6; i++){
    Serial.println(msg[i]);
  }
  
  Serial.println("-------------");
}

// MQTT
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for(int i = 0; i< length; i++) Serial.print(payload[i]);
  Serial.println("-----------------------");
}
 
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
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  client.loop();
}
