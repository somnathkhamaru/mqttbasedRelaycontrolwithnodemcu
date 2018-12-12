
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.

const char* ssid = "soms";
const char* password = "sunny007";
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);
int relayInput1 = 5; // the input to the relay pin 1
int relayInput2 = 4; // the input to the relay pin 2
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // decode the JSON payload
  StaticJsonBuffer<128> jsonInBuffer;
  JsonObject& root = jsonInBuffer.parseObject(payload);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  
   root.printTo(Serial);
    Serial.println("");
   int relay1 = root["L1"];
   Serial.println("RELAY 1 new old");
   Serial.println(relay1);
   Serial.println( digitalRead(relayInput1));
   int relay2 = root["L2"];
   Serial.println("RELAY 2 new old");
   Serial.println(relay2);
   Serial.println( digitalRead(relayInput2));
 
    if(relay1==1 && digitalRead(relayInput1)==1)
    {
      Serial.println("L1 ON");
      digitalWrite(relayInput1, LOW);
    }
    if(relay1==0 && digitalRead(relayInput1)==0)
    {
       Serial.println("L1 OFF");
      digitalWrite(relayInput1, HIGH);
    }
    if(relay2==1 && digitalRead(relayInput2)==1)
    {
       Serial.println("L2 ON");
      digitalWrite(relayInput2, LOW);
    }
    if(relay2==0 && digitalRead(relayInput2)==0)
    {
      Serial.println("L2 OFF");
      digitalWrite(relayInput2, HIGH);
    }
    


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("lablocator/light");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(relayInput1, OUTPUT); // initialize Relay pin1 as OUTPUT
  pinMode(relayInput2, OUTPUT);
  digitalWrite(relayInput1, HIGH);
  digitalWrite(relayInput2, HIGH);// initialize Relay pin2 as OUTPUT
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("lablocator/light");
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  }

