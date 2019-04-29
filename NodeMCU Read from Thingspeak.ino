//-----------James.L v1 4.29.19-------------//
//Read from 8 Thingspeak fields// 

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
const char ssid[] = "ssid";  // your network SSID (name)
const char pass[] = "password";   // your network password
int statusCode = 0;
WiFiClient  client;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//---------Channel Details---------//
unsigned long counterChannelNumber = ch id;            // Channel ID
const char * myCounterReadAPIKey = "api key"; // Read API Key
const int FieldNumber1 = 1;
const int FieldNumber2 = 2;
const int FieldNumber3 = 3;
const int FieldNumber4 = 4;
const int FieldNumber5 = 5;
const int FieldNumber6 = 6;
const int FieldNumber7 = 7;
const int FieldNumber8 = 8;
//-------------------------------//

void setup()
{
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Display Test:OK");
  delay(1000);
}

void loop()
{
  //----------------- Network -----------------//
  if (WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to ");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
    delay(1000);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conn.. to Wi-Fi");
    lcd.setCursor(0, 1);
    lcd.print("Succesfully.");
    delay(1000);
    lcd.clear();
  }
  //--------- End of Network connection--------//

  //---------------- Channel 1 ----------------//
  long temp = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" *F   ");
  Serial.print("Temperature: ");
    Serial.print(temp);
  Serial.println(" *F");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(100);
  //-------------- End of Channel 1 -------------//

  //---------------- Channel 2 ----------------//
  long heatidx = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 1);
    lcd.print("Heat Idx: ");
    lcd.print(heatidx);
    lcd.print(" *F  ");
  Serial.print("Heat Index: ");
    Serial.print(heatidx);
    Serial.println(" *F");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(3000);
  //-------------- End of Channel 2 -------------//
  lcd.clear();
  //---------------- Channel 3 ----------------//
  long dewpoint = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 0);
    lcd.print("Dew Point: ");
    lcd.print(dewpoint);
    lcd.print(" *F  ");
  Serial.print("Dew Point: ");
    Serial.print(dewpoint);
  Serial.println(" *F");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(100);
  //-------------- End of Channel 3 -------------//

  //---------------- Channel 4 ----------------//
  long humidity = ThingSpeak.readLongField(counterChannelNumber, FieldNumber4, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print(" %  ");
  Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % ");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(3000);
  //-------------- End of Channel 4 -------------//
    lcd.clear();
  //---------------- Channel 5 ----------------//
  long pressureM = ThingSpeak.readLongField(counterChannelNumber, FieldNumber5, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 0);
    lcd.print("Baro: ");
    lcd.print(pressureM);
    lcd.print(" mBar  ");
  Serial.print("Barometric: ");
    Serial.print(pressureM);
  Serial.println(" milliBar");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(100);
  //-------------- End of Channel 5 -------------//

  //---------------- Channel 6 ----------------//
  long pressureI = ThingSpeak.readLongField(counterChannelNumber, FieldNumber6, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 1);
    lcd.print("Baro: ");
    lcd.print(pressureI);
    lcd.print(" inHG  ");
  Serial.print("Barometric: ");
    Serial.print(pressureI);
    Serial.println(" inHG");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(3000);
  //-------------- End of Channel 6 -------------//
  lcd.clear();
  //---------------- Channel 7 ----------------//
  long light = ThingSpeak.readLongField(counterChannelNumber, FieldNumber7, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print(light);
    lcd.print(" lums  ");
  Serial.print("Light: ");
    Serial.print(light);
  Serial.println(" lums");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(100);
  //-------------- End of Channel 7 -------------//

  //---------------- Channel 8 ----------------//
  long ppm = ThingSpeak.readLongField(counterChannelNumber, FieldNumber8, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    lcd.setCursor(0, 1);
    lcd.print("Air: ");
    lcd.print(ppm);
    lcd.print(" ppm  ");
  Serial.print("Air Quality: ");
    Serial.print(ppm);
    Serial.println(" ppm");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unable to read");
  Serial.println("Unable to read");
    lcd.setCursor(0, 1);
    lcd.print("or No internet!");
  }
  delay(3000);
  Serial.println("..................................");
   
  //-------------- End of Channel 8 -------------//
  
  
  
}
