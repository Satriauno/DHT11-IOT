
#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

String apiKey = "Enter Your Apikey";

const char *ssid = "Name Wifi";
const char *pass = "Enter Your Password";
const char* server = "api.thingspeak.com";

void setup(){
  Serial.begin(115200);
  delay(10);
  dht.begin();
Serial.println("Connecting to ");
  Serial.println(ssid);
WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop()
{ 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h)||isnan(t)){
      Serial.println("Failed to raed from DHT sensor!");
      return;
    }
if(client.connect(server, 80)){
    String postStr= apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";
client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.print("%. Send to Thingspeak.");
  }
  
  client.stop();
  Serial.println("Waiting...");
  delay(1000);
}
