#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 22        
#define DHTTYPE DHT11   // Loại cảm biến DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Lỗi đọc dữ liệu từ DHT11!");
        return;
    }

    Serial.print("Nhiệt độ: ");
    Serial.print(temperature);
    Serial.print(" °C  -  Độ ẩm: ");
    Serial.print(humidity);
    Serial.println(" %");

    delay(1000); // Đọc lại sau 2 giây
}
