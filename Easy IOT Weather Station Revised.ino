/*This sketch is a combination of other sketches to Plot data on thingspeak.com using a Nodemcu
https://www.instructables.com/id/Esay-IoT-Weather-Station-With-Multiple-Sensors/
Adafruit BMP 085 replaced with Sparkfun BMP180 library
DHT11 using updated library that includes Heat index calculations and code for Dew point
TSL2561 Sensor and library to replace the LDR
Added  MQ135 sensor and library
Updated sketch file: Easy IOT Weather Station Revised.ino
April 19 2019
Author: James L.
*/
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <SFE_BMP180.h>
#include "TSL2561.h"

#include "MQ135.h"

#include <Wire.h>

#define DHTPIN D3 // what pin we’re connected to
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);
float dP;
float dPF;

TSL2561 tsl(TSL2561_ADDR_FLOAT); 


SFE_BMP180 pressure;

#define ALTITUDE 237.0 // Your locations Altitude in meters

#define ANALOGPIN A0
   
//#define RZERO 1090.34
#define RZERO 796.94
    
MQ135 gasSensor = MQ135(ANALOGPIN);

// replace with your channel’s thingspeak API key,
String apiKey = "API key";                                     
const char* ssid = "ssid";                                  
const char* password = "password";                              

const char* server = "api.thingspeak.com";

WiFiClient client;
//--------------------------setup-------------------------
void setup() {
pinMode(ANALOGPIN, INPUT);  
Serial.begin(115200);
delay(10);

Serial.println();
Serial.println();
Serial.print("\tConnecting to ");
Serial.println(ssid);
Serial.print("\t..........");
Serial.println();
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print("\t.");

}
Serial.println("");
Serial.println("\tWiFi connected");
delay(800);

dht.begin();
if(!pressure.begin()) {
Serial.print("\tFailed to read from BMP180 sensor!!");
while(1);
}
if (!tsl.begin()) {
    Serial.println("\tNo sensor?");
    while (1);
  }
    
  tsl.setGain(TSL2561_GAIN_16X);    //  (for dim situations)
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // (bright light)
  
}


void loop() {
//--------------------------DHT11-----------------------
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  delay(250);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);
  float hiDegC = dht.convertFtoC(hi);
  Serial.println("\tDHT11 ");
  Serial.print("\tHumidity: "); 
  Serial.print(h);
  Serial.println(" %");
  Serial.print("\tTemperature: "); 
  Serial.print(f);
  Serial.print(" *F  ");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("\tHeat index: ");
  Serial.print(hi);
  Serial.print(" *F  ");
  Serial.print(hiDegC);
  Serial.println(" *C ");
  Serial.print("\tDew Point: ");
  dP=(dewPointFast(t, h));
  float dPF=((dP*9)/5)+32;
  Serial.print(dPF);
  Serial.print(" *F ");
  Serial.print(dewPointFast(t, h));
  Serial.println(" *C ");
 // Serial.println(".................");
//--------------------------BMP180------------------------
  char status;
  double T,P,p0,a;

  Serial.println("\tBMP180");
  Serial.print("\tprovided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters  ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  status = pressure.startTemperature();
  if (status != 0)
  delay(status);
  {
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("\tTemperature BMP180: ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.print(" *F  ");
      Serial.print(T,2);
      Serial.println(" *C");
      
      status = pressure.startPressure(3);
      if (status != 0)
      delay(status);
      {
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("\tAbsolute Pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          p0 = pressure.sealevel(P,ALTITUDE); 
          Serial.print("\tRelative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          a = pressure.altitude(P,p0);
          Serial.print("\tComputed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure\n");
      }
     // else Serial.println("error starting pressure\n");
    }
    else Serial.println("error retrieving temperature\n");
  }
 // else Serial.println("error starting temperature\n");
  
 // Serial.println(".................");
 delay(300);

//--------------------------TSL2561-------------------------
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.println("\tTLS2561 ");
  Serial.print("\tIR: ");
  Serial.println(ir);   
  Serial.print("\tFull: "); 
  Serial.println(full);   
  Serial.print("\tVisible: "); 
  Serial.println(full - ir);   
  Serial.print("\tLux: "); 
  Serial.println(tsl.calculateLux(full, ir));
delay(100);

//--------------------------MQ135------------------------- 
  float rzero = gasSensor.getRZero();
  delay(3000);
  Serial.print("\tMQ135 RZERO: ");
  Serial.println(rzero);
  float ppm = gasSensor.getPPM();
  delay(1000);
  Serial.print("\t ppm value : ");
  Serial.println(ppm);

  Serial.println(".................");

//--------------------------thingspeak-------------------------

if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=";
postStr += String(f);
postStr +="&field2=";
postStr += String(hi);
postStr +="&field3=";
postStr += String(dPF);
postStr +="&field4=";
postStr += String(h);
postStr +="&field5=";
postStr += String(P);
postStr +="&field6=";
postStr += String(P*0.0295333727,2);
postStr +="&field7=";
postStr += String(tsl.calculateLux(full, ir));
postStr +="&field8=";
postStr += String(ppm);
postStr += "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n\n\n\n\n\n\n");
client.print(postStr);



}
client.stop();


// thingspeak needs minimum 15 sec delay between updates
delay(20000);
}
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
  double Td = (b * temp) / (a - temp);
  
  return Td;
}