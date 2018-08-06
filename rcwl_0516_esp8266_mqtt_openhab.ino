#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//------------------- VARIABLES WIFI --------------------------
#define WLAN_SSID  "MY_SSID"
#define WLAN_PASS  "MY_PASSWORD"

//------------------- VARIABLES MQTT --------------------------
#define mqtt_server   "my_mqtt_server_ip"
#define mqtt_user     "mqtt_user"
#define mqtt_password "mqtt_passwd"
#define sensor_topic   "my_data_topic"
#define registro_topic "my_register_topic"
//------------------- VARIABLES GENERALES --------------------------
WiFiClient espClient;
PubSubClient pubClient(espClient);

#define SensorPin 0
#define LedPin    2

int  delayTime = 1000; 
int  estado    = 0;
bool estado2   = false;

String clientName;

void setup() {
  Serial.begin(115200);
  delay(100);

  setup_wifi();

  pubClient.setServer(mqtt_server, 1883);
    
  // put your setup code here, to run once:
  pinMode(SensorPin, INPUT);
  pinMode(LedPin, OUTPUT);  
}

void loop() {
  
   if (!pubClient.connected()) {
        delay(100);
        reconnect();
    }
    pubClient.loop();
  
  estado = digitalRead(SensorPin);

  String temp = String(estado);
  temp += clientName;
  
  if(digitalRead(SensorPin)==HIGH){
      if(!estado2){
        // Postear datos
        pubClient.publish(sensor_topic, temp.c_str(), true);
        estado2 = true;
        // I'm using a led in the prototype
        digitalWrite(LedPin,digitalRead(SensorPin));
        delay(10000);
      }
   }else{
      if (estado2){
        pubClient.publish(sensor_topic, temp.c_str(), true);
        estado2 = false;
        // I'm using a led in the prototype
        digitalWrite(LedPin,digitalRead(SensorPin));
        delay(3000);
      }
   }
}

void setup_wifi() {
      delay(10);
      // We start by connecting to a WiFi network
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(WLAN_SSID);
    
      WiFi.begin(WLAN_SSID, WLAN_PASS);
    
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
    
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!pubClient.connected()) {
        Serial.println("Attempting MQTT connection...");
    
      clientName = "anything_that_you_want";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);

    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (pubClient.connect( (char*) clientName.c_str() , mqtt_user, mqtt_password) ) {
      Serial.println("connected");
      pubClient.publish(registro_topic, clientName.c_str() );
    } else {
      Serial.print("failed, rc=");
      Serial.print(pubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
  }
  return result;
}
