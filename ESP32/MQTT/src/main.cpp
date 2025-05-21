#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "rm3p";
const char* password = "0987654321";

const char* mqtt_server = "95a91bb489de4e7ebd91f5707f3c8ce9.s1.eu.hivemq.cloud"; 
const int mqtt_port = 8883;
const char* mqtt_user = "esp32mqtt";
const char* mqtt_password = "Thuchanh4";

const char* publish_topic_lux = "esp32/bh1750";
const char* publish_topic_hum = "esp32/dht111";
const char* publish_topic_temp = "esp32/dht112";
const char* subscribe_topic = "esp32/control";

BH1750 lightMeter(0x23);
WiFiClientSecure espClient;
PubSubClient client(espClient);

#define LED_PIN 23

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Nhận lệnh MQTT: ");
  Serial.println(message);

  if (message == "on") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED BẬT");
  } else if (message == "off") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED TẮT");
  }
}

void setup_wifi() {
  Serial.print("Kết nối WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("OK");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Đang kết nối MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("success");
      client.subscribe(subscribe_topic);
    } else {
      Serial.println("false");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(21,22);
  lightMeter.begin();
  dht.begin();

  setup_wifi();

  espClient.setInsecure();  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

unsigned long lastSend = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();

  if (now - lastSend >= 3000) {
    lastSend = now;

    float lux = lightMeter.readLightLevel();
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    String payload_lux =  String(lux, 2);
String payload_temp = String(temp, 1);
String payload_hum =  String(hum, 1);

client.publish(publish_topic_lux, payload_lux.c_str());
client.publish(publish_topic_temp, payload_temp.c_str());
client.publish(publish_topic_hum, payload_hum.c_str());

Serial.println("Đã gửi: " + payload_lux);
Serial.println("Đã gửi: " + payload_temp);
Serial.println("Đã gửi: " + payload_hum);
  }
}
