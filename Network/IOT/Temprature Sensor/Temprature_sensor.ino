//Required libraries for the WiFi
#include <SPI.h>
#include <WiFiNINA.h>

//Required libraries for the temprature sensor
#include <OneWire.h>
#include <DallasTemperature.h> 

char ssid[]= "Mi A3";   //  network SSID (name)
char pass[]= "infinity007";   // network passwork
int keyIndex =0;

int status = WL_IDLE_STATUS;  // the WiFi radio's status
WiFiServer server(80); //We setup the server port

// Here the input pin is declared to which the sensor module is connected
#define Temprature_sensor 7

// Here we configure the libraries required for the temprature sensor
OneWire oneWire(Temprature_sensor);          
DallasTemperature sensors(&oneWire);

void setup(){
   // We the initialize serial output
  Serial.begin(9600);

//BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI BEGIN_WIFI
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to the WiFi network named: ");
    
    Serial.println(ssid);

    // connect to WPA/WPA2 network:
    
    status = WiFi.begin(ssid, pass);

    //wait 10 seconds for connection:
    
    delay(10000);
   
  }

  Serial.println("You are connected to the WiFi network.");
  Serial.println("Below is the network information.");

  wifi_information();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




  
//BEGIN_TEMPRATURE_SENSOR  BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR
  // We initialize the temprature sensor
  sensors.begin(); 
  Serial.println("KY-001 temperature measurement");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


server.begin();   //We initialize the server


}



void loop(){
 
 
 // Temperature measurement is started...
    sensors.requestTemperatures();
    // ... and output measured temperature
    Serial.print("Temperature: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" °C");

    delay(1000); // 1s pause until next measurement


    WiFiClient client = server.available();
    if (client) {
      Serial.println("new client");
      String currentLine = "";
      while (client.connected()){
        if (client.available()){
          char c = client.read();
          Serial.write(c);
          if (c=='\n'){
            
            if (currentLine.length() == 0){

              client.print("Temprature: ");
              client.print(sensors.getTempCByIndex(0));
              Serial.println(" °C");

              break;
              } else{
                currentLine = "";
                }
            }else if (c != '\r'){
              currentLine += c;
              }
          }
        }

        client.stop();
        Serial.println("client disconnected");
      }
}





void wifi_information(){
  String ssid = WiFi.SSID();
  IPAddress ip = WiFi.localIP();
  IPAddress subnet = WiFi.subnetMask();
  IPAddress gateway = WiFi.gatewayIP();

  byte mac[6];
  WiFi.macAddress(mac);

  String macAddress = String(mac[5], HEX) + (":") +
                      String(mac[4], HEX) + (":") + String(mac[3], HEX) + (":") +
                      String(mac[3], HEX) + (":") + String(mac[2], HEX) + (":") +
                      String(mac[1], HEX) + (":") + String(mac[0], HEX);

  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("IP: ");
  Serial.println(ip);

  Serial.print("Gateway: ");
  Serial.println(gateway);

  Serial.print("MAC: ");
  Serial.println(macAddress);
  
  }
