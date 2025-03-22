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

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 22       
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Lỗi đọc cảm biến!");
        return;
    }

    Serial.print("Nhiệt độ: "); Serial.print(temp); Serial.println("°C");
    Serial.print("Độ ẩm: "); Serial.print(hum); Serial.println("%");
    delay(2000);
}

