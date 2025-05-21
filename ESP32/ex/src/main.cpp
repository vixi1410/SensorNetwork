// #include <BH1750.h>
// #include <Wire.h>

// BH1750 lightMeter(0x23);

// void setup() {

//   Serial.begin(9600);
//   Wire.begin(21,22);
//   lightMeter.begin();
// }
// void loop() {
//   if (lightMeter.measurementReady()) {
//     float lux = lightMeter.readLightLevel();
//     Serial.print("Light: ");
//     Serial.print(lux);
//     Serial.println(" lx");
//     delay(2000);
//   }
// }

///////////////////////////////////////////////////////////////////////

// #include <Adafruit_Sensor.h>
// #include <DHT.h>

// #define DHTPIN 22
// #define DHTTYPE DHT11

// DHT dht(DHTPIN, DHTTYPE);

// // Tạo buffer để chứa dữ liệu
// char buffer[100]; 

// void setup() {
//   Serial.begin(9600);                  // UART0 để debug
//   Serial2.begin(115200);               // UART2 để truyền dữ liệu đi
//   dht.begin();                         // Khởi động cảm biến DHT11
// }

// void loop() {
//   float temp = dht.readTemperature();
//   float hum = dht.readHumidity();

//   if (isnan(temp) || isnan(hum)) {
//     Serial.println("Lỗi đọc cảm biến!");
//     snprintf(buffer, sizeof(buffer), "Loi doc cam bien!\r\n");
//     Serial2.print(buffer);
//     return;
//   }

//   // Ghi dữ liệu vào buffer trước
//   snprintf(buffer, sizeof(buffer), "NHIETDO:%.1fC, DOAM:%.1f%%\r\n", temp, hum);

//   // Gửi qua UART0 (debug)
//   Serial.print(buffer);

//   // Gửi buffer qua UART2
//   Serial2.print(buffer);

//   delay(2000);
// }


///////////////////////////////////////////////////////////////////////

// #include <SPI.h>
// #include <MFRC522.h>
// #define RST_PIN         22         // Chân Reset của RC522 (thay đổi nếu cần)
// #define SDA_PIN         5          // Chân SDA của RC522 (thay đổi nếu cần)

// MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Tạo đối tượng MFRC522 với SDA là pin chip select và RST là chân reset.

// // Hàm khai báo dump_byte_array trước khi sử dụng
// void dump_byte_array(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }

// void setup() {
//   Serial.begin(9600); // Khởi tạo giao tiếp serial với PC
//   while (!Serial);    // Đợi nếu không có cổng serial

//   // Khởi tạo SPI với các chân cho ESP32
//   SPI.begin(18, 19, 23, SDA_PIN);  // SCK=18, MISO=19, MOSI=23, SDA=5 cho ESP32

//   mfrc522.PCD_Init(); // Khởi tạo mô-đun RC522
//   Serial.println(F("RFID Reader Initialized"));
// }

// void loop() {
//   // Kiểm tra có thẻ mới hay không
//   if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
//     Serial.print(F("Card UID:"));
//     dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
//     Serial.println();
    
//     MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
//     Serial.print(F("PICC type: "));
//     Serial.println(mfrc522.PICC_GetTypeName(piccType));

//     // Dừng thẻ và kết thúc mã hóa
//     mfrc522.PICC_HaltA();
//     mfrc522.PCD_StopCrypto1();
//   }
// }

///////////////////////////////////////////////////////////////////////

#include <Arduino.h> 
// Khai báo UART2
#define UART2_TX 23   
#define UART2_RX 22  

void setup() {
  Serial.begin(9600);      
  Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX); 

  Serial.println("ESP32 ready. Type 'a' to send over UART2.");
}

void loop() {
  if (Serial.available()) {
    char incomingChar = Serial.read();

    if (incomingChar == 'a') {
      Serial2.write('a');
      Serial.println("Sent 'a' over UART2");
    }
    else if (incomingChar == 'b') {
      Serial2.write('b');
      Serial.println("Sent 'b' over UART2");
    }
    else {
      Serial.println("Only 'a' or 'b' will be sent.");
    }
  }
}
