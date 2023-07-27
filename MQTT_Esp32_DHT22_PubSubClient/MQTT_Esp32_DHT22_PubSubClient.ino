#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Konfigurasi jaringan
const char* ssid = "Enter Your SSID";
const char* password = "Enter Your Password";

// Konfigurasi MQTT
const char* mqtt_server = "xxx.xxx.xxx.xxx";
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/dht22";

// Konfigurasi pin sensor DHT22
const int dht_pin = 27;
const int dht_type = DHT22;

DHT dht(dht_pin, dht_type);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi terhubung");
  
  // Menghubungkan ke server MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Terhubung ke server MQTT");
    } else {
      Serial.println("Gagal terhubung ke server MQTT, coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
  
  // Memulai sensor DHT22
  dht.begin();
}

void loop() {
  // Membaca suhu dan kelembaban dari sensor DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Mengirim data suhu ke server MQTT
  char temperatureString[6];
  dtostrf(temperature, 4, 2, temperatureString);
  client.publish(mqtt_topic, temperatureString);

  // Mengirim data kelembaban ke server MQTT
  char humidityString[6];
  dtostrf(humidity, 4, 2, humidityString);
  client.publish(mqtt_topic, humidityString);

  // Menunggu 5 detik sebelum mengirim data lagi
  delay(5000);
}
