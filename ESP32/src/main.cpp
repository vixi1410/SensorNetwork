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

// #include <Adafruit_Sensor.h>
// #include <DHT.h>

// #define DHTPIN 22       
// #define DHTTYPE DHT11  

// DHT dht(DHTPIN, DHTTYPE);

// void setup() {
//     Serial.begin(9600);
//     dht.begin();
// }

// void loop() {
//     float temp = dht.readTemperature();
//     float hum = dht.readHumidity();

//     if (isnan(temp) || isnan(hum)) {
//         Serial.println("Lỗi đọc cảm biến!");
//         return;
//     }

//     Serial.print("Nhiệt độ: "); Serial.print(temp); Serial.println("°C");
//     Serial.print("Độ ẩm: "); Serial.print(hum); Serial.println("%");
//     delay(2000);
// }


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22         // Chân Reset của RC522 (thay đổi nếu cần)
#define SDA_PIN         21          // Chân SDA của RC522 (thay đổi nếu cần)

MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Tạo đối tượng MFRC522 với SDA là pin chip select và RST là chân reset.

// Hàm khai báo dump_byte_array trước khi sử dụng
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void setup() {
  Serial.begin(9600); // Khởi tạo giao tiếp serial với PC
  while (!Serial);    // Đợi nếu không có cổng serial

  // Khởi tạo SPI với các chân cho ESP32
  SPI.begin(18, 19, 23, SDA_PIN);  // SCK=18, MISO=19, MOSI=23, SDA=5 cho ESP32

  mfrc522.PCD_Init(); // Khởi tạo mô-đun RC522
  Serial.println(F("RFID Reader Initialized"));
}

void loop() {
  // Kiểm tra có thẻ mới hay không
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.print(F("PICC type: "));
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Dừng thẻ và kết thúc mã hóa
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

