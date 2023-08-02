/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <WiFi.h>
// #include <DHT.h>
#include <PubSubClient.h>

const char* ssid     = "Your SSID";
const char* password = "Your Password";

// Update these with values suitable for your network.
IPAddress ip(xxxx,xxxx, xxxx, xxxx);
IPAddress server(xxxx,xxxx, xxxx, xxxx);
IPAddress gateway(xxxx,xxxx, xxxx, xxxx;
IPAddress subnet(xxxx,xxxx, xxxx, xxxx);
IPAddress primaryDNS(xxxx,xxxx, xxxx, xxxx); //optional
IPAddress secondaryDNS(xxxx,xxxx, xxxx, xxxx); //optional

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test","Testing");
      Serial.print("Publish Mqtt Server With Topic : test & message : Testing");
      // ... and resubscribe
      client.subscribe("test");
      client.setCallback(callback);
  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  if (!WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  client.setServer(server, 1883);
  client.setCallback(callback);

  WiFi.begin(ssid, password);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
