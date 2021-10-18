#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#endif
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                              // firebase library
#include "CO2Sensor.h"

#include <Wire.h>
int relay=14;
int normal_level=600;
CO2Sensor co2Sensor(A0, 0.99, 100);
#define FIREBASE_HOST "airqon-iot.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "Bo0nrN6iaW6rTBmwncc8Fnqqq8HWVYTNKWSUG3h1"            // the secret key generated from firebase

#define WIFI_SSID "AirQon"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "airqon1234"                                    //password of wifi ssid

                                                   

void setup() {
   u8g2.begin();
  co2Sensor.calibrate();
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  pinMode(relay, OUTPUT);// connect to firebase                                                              //Start reading dht sensor
}

void loop() { 
  
int co2_level = co2Sensor.read(); 
if((co2_level)>500){
  digitalWrite(relay, HIGH);
  }else{
    digitalWrite(relay,LOW);
    }

  Serial.print("Co2 Value is : ");
  Serial.print(co2_level);
  Serial.println(" ");
  delay(1000);
  
int  lastco2=co2_level;
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_profont17_mr); // choose a suitable fontu8g2_font_ncenB08_tr
  u8g2.drawStr(0,11,"Co2 in ppm:");
  u8g2.setCursor(0,31);
  u8g2.print(co2_level);
  u8g2.sendBuffer();          // transfer internal memory to the display
      
 String fireco2 = String(co2_level) + String("ppm"); 
  
  Firebase.pushString("/AirQon/Co2", fireco2); 
  //delay(15000);
//setup path and send readings
  //Firebase.pushString("/DHT11/Temperature", fireTemp);    
  //setup path and send readings
   
}
