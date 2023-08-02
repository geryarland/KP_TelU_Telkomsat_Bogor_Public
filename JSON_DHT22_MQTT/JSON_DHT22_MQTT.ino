#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DHTPIN 27     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi jaringan
const char* ssid = "Enter Your SSID";
const char* password = "Enter Your Password";

// Konfigurasi MQTT
const char* mqtt_server = "192.168.84.103";
const int mqtt_port = 1883;
const char* mqtt_topic = "your_topic";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

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
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
  
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Terhubung ke server MQTT");
    } else {
      Serial.println("Gagal terhubung ke server MQTT, coba lagi dalam 5 detik...");
      delay(5000);
    }
  }

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f) || isnan(hic)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  

  //Mencentak untuk Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));

  // Membuat objek JSON dan mengisi data
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["sensor"] = "DHT22";
  jsonDoc["temperature"] = String(t);
  jsonDoc["humidity"] = String(h, 2);
  jsonDoc["heatIndex"] = String(hic, 2);

  // Konversi objek JSON ke string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Mengirim data ke MQTT Broker
  char topic[] = "your_topic";
  mqttClient.publish(topic, jsonString.c_str());

  delay(5000); // Interval pengiriman data (dalam milidetik)
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Callback kosong, tidak ada aksi khusus
}
