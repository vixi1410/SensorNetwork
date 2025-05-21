// #include <WiFi.h>

// void setup() {
//   // Khởi tạo Serial Monitor
//   Serial.begin(115200);
  
//   // Đảm bảo WiFi đã được khởi tạo
//   WiFi.mode(WIFI_STA);  // Chế độ station (kết nối mạng)
  
//   // Đọc địa chỉ MAC của ESP32
//   uint8_t mac[6];
//   WiFi.macAddress(mac);

//   // In địa chỉ MAC dưới dạng chuỗi
//   Serial.print("MAC Address: ");
//   for (int i = 0; i < 6; i++) {
//     // In từng byte của địa chỉ MAC
//     if (mac[i] < 0x10) {
//       Serial.print("0");
//     }
//     Serial.print(mac[i], HEX);
//     if (i < 5) {
//       Serial.print(":");
//     }
//   }
//   Serial.println();
// }

// void loop() {
// }

///////////////////////////////////////////////////

// #include <Arduino.h>

// #define RXD2 16
// #define TXD2 17

// void setup() {
//   Serial.begin(115200); // Debug monitor
//   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // UART2 nhận STM32
// }

// void loop() {
//   if (Serial2.available()) {
//     String data = Serial2.readStringUntil('\n'); // Đọc tới khi gặp newline
//     Serial.println("Received: " + data); // In ra debug monitor
//   }
// }

///////////////////////////////////////////////////

//MAC Address: 3C:8A:1F:5D:7F:48


#include <WiFi.h>
#include <esp_now.h>

#define RXD2 16
#define TXD2 17

uint8_t peerAddress[] = {0x94, 0x54, 0xC5, 0xEF, 0xF9, 0x7C};

typedef struct struct_message {
  char sender[100];
  float x;
  int value;
} struct_message;
struct_message incomingData;
struct_message outgoingData;

// Hàm callback khi gửi xong
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Hàm callback khi nhận dữ liệu

void onDataReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  Serial.print("Received from ESP-NOW: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}

// void onDataReceive(const uint8_t *mac, const uint8_t *incomingDataBytes, int len) {
//   memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
//   Serial.println("\nReceived message:");
//   Serial.printf("From: %s\n", incomingData.sender);
//   Serial.printf("Message: %f\n", incomingData.x);
//   Serial.printf("Value: %d\n", incomingData.value);
// }

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // UART

  WiFi.mode(WIFI_STA); // Station mode

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataReceive);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Đọc dữ liệu từ UART và gửi qua ESP-NOW
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');

    esp_now_send(peerAddress, (uint8_t *)data.c_str(), data.length());
    Serial.println("Sent UART to ESP-NOW: " + data);
  }

  // Gửi dữ liệu từ terminal PC sang ESP-NOW
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    esp_now_send(peerAddress, (uint8_t *)data.c_str(), data.length());
    Serial.println("Sent Terminal to ESP-NOW: " + data);
  }
}

// void loop() {
//   strcpy(outgoingData.sender, "thang"); 
//   outgoingData.x = 15.67;
//   outgoingData.value = random(0, 100);
    
//   esp_err_t result = esp_now_send(peerAddress, (uint8_t *)&outgoingData, sizeof(outgoingData));

//   if (result == ESP_OK) {
//     Serial.println("Sent message successfully");
//   } else {
//     Serial.println("Error sending the data");
//   }
//   delay(1000);
// }
