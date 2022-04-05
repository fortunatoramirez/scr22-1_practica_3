#include <Arduino.h>
#include <WiFi.h>
#include <SocketIoClient.h>
#include <ArduinoJson.h>

String id = "20211961";
const char* ssid     = "INFINITUMC2A2_2.4"; 
const char* password = "Dus2qzqyFC";
const char* server = "201.174.122.202"; //Enter server adress
const uint16_t port = 5001; // Enter server port
uint64_t messageTimestamp;
int payload_entero;
SocketIoClient socketIO;
#define ONBOARD_LED  2

//**************************************
//*********** DHT SENSOR  **************
//**************************************
#include <DHT.h>
#define DHTPIN 23     // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/* SENSOR DHT*/
float h=0;
float t=0;
String str;
String str_luz = "OFF";
StaticJsonDocument<256> doc;
bool LIGHT = false;
bool TEMPERATURE = false;
bool HUMIDITY = false;

void setup() {
  pinMode(ONBOARD_LED,OUTPUT); 
  pinMode(15,OUTPUT);
  Serial.begin(115200);
  connectWiFi_STA();
  socketIO.begin(server, port);
  socketIO.on("controlar_ESP",procesar_mensaje_recibido);
  /* SENSOR DHT */
  dht.begin();
}

void loop() {
  socketIO.loop();
  uint64_t now = millis();
  if(now - messageTimestamp > 2000)
  {
      if(TEMPERATURE && !HUMIDITY)
      {
        t = dht.readTemperature();
        if (isnan(t)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+String(t)+", \"humedad\":"+"\"OFF\",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }  
      else if(!TEMPERATURE && HUMIDITY)
      {
        h = dht.readHumidity();
        if (isnan(h)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+"\"OFF\""+", \"humedad\":"+String(h)+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      else if(TEMPERATURE && HUMIDITY)
      {
        t = dht.readTemperature();
        h = dht.readHumidity();
        if (isnan(h) || isnan(t)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+String(t)+", \"humedad\":"+String(h)+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      else if(!TEMPERATURE && !HUMIDITY)
      {
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+"\"OFF\""+", \"humedad\":"+"\"OFF\""+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      messageTimestamp = now; 
    }  
}

void procesar_mensaje_recibido(const char * payload, size_t length) {

 //Serial.printf("Mensaje recibido: %s\n", payload);
 
 auto error = deserializeJson(doc, payload);
 if (error) {
   Serial.print(F("deserializeJson() failed with code "));
   Serial.println(error.c_str());
   return;
 }

 String id_r = doc["id"];
 //Serial.printf("id_r: %s\n",id_r);
 
 if(id_r == id)
 {

   String comm = doc["command"];
   if(comm == "LIGHT_ON")
   {
    digitalWrite(ONBOARD_LED,HIGH);
    digitalWrite(15,HIGH);
    LIGHT = true;
   }
   else if(comm == "LIGHT_OFF")
   {
    digitalWrite(ONBOARD_LED,LOW);
    digitalWrite(15,LOW);
    LIGHT = false;
   }
   else if(comm == "TEMPERATURE_ON")
   {
    TEMPERATURE = true;
   }
   else if(comm == "TEMPERATURE_OFF")
   {
    TEMPERATURE = false;
   }
   else if(comm == "HUMIDITY_ON")
   {
    HUMIDITY = true;
   }
   else if(comm == "HUMIDITY_OFF")
   {
    HUMIDITY = false;
   }
 }
 
}

void connectWiFi_STA()
{
   delay(10);
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     Serial.print('.'); 
   }
   Serial.println("");
   Serial.print("Iniciado STA:\t");
   Serial.println(ssid);
   Serial.print("IP address:\t");
   Serial.println(WiFi.localIP());
}
