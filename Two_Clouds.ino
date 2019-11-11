
#include <PubSubClient.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>



#define FIREBASE_HOST "iot-nodemcu-f01ec.firebaseio.com"
#define FIREBASE_AUTH "5hamwJ5ekssOcBPhFZ5ar2HhRMERbaoqJEhRDX3p"
#define WIFI_SSID "ESPIOT"
#define WIFI_PASSWORD "12341234"
#define anInput     A0                        //analog feed from MQ135
#define digTrigger   2                        //digital feed from MQ135
#define co2Zero     45                        //calibrated CO2 0 level
#include <ESP8266WiFi.h>
 
char ssid[] = "ESPIOT";   // your network SSID (name) 
char pass[] = "12341234";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = "730762";
const char * myWriteAPIKey = "K67BY27SB99SFCXI";

// Initialize our values
int number1 = 0;
int number2 = random(0,100);
int number3 = random(0,100);
int number4 = random(0,100);
String myStatus = "";
void setup() 
{

  
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

 
}
 
void loop() 
{
int co2now[10];                               //int array for co2 readings
int co2raw = 0;                               //int for raw value of co2
int co2comp = 0;                              //int for compensated co2 
int co2ppm = 0;                               //int for calculated ppm
int zzz = 0;                                  //int for 

for (int x = 0;x<10;x++){                   //samplpe co2 10x over 2 seconds
    co2now[x]=analogRead(A0);
    delay(200);
    
  }

for (int x = 0;x<10;x++){                     //add samples together
    zzz=zzz + co2now[x];
    
  }
  co2raw = zzz/10;                            //divide samples by 10
  co2comp = co2raw - co2Zero;                 //get compensated value
  co2ppm = map(co2comp,0,1023,400,5000);      //map value for atmospheric le
Serial.println(co2ppm);

//int airData = analogRead(airPin);

Firebase.setInt("co2ppm",co2ppm);

      
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(co2ppm);
                             //postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                            
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
