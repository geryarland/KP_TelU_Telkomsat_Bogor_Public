#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"

#define LED 2
#define DHTPIN 27  
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// Konfigurasi MQTT
const char* mqtt_server = "MQTT Server";
const int mqtt_port = 1883;

//cek ip address broker, cek availability ip yg tersedia
IPAddress local_IP(xx, xx, xx, xx);
IPAddress gateway(xx, xx, xx, xx);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(x, x, x, x); //optional
IPAddress secondaryDNS(x, x, x, x); //optional


WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char message[150];
int value = 0;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <button class="button" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

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
    String client_id="ESP32-Client-" + String(WiFi.macAddress());
    if (mqttClient.connect(client_id.c_str())) {
      Serial.printf("Terhubung ke server MQTT with client ID %s",client_id.c_str() );
      // Subscribe ke topik "LED"
      mqttClient.subscribe("LED"); 
      // Subscribe ke topik "dht11"
      mqttClient.subscribe("dht11");
    } else {
      Serial.println("Gagal terhubung ke server MQTT, coba lagi dalam 5 detik...");
      delay(5000);
    }
  }

  pinMode(LED,OUTPUT);
  dht.begin();
  //running web server (simple)
   // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    mqttClient.publish("LED", "on"); 
    request->send(200, "text/plain", "Turn on LED");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    mqttClient.publish("LED", "off"); 
    request->send(200, "text/plain", "Turn off LED");
  });
  
  server.onNotFound(notFound);
  server.begin();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("\nMessage arrived on topic: ");
  Serial.print(topic);
  Serial.println(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  
  // If a message is received on the topic LED, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "LED") {
    Serial.printf("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(LED, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(LED, LOW);
    }
  }
  else if(String(topic) == "dht11"){
    Serial.println(messageTemp);
  }
    
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
   String client_id="ESP32-Client-" + String(WiFi.macAddress());
    if (mqttClient.connect(client_id.c_str())) {
      Serial.printf("Terhubung ke server MQTT with client ID %s",client_id.c_str() );
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
  jsonDoc["sensor"] = "DHT11";
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
    char topic[] = "dht11";
    mqttClient.publish(topic, jsonString.c_str());
    //print message to serial monitor
    Serial.printf("Publish to MQTT broker with topic dht11 & message: %s",jsonString.c_str());
  }
}