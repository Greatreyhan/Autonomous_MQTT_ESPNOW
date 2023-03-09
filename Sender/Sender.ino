
#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC ADDRESS Penerima
uint8_t broadcastAddress[] = {0xDC, 0x4F, 0x22, 0x68, 0x9E, 0x13};

uint8_t msg[6] = {0xA5,0x5A,0x01,0x01,0x01,0x00};

// Callback jika data dikirimkan
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Status Pengiriman: ");
  if (sendStatus == 0){
    Serial.println("Berhasil");
  }
  else{
    Serial.println("Gagal");
  }
}
 
void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
 
  // Konfigurasi devais sebagai Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Konfigurasi divais sebagai pengirim 
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Konfigurasi alamat penerima pesan
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
    // Mengirimkan pesan ke penerima
    esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
    delay(100);
}
