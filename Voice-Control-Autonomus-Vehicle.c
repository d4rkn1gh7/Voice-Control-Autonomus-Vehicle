#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT <PORT>
#define AIO_KEY "<KEY>"
#define AIO_USERNAME "<USERNAME>"
#define WLAN_SSID "<SSID>"
#define WLAN_PASS "<PASSWORD>"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);
Adafruit_MQTT_Subscribe CarStatus = Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME "/feeds/CarStatus");
int trigger = D6;
int echo = D7;

int obstacle()
{
  digitalWrite(trigger,HIGH);
  delay(1);
  digitalWrite(trigger,LOW);
long  duration = pulseIn(echo,HIGH);
long  distance = (duration/2)/29.1;
  if(distance<1)
  { 
  return 0;
  stop();
  }
  else{
     
  
  return 1;
}
}
int x = obstacle();



void setup() 
{   pinMode(trigger,INPUT);
    Serial.println("In Setup");
    pinMode(echo,OUTPUT);
   Serial.begin(115200);
   WiFi.begin(WLAN_SSID,WLAN_PASS);
   Serial.println(WiFi.localIP());
   mqtt.subscribe(&CarStatus);
   
  
    
}
void loop()
{   Serial.println("In loop");
   MQTT_Connect();
   Adafruit_MQTT_Subscribe *subscription;
   while(subscription = mqtt.readSubscription(5000))
   {
     if(subscription=&CarStatus)
     {
      if(strcmp((char*)CarStatus.lastread,"move forward")==0)
      {
        
        Serial.println("moving forward");
       forward();
      }
      if(strcmp((char*)CarStatus.lastread,"move backward")==0)
      {
        
        Serial.println("moving backward");
       backward(); 
        
      }
      if(strcmp((char*)CarStatus.lastread,"turn left")==0)
      {
        Serial.println("turning left");
        left();
      }
      if(strcmp((char*)CarStatus.lastread,"turn right")==0)
      {
        Serial.println("turning right");
        right();
      }
      
      
     }
   }
    
}
void MQTT_Connect()
{
  int8_t ret;
  if(mqtt.connected())
  {
    return;
  
  }
  Serial.println("Connecting to MQTT...");
  uint8_t retries=3;
  while((ret=mqtt.connect())!=0)
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if(retries==0)
    {
      while(1);
    }
  }
  Serial.println("MQTT Connected");
}
void forward()
{
   while(x)
   {
  Serial.println("Moving forward");
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,HIGH);
  digitalWrite(D5,LOW);
   }
}
void backward()
{
  while(x){
  
  
  Serial.println("Moving backward");
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
  digitalWrite(D5,HIGH);
  
}
}
void left()
{  
  while(x)
{
  Serial.println("Turning left");
  digitalWrite(D2,HIGH);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  digitalWrite(D5,HIGH);
  delay(1000);//waiting for it to turn 90 degrees
  
 
       forward();
      
       
        
      }
}
}
void right()
{ 
  while(x){
  Serial.println("Turning right");
  digitalWrite(D2,LOW);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  digitalWrite(D5,LOW);
  Serial.println("turned right");
  delay(1000);//waiting for it to turn 90degrees
 
   forward();
     
        
      }
  
  
}
}


void stop()
{
  Serial.println("Obstacle detected. Stopped");
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  digitalWrite(D5,LOW);  
  
  
}
