#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Initialize servo
Servo myservo;

// WiFi
const char *ssid = "infinergy"; 
const char *password = "okeokeoke";  

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "abarobotic/ser";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t msg[8];

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

  myservo.attach(D1);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.println("Message:");
  for(int i = 0; i< length; i++){
    if(i%2 == 0){
      char bufA = char(payload[i]);
      char bufB = char(payload[i+1]); 
      char res[3];
      sprintf(res, "%c%c", bufA, bufB);
      int hexValue = strtol(res, NULL, 16);
      msg[i/2] = hexValue;
      Serial.println(hexValue);  
    }
  }
  if((msg[0] == 165) && (msg[1] == 90)){
    myservo.write(msg[2]/0.708333);
    Serial.println("Servo is moving");
  }
  else Serial.println("Servo header wrong");
  Serial.println("-----------------------");
}
void loop() {
  client.loop();
}
