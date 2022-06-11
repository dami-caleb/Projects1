t//Required libraries for the WiFi
#include <SPI.h>
#include <WiFiNINA.h>

//Required library for MQTT connection
#include <ArduinoMqttClient.h>

//Required libraries for the temprature sensor
#include <OneWire.h>
#include <DallasTemperature.h> 


//Include the header file created that has the WiFi password
#include "secret_passwords.h"

//Pin declearation for the stepper motor (i.e. Fan)
#define STEPPER_PIN_1 6
#define STEPPER_PIN_2 5
#define STEPPER_PIN_3 4
#define STEPPER_PIN_4 3
int step_number = 0;

//Pin declearation for the buzzer (i.e. Alarm) 
int Buzzer = 10;

//Pin declearation for the temprature sensor (i.e. Themometer)
#define Temprature_sensor 2
// Here we configure the libraries required for the temprature sensor
OneWire oneWire(Temprature_sensor);          
DallasTemperature sensors(&oneWire);


//Pin declearation for the HC-SR04 ultrasonic sensor
#define echoPin 8 //Echo pin of HC-SR04
#define trigPin 9 //Trig pin of HC-SR04
//variable declearation
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement




char ssid[]= THE_SSID_NAME;   //  network SSID (name)
char pass[]= THE_SSID_PASSWORD;   // network passwork
int status = WL_IDLE_STATUS;  // the WiFi radio's status

WiFiClient net;
MqttClient mqttClient(net);

const char broker[] = "192.168.158.24"; // Address of the MQTT server
int port = 1883;

//topics
const char smart_heating_topic[]  = "smart_home/smart_heating";
const char fan_topic[]  = "smart_home/fan";   
const char smart_light_topic[]  = "smart_home/smart_light"; 
const char light_bulb_topic[]  = "smart_home/light_bulb"; 
const char theft_topic[]  = "smart_home/theft";
const char off_alarm_topic[]  = "smart_home/off_alarm";

String subMessage = "";

void setup(){
  // We initialize the serial output
  Serial.begin(9600);

//BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION  BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION BEGIN_WIFI_CONNECTION 
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




//BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION BEGIN_MQTT_CONNECTION

mqttClient.setId("Arduino1");
Serial.print("Attempting to connect to the MQTT broker: ");
Serial.println(broker);

while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
  
  
  // subscribed topics
  mqttClient.subscribe(fan_topic);
  mqttClient.subscribe(smart_light_topic);
  mqttClient.subscribe(light_bulb_topic);
  mqttClient.subscribe(theft_topic);
  mqttClient.subscribe(off_alarm_topic);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


fan_setup(); //setup for the fan
buzzer_setup(); //setup for the alaram topic
temprature_sensor_setup(); //setup for the temprature
}


void loop(){
 MQTT_Subscribe();        //This contains the: fan, smart_light, light_bulb, off_alarm topic
 //fan_action(false);
  //delay(2);
  temprature_sensor_action(); //arduino publisehes to the smart_heating topic
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


void MQTT_Subscribe(){
  //We call poll() regularly to allow the library to send MQTT keep alives which
  //avoids being disconnected by the broker
    //mqttClient.poll();

   int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    subMessage = "";
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      subMessage = subMessage + (char)mqttClient.read();
    }
    Serial.println(subMessage);

    while (subMessage == "ON_FAN"){                                   // from: smart_home/fan topic
        Serial.println("Turning on fan");
        fan_action(false);  //check if this works
        delay(2);
        }
      while (subMessage =="OFF_FAN"){                                // from: smart_home/fan topic
        Serial.println("Turning off fan");
        fan_action(true);   //check if this works
        delay(2); 
      }
      while (subMessage == "INDIVIDUAL"){                          // from: smart_home/smart_light topic
        Serial.println("Turning on room light");
      }
        
      while (subMessage == "OFF_LIGHT"){                           //from: smart_home/light_bulb topic
        Serial.println("Turning off room light"); 
      }

      while (subMessage == "INTRUDER_DETECTED"){                  //from: smart_home/theft topic
        Serial.println("Turning on alarm");
        buzzer_action_on();
      } 

      while (subMessage == "NO_INTRUDER"){                        //from: smart_home/off_alarm topic
        Serial.println("Turning off alarm");
        buzzer_action_off();
      }
  }
    
  }



void MQTT_Publisher(const char *topic, const char *message){
    mqttClient.beginMessage(topic);
    mqttClient.print(message);
    mqttClient.endMessage();
    Serial.print("Sent MQTT message to the topic: ");
    Serial.println(topic);
  }





  

//START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN //START_FAN

void fan_setup(){
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  }



void fan_action(bool dir){
    if(dir){
switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
} 
  }else{
    switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
 
  
} 
  }
step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}

//END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN //END_FAN 






//START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER //START_BUZZER 
void buzzer_setup(){
  pinMode (Buzzer, OUTPUT); // Initialize output pin for the buzzer
  }


void buzzer_action_on(){
  digitalWrite (Buzzer, HIGH); // Buzzer is switched on
  delay (4000); // wait mode for 4 seconds
  digitalWrite (Buzzer, LOW); // Buzzer is switched off
  delay (2000); // Wait mode for another two seconds in which the LED is then turned off
  }

void buzzer_action_off(){
  digitalWrite (Buzzer, LOW); // Buzzer is switched off
  }

//END_BUZZER //END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER//END_BUZZER




//BEGIN_TEMPRATURE_SENSOR  BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR BEGIN_TEMPRATURE_SENSOR
//Check for errors
void temprature_sensor_setup(){
  //We initilaize the sensor
  sensors.begin();
  }


void temprature_sensor_action(){
  // Temperature measurement is started...
    sensors.requestTemperatures();
    // ... and output measured temperature
    Serial.print("Temperature: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" °C");
    //Check for errors
    int temprature =  sensors.getTempCByIndex(0);
    if (temprature >= 32){
      //MQTT_Publisher(const char *topic, const char *message)
      MQTT_Publisher("smart_home/smart_heating", "HIGH-TEMP");
      }
    delay(1000); // 1s pause until next measurement

  }


//END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  //END_TEMPRATURE_SENSOR  




//BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR //BEGIN_ULTASONIC_SENSOR 
void Ultrasonic_Sensor_setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // Start Ultrasonic Sensor HC-SR04
  Serial.println("with Arduino Rev Board");
  }

void Ultrasonic_Sensor_action(){
    // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance >= 62){
      //MQTT_Publisher(const char *topic, const char *message)
      MQTT_Publisher("smart_home/theft", "INTRUDER_DETECTED");
      }
    }


//END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  //END_ULTRASONIC_SENSOR  
