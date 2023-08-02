#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED 2
#define DHTPIN 27  
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Your SSID";
const char* password = "Your Password";

// Konfigurasi MQTT
const char* mqtt_server = "xxxx.xxxx.xxxx.xxxx";
const int mqtt_port = 1883;

// char message[200]; // Menampung pesan dari subscribed topics
// int messageIndex = 0; // Indeks untuk memasukkan karakter ke dalam message

//cek ip address broker, cek availability ip yg tersedia
IPAddress local_IP(xxxx,xxxx,xxxx,xxxx);
IPAddress gateway(xxxx,xxxx,xxxx,xxxx);
IPAddress subnet(xxxx,xxxx,xxxx,xxxx);
IPAddress primaryDNS(xxxx,xxxx,xxxx,xxxx); //optional
IPAddress secondaryDNS(xxxx,xxxx,xxxx,xxxx); //optional

WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
int value = 0;


void setup()
{
  Serial.begin(115200);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());

  // Menghubungkan ke server MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
  
  while (!mqttClient.connected()) {
    String client_id="ESP32Client-"+WiFi.macAddress();
    if (mqttClient.connect(client_id.c_str())) {
      Serial.printf("connected to MQTT broker with client id %s",client_id.c_str());
      Serial.println();
    } else {
      Serial.println("Gagal terhubung ke server MQTT, coba lagi dalam 5 detik...");
      delay(5000);
    }
  }

  pinMode(LED,OUTPUT);
  dht.begin();

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.println(". Message: ");
  // String message;
  // for (int i=0;i<length;i++) {
  //   Serial.print((char)payload[i]);
  //   message += (char)payload[i];
  // }
  // Serial.println(message);
   
  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  // if (String(topic) == "LED") {
  //   Serial.print("Changing output to ");
  //   if(message == "on"){
  //     Serial.println("on");
  //     digitalWrite(LED, HIGH);
  //   }
  //   else if(message == "off"){
  //     Serial.println("off");
  //     digitalWrite(LED, LOW);
  //   }
  // }
  // else if(String(topic) == "dht22"){
  //   Serial.println(message);
  // }
    
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String client_id="ESP32Client-"+WiFi.macAddress();
    if (mqttClient.connect(client_id.c_str())) {
      Serial.printf("connected to MQTT broker with client id %s",client_id.c_str());
      // Subscribe
      mqttClient.subscribe("dht22");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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
  
  // Membuat objek JSON dan mengisi data
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["sensor"] = "DHT22";
  jsonDoc["temperature"] = String(t);
  jsonDoc["humidity"] = String(h, 2);
  jsonDoc["heatIndex"] = String(hic, 2);

  // Konversi objek JSON ke string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
      // Mengirim data ke MQTT Broker
    char topic[] = "dht22";
    mqttClient.publish(topic, jsonString.c_str());
    //print message to serial monitor
    Serial.println(jsonString.c_str());
  }
}