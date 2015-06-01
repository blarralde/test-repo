////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////WIFI CONFIG/////////////////////////////////////////////////////
#include <LTask.h>                  //WIFI
#include <LWiFi.h>                  //WIFI
#include <LWiFiServer.h>            //WIFI
#include <LWiFiClient.h>            //WIFI
#define WIFI_AP "BACK_HOME_1"
#define WIFI_PASSWORD "556890556890"
#define WIFI_AUTH LWIFI_WPA
LWiFiServer server(80);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////I2C BUS CONFIG//////////////////////////////////////////////////
#include <Wire.h>                   //I2C BUS
#include <LiquidCrystal_I2C.h>      //I2C LCD
#include <PortExpander_I2C.h>       //PCF8574
#include <RTClib.h>                 //RTC TINTY Clock

LiquidCrystal_I2C lcd1(0x25,20,4);      // set the LCD1 address to 0x25 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd2(0x26,20,4);      // set the LCD2 address to 0x26 for a 20 chars and 4 line display
PortExpander_I2C relais_board_1(0x3e);  //3e
RTC_DS1307 RTC;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Temp Sensors////////////////////////////////////////////////////
const int pinTempSensor1 = A1;          // port A1
const int pinTempSensor2 = A2;          // port A2

int Temp_sens_1;                    // read wert von temperatur sensor 1
int Temp_sens_2;                    // read wert von temperatur sensor 2
float temperature_1;                //rechnung T wert 1
float temperature_2;                //rechnung T wert 2
int B=3975;                         //B value of the thermistor
float resistance_1;                 //rechnung R wert 1
float resistance_2;                 //rechnung R wert 2
char buff[256];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////IR Sensor///////////////////////////////////////////////////////
#define IR_PROXIMITY_SENSOR A0 // Analog input pin that  is attached to the sensor
#define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the seeeduino
         //board switches to 3V3, the ADC_REF should be 3.3
float voltage;//the sensor voltage, you can calculate or find the distance
        // to the reflective object according to the figures
        //on page 4 or page 5 of the datasheet of the GP2Y0A21YK.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////DHT11///////////////////////////////////////////////////////////
#include <dht11.h>                  //DHT11

dht11 DHT11_3;
#define DHT11PIN3 7                     // port D7
dht11 DHT11_4;
#define DHT11PIN4 4                     // port D4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Battery/////////////////////////////////////////////////////////
#include <LBattery.h>               //Battery
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////CONFIGS/////////////////////////////////////////////////////////
  float modus = 0;                                                         // 0= AUTO; 1= MANUAL;
  float rainDelayOFF = 7200;                                               // Aus Dauer
  float rainDelayON = 120;                                                 // Ein Dauer
  float Hmin =70;                                                          // Humindity minimum
  float Tmin = 25;                                                         // maximale temperature
  float Tmax = 30;                                                         // minimale temperature
  float thermomax = 40;                                                    // maximale temperature für heizmatten
  float thermomin = 35;                                                    // minimale temperature für heizmatten
  float Lightstart = (6*60*60)+(00*60)+(00);                               // Einschalt Zeit   (06h00)               
  float Lightstop = (23*60*60)+(00*60)+(00);                               // Ausschalt Zeit   (23h00
  float waterlevelmin = 0;                                                 // Wasserlevelminimum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Values//////////////////////////////////////////////////////////  
  float VERSION = 0.61;                                                    // Version
  float ALLESAUS = 0;                                                      // Alle Wasserspiele Aus                  //Don`t edit
  float TIME_NOW = 0;                                                      //aktuelle Zeit                           //Don`t edit
  float Humy = 0;                                                          //aktuelle Humidity                       //Don`t edit
  float Temp = 0;                                                          //aktuelle Temp                           //Don`t edit                
  float rainTimer = 0;                                                     // Rain Timer                             //Don`t edit
  int RAINNEXT = 0;                                                        // DISPLAY VALUE                          //Don`t edit     
  float DHT11_H3_VALUE = 0;                                                // DISPLAY VALUE                          //Don`t edit
  float DHT11_H3_STATS = 0;                                                // DISPLAY VALUE                          //Don`t edit
  float DHT11_T3_VALUE = 0;                                                // DISPLAY VALUE                          //Don`t edit
  float DHT11_H4_VALUE = 0;                                                // DISPLAY VALUE                          //Don`t edit
  float DHT11_H4_STATS = 0;                                                // DISPLAY VALUE                          //Don`t edit
  float DHT11_T4_VALUE = 0;                                                // DISPLAY VALUE                          //Don`t edit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////Linkit One Version/////////////////////////////////////////////////
#include "vmpromng.h"
   
  VMCHAR hostVersion[100]   = {0};
  VMCHAR buildDateTime[100] = {0};
   
  boolean getVersion(void* userdata)
  {
      // Retrieve version info string
      vm_get_sys_property(MRE_SYS_HOST_VERSION,  hostVersion, sizeof(hostVersion));
      vm_get_sys_property(MRE_SYS_BUILD_DATE_TIME,  buildDateTime, sizeof(buildDateTime));
       
      // Allow remoteCall to return
      return true;
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////STORAGE/////////////////////////////////////////////////////////  
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h> 

// #define Drv LFlash          // use Internal 10M Flash
#define Drv LSD           // use SD card
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////BOUTTONS///////////////////////////////////////////////////////////
String readString; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










































////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////void setup////////////////////////////////////////////////////////////////
void setup() 
{
  
  //////////////////////////////////////////////////////WIFI////////////////////////////////////////////////////////////////////
  
  LTask.begin();                    // initialize the WIFI
  LWiFi.begin();                    // initialize the WIFI
  Serial.begin(115200);
  //while(!Serial.available()); // until something input
  //while(Serial.available())Serial.read(); // clear serial data

  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(1000);
  }

  Serial.println("Start Server");
  server.begin();
  Serial.println("Server Started");
  
  //////////////////////////////////////////////////////I2C BUS/////////////////////////////////////////////////////////////////
  
  Wire.begin();                     // initialize I2C 
  RTC.begin();                      // initialize RTC
  lcd1.init();                      // initialize the lcd1 
  lcd2.init();                      // initialize the lcd2
  
  if(! RTC.isrunning())
  {
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  // Relais Board 1
  relais_board_1.init();
  relais_board_1.digitalWrite(0, HIGH);    // turn the LED off --> 230V Relais (NOT IN USE)
  relais_board_1.digitalWrite(1, HIGH);    // turn the LED off --> Heizung 1 
  relais_board_1.digitalWrite(2, HIGH);    // turn the LED off --> Wasserfallpumpe
  relais_board_1.digitalWrite(3, HIGH);    // turn the LED off --> Wasserheizung
  relais_board_1.digitalWrite(4, HIGH);    // turn the LED off --> Nebler
  relais_board_1.digitalWrite(5, HIGH);    // turn the LED off --> Regenpumpe
  relais_board_1.digitalWrite(6, HIGH);    // turn the LED off --> Heizung 2
  relais_board_1.digitalWrite(7, HIGH);    // turn the LED off --> Licht

  delay(1000);

  // Print a message to the LCD1.
  lcd1.backlight();
  lcd1.setCursor(3,0);
  lcd1.print("Hello, world!");
  lcd1.setCursor(5,1);
  lcd1.print("Display 1!");
  lcd1.setCursor(3,2);
  lcd1.print("Terrarium V");
  lcd1.setCursor(14,2);
  lcd1.print(VERSION);
  lcd1.setCursor(2,3);
  lcd1.print("By Djpredator69!");


  // Print a message to the LCD2.
  lcd2.backlight();
  lcd2.setCursor(3,0);
  lcd2.print("Hello, world!");
  lcd2.setCursor(5,1);
  lcd2.print("Display 2!");
  lcd2.setCursor(3,2);
  lcd2.print("Terrarium V");
  lcd2.setCursor(14,2);
  lcd2.print(VERSION);
  lcd2.setCursor(2,3);
  lcd2.print("By Djpredator69!");

  delay(4000);
  lcd1.clear();      // clear lcd1
  lcd2.clear();      // clear lcd2
  
  printWifiStatus(); //Wifi infos

  lcd2.setCursor(3,1);
  lcd2.print("Server Started!");
  
  delay(4000);
  lcd1.clear();      // clear lcd1
  lcd2.clear();      // clear lcd2

  //////////////////////////////////////////////////////////DHT11///////////////////////////////////////////////////////////////
  
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////Linkit One Version///////////////////////////////////////////////
      LTask.remoteCall(getVersion, NULL);
      Serial.println("Version Info:");
      Serial.println(hostVersion);
      Serial.println(buildDateTime);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////STORAGE///////////////////////////////////////////////////////
  Drv.begin();
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









































////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////Void Loop/////////////////////////////////////////////////////////
void loop() 
{
  if(modus == 0)              //AUTOMATIC
  {
  ////////////////////////////////////////////////////////CALLS/////////////////////////////////////////////////////////////// 
  Timenow();                  // Time
  light();                    // Light 
  temp_humy();                // Temp Humidity
  wasserstand();              // Wasserstand
  rain();                     // Rain
  nebler();                   // Nebler
  wheizung();                 //wandheizung 
  DISPLAYtoLCD();             // Display all values to LCD1 + LCD2
  datalog();                  // Data Log
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  }
  else                        //MANUAL
  {
    
  }  
  
  HTML();                     //Build HTML Page

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









































////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////WIFI////////////////////////////////////////////
void printWifiStatus()
{
   // print your WiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
  lcd1.setCursor(0,0);
  lcd1.print("IP");
  lcd1.setCursor(5,0);
  lcd1.print(ip);

  lcd1.setCursor(0,1);  
  lcd1.print("SubN");
  lcd1.setCursor(5,1);
  lcd1.print(LWiFi.subnetMask());

  lcd1.setCursor(0,2);  
  lcd1.print("GW");
  lcd1.setCursor(5,2);
  lcd1.print(LWiFi.gatewayIP());

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  lcd1.setCursor(0,3);  
  lcd1.print(LWiFi.SSID());
  lcd1.setCursor(17,3);
  lcd1.print(rssi);  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////Time Now////////////////////////////////////////////////
void Timenow()
{
  DateTime now = RTC.now();
  int NOW = (now.hour()*60*60)+(now.minute()*60)+(now.second());     // Aktuelle Zeit 
  
       if(NOW >= 86401){delay(200); Timenow();} 
  else if(NOW == 0){delay(200); Timenow();}
  
  else{TIME_NOW = NOW;}
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////Light////////////////////////////////////////////////////////
void light()
{
  
if((TIME_NOW >= Lightstart) && (TIME_NOW <= Lightstop))
{
relais_board_1.digitalWrite(7, LOW);   //licht ein
lcd1.backlight();                      //LCD 1 ein
lcd2.backlight();                      //LCD 2 ein
}else{
relais_board_1.digitalWrite(7, HIGH);  //licht aus
lcd1.noBacklight();                    //LCD 1 aus
lcd2.noBacklight();                    //LCD 2 aus
}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Temp Humidity///////////////////////////////////////////////////
void temp_humy()
{
  //werte korecktur
  if(DHT11_3.humidity >100){DHT11_3.humidity = 0;}
  if(DHT11_4.humidity >100){DHT11_4.humidity = 0;}
  if(DHT11_3.temperature >50){DHT11_3.temperature = 0;}
  if(DHT11_4.temperature >50){DHT11_4.temperature = 0;}
    
  Humy = ((DHT11_3.humidity)+(DHT11_4.humidity))/2;
  Temp = ((DHT11_3.temperature)+(DHT11_4.temperature))/2;
  
  //LCD TEMP HUMIDITY
  int chk3 = DHT11_3.read(DHT11PIN3);
  switch (chk3)
  { case  0: DHT11_H3_STATS = 1; break;
    case -1: DHT11_H3_STATS = 0; break;
    case -2: DHT11_H3_STATS = 0; break;
    default: DHT11_H3_STATS = 0; break; }
  // Sensor 1
  ///////////////////////////////show this to LCD
  DHT11_H3_VALUE = DHT11_3.humidity;
  DHT11_T3_VALUE = DHT11_3.temperature;
  ///////////////////////////////
  
  int chk4 = DHT11_4.read(DHT11PIN4);
  switch (chk4)
  { case  0: DHT11_H4_STATS = 1; break;
    case -1: DHT11_H4_STATS = 0; break;
    case -2: DHT11_H4_STATS = 0; break;
    default: DHT11_H4_STATS = 0; break; }
  //Sensor 2
  ///////////////////////////////show this to LCD
  DHT11_H4_VALUE = DHT11_4.humidity;
  DHT11_T4_VALUE = DHT11_4.temperature;
  ///////////////////////////////
  
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Wasserstand/////////////////////////////////////////////////////
void wasserstand()
{
  //  Serial.print("sensor voltage  = " );                       
  //   Serial.println(getVoltage()); 
 
  //if (getVoltage() <= waterlevelmin)
  //{
  //relais_board_1.digitalWrite(2, HIGH);                              // turn the LED off --> Wasserfallpumpe
  //relais_board_1.digitalWrite(3, HIGH);                              // turn the LED off --> Wasserheizung
  //ALLESAUS = 1;  
  //}else
  //{
  relais_board_1.digitalWrite(2, LOW);                              // turn the LED on --> Wasserfallpumpe
  relais_board_1.digitalWrite(3, LOW);                              // turn the LED on --> Wasserheizung
  ALLESAUS = 0;
  //}
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////Rain/////////////////////////////////////////////////////////
void rain()
{   
    if(ALLESAUS == 0) // wasserstand 0=OK
    {
      
    if((TIME_NOW <= 86400) && (TIME_NOW >= rainTimer) && (relais_board_1.digitalRead(5) == HIGH ))
    {
      relais_board_1.digitalWrite(5, LOW);                          // turn the LED on --> Regenpumpe
      rainTimer = TIME_NOW + rainDelayON;
    }
    else if((TIME_NOW <= 86400) && (TIME_NOW >= rainTimer) && (relais_board_1.digitalRead(5) == LOW ))
    {
      relais_board_1.digitalWrite(5, HIGH);                         // turn the LED off --> Regenpumpe
      rainTimer = TIME_NOW + rainDelayOFF;
    } 
    
    
    ///////////////////////////////show this to LCD
    RAINNEXT = rainTimer - TIME_NOW;
    ///////////////////////////////
    
    }
    else
    {
    relais_board_1.digitalWrite(5, HIGH);                         // turn the LED off --> Regenpumpe
    }
    
// korektur für die 0 uhr umstellung

    if( (TIME_NOW < 60) && (rainTimer > 86400) && (rainTimer < (86400+rainDelayON)) && (relais_board_1.digitalRead(5) == LOW ))
    {
      rainTimer = rainTimer - 86400;
    }

    if( (TIME_NOW < 60) && (rainTimer > 86400) && (rainTimer < (86400+rainDelayOFF)) && (relais_board_1.digitalRead(5) == HIGH ))
    {
      rainTimer = rainTimer - 86400;
    }
    
    
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Nebler//////////////////////////////////////////////////////////
void nebler()
{
  if(ALLESAUS == 0) // wasserstand 0=OK
  {
    
  if(Humy <= Hmin)                                                              
  {    
     relais_board_1.digitalWrite(4, LOW);                           // turn the LED on --> Nebler
  }else{                                                            
     relais_board_1.digitalWrite(4, HIGH);                          // turn the LED off --> Nebler
  }
  
  }
  else
  {
  relais_board_1.digitalWrite(4, HIGH);                          // turn the LED off --> Nebler
  }
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////Wandheizung/////////////////////////////////////////////////////
void wheizung()
{
  Temp_sens_1=analogRead(pinTempSensor1);
  resistance_1=(float)(1023-Temp_sens_1)*10000/Temp_sens_1; //get the resistance of the sensor;
  temperature_1=1/(log(resistance_1/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

  Temp_sens_2=analogRead(pinTempSensor2);
  resistance_2=(float)(1023-Temp_sens_2)*10000/Temp_sens_2; //get the resistance of the sensor;
  temperature_2=1/(log(resistance_2/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
  
  if(Temp <= Tmin) 
  {
     if(temperature_1 <= thermomin)
     {relais_board_1.digitalWrite(1, LOW);}     // turn the LED on --> Heizung 1 
     if(temperature_1 >= thermomax)
     {relais_board_1.digitalWrite(1, HIGH);}     // turn the LED off --> Heizung 1 
     
     if(temperature_2 <= thermomin)
     {relais_board_1.digitalWrite(6, LOW);}     // turn the LED on --> Heizung 2 
     if(temperature_2 >= thermomax)
     {relais_board_1.digitalWrite(6, HIGH);}     // turn the LED off --> Heizung 2  
  }
  
  if(Temp >=Tmax)
  {      
     relais_board_1.digitalWrite(1, HIGH);    // turn the LED off --> Heizung 1 
     relais_board_1.digitalWrite(6, HIGH);    // turn the LED off --> Heizung 2 
  }
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////LCD Display/////////////////////////////////////////////////////
void DISPLAYtoLCD()
{
  
    lcd1.clear();      // clear lcd1
    lcd2.clear();      // clear lcd2
  
  ///////////////////////////////////////////////Temp and Humidity Sensor 1
  lcd1.setCursor(9, 0);
  if(DHT11_H3_STATS == 1)
  {
  lcd1.print("OK");
  }else{
  lcd1.print("XX");
  }
  lcd1.setCursor(0, 0);
  lcd1.print("H:");
  lcd1.setCursor(2, 0);
  lcd1.print(DHT11_H3_VALUE);
  lcd1.setCursor(7, 0);
  lcd1.print("%");
  lcd1.setCursor(12, 0);
  lcd1.print("T:");
  lcd1.setCursor(14, 0);
  lcd1.print(DHT11_T3_VALUE);
  lcd1.setCursor(19, 0);
  lcd1.print("C"); 
  ///////////////////////////////////////////////Temp and Humidity Sensor 2
  lcd1.setCursor(9, 1);
  if(DHT11_H4_STATS == 1)
  {
  lcd1.print("OK");
  }else{
  lcd1.print("XX");
  }
  lcd1.setCursor(0, 1);
  lcd1.print("H:");
  lcd1.setCursor(2, 1);
  lcd1.print(DHT11_H4_VALUE);
  lcd1.setCursor(7, 1);
  lcd1.print("%");
  lcd1.setCursor(12, 1);
  lcd1.print("T:");
  lcd1.setCursor(14, 1);
  lcd1.print(DHT11_T4_VALUE);
  lcd1.setCursor(19, 1);
  lcd1.print("C");
  ///////////////////////////////////////////////Next Rain delay
  lcd1.setCursor(0, 2);
  lcd1.print("Rain next: ");
  lcd1.setCursor(12, 2);
  lcd1.print(RAINNEXT);
  ///////////////////////////////////////////////Battery  
  lcd1.setCursor(0, 3);
  lcd1.print("Battery");
     if(LBattery.isCharging() == 1){ //Battery load
     lcd1.setCursor(8, 3);
     lcd1.print("LOADING ");  
     }else{
     lcd1.setCursor(8, 3);
     lcd1.print("IN USE ");    
     }
  lcd1.setCursor(16, 3);
  lcd1.print(LBattery.level());
  lcd1.setCursor(19, 3);
  lcd1.print("%");
  ///////////////////////////////////////////////Clock
  DateTime now = RTC.now();
  lcd2.setCursor(0, 0);
  lcd2.print(now.hour(), DEC);
  lcd2.setCursor(2, 0);
  lcd2.print(":");
  lcd2.setCursor(3, 0);
  lcd2.print(now.minute(), DEC);
  lcd2.setCursor(5, 0);
  lcd2.print(":");
  lcd2.setCursor(6, 0);
  lcd2.print(now.second(), DEC);
  lcd2.setCursor(8, 0);
  lcd2.print("  ");
  lcd2.setCursor(10, 0);
  lcd2.print(now.day(), DEC);
  lcd2.setCursor(15, 0);
  lcd2.print("/");
  lcd2.setCursor(13, 0);
  lcd2.print(now.month(), DEC); 
  lcd2.setCursor(12, 0);
  lcd2.print("/");
  lcd2.setCursor(16, 0);
  lcd2.print(now.year(), DEC);
  ///////////////////////////////////////////////Wasserstand
  lcd2.setCursor(0, 1);
  lcd2.print("Wasserstand :");
  lcd2.setCursor(0, 2);
  lcd2.print(getVoltage());
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////////////////////////////////IR Sensor/////////////////////////////////////////////////////////////// 
float getVoltage()
{
  int sensor_value;
  int sum;  
  // read the analog in value:
  for (int i = 0;i < 20;i ++)//Continuous sampling 20 times
  {
    sensor_value = analogRead(IR_PROXIMITY_SENSOR); 
    sum += sensor_value;
  }
  sensor_value = sum / 20;
  float voltage;
  voltage = (float)sensor_value*ADC_REF/1024;
  return voltage;
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////DATALOG/////////////////////////////////////////////////////////
void datalog()
{
  DateTime now = RTC.now();
  // make a string for assembling the data to log:
  String dataString = "";
 

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  LFile dataFile = Drv.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
//Date Time    
    dataFile.print("DATE TIME : ");
    dataFile.println("");
    dataFile.print(String(now.hour(), DEC));
    dataFile.print(":");
    dataFile.print(String(now.minute(), DEC));
    dataFile.print(":");
    dataFile.print(String(now.second(), DEC));
    dataFile.print(" - ");
    dataFile.print(String(now.day(), DEC));
    dataFile.print("/");
    dataFile.print(String(now.month(), DEC));
    dataFile.print("/");
    dataFile.print(String(now.year(), DEC));
    dataFile.println("");
//Wasserstand      
    dataFile.print("Wasserstand : ");
    dataFile.print(String(getVoltage()));
    dataFile.println(",");
//DHT11 sensor 1    
    dataFile.print("Humidity 1 : ");
    dataFile.print(String(DHT11_H3_VALUE));
    dataFile.println("");
    
    dataFile.print("Temp 1 : ");
    dataFile.print(String(DHT11_T3_VALUE));
    dataFile.println("");
//DHT11 sensor 2    
    dataFile.print("Humidity 2 : ");
    dataFile.print(String(DHT11_H4_VALUE));
    dataFile.println("");
    
    dataFile.print("Temp 2 : ");
    dataFile.print(String(DHT11_T4_VALUE));
    dataFile.println("");
//Rain next    
    dataFile.print("Rain next : ");
    dataFile.print(String(RAINNEXT));
    dataFile.println("");
//Battery    
    dataFile.print("Battery : ");
    if(LBattery.isCharging() == 1){ //Battery load
     dataFile.print(String("LOADING "));
     }else{
     dataFile.print(String("IN USE "));
     }
    dataFile.print(String(LBattery.level()));
    dataFile.print("%");
    dataFile.println("");
//rainTimer    
    dataFile.print("rainTimer : ");
    dataFile.print(String(rainTimer));
    dataFile.println("");
//TIME_NOW    
    dataFile.print("TIME_NOW: ");
    dataFile.print(String(TIME_NOW));
    dataFile.println("");
//lerre zeilen    
    dataFile.println("");
    dataFile.println("");
    
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////HTML////////////////////////////////////////////////////////////
void HTML()
{
  ////////////////////////////////////////////////////WIFI//////////////////////////////////////////////////////////////////////
  
  LWiFiClient client = server.available();
  if (client)
  {
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    unsigned long timer_out = millis();
    
    while (client.connected())
    {
       if(millis()-timer_out > 5000)break;
            
      if (client.available())
      {
        // we basically ignores client request, but wait for HTTP request end
       // int c = client.read();
       // Serial.print((char)c);
       
       
       char c = client.read();

         //read char by char HTTP request
         if (readString.length() < 100)
         {
           //store characters to string 
           readString += c; 
           //Serial.print(c);
         } 
       

        if (c == '\n' && currentLineIsBlank)
        {
       //   Serial.println("send response");
          // send a standard http response header
         // Serial.println(readString); //print to serial monitor for debuging
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          if(modus==0)//AUTOMATIC
          {client.println("Refresh: 5");}  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<HEAD>");
          client.println("<TITLE>Terrarium</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          
          
          client.println("<H1 align=\"center\">Terrarium</H1>");
          
          client.println("<table border=1 align=\"center\" width=\"600\">");
          
          
          
//CLOCK          
          DateTime now = RTC.now();
          client.print("<tr><td colspan=2 align=\"center\">");
               client.print(now.hour(), DEC);
               client.print(":");
               client.print(now.minute(), DEC);
               client.print(":");
               client.print(now.second(), DEC);
               client.print(" - ");
               client.print(now.day(), DEC);
               client.print("/");
               client.print(now.month(), DEC);
               client.print("/");
               client.print(now.year(), DEC);
               client.print("</td>");
               if(modus==0)
               {
               client.print("<td align=\"center\">");
               client.print("AUTOMATIC");
               client.print("</td>");
               }
               else
               {
               client.print("<td align=\"center\">");
               client.print("MANUAL");
               client.print("</td>");
               }
               client.println("</tr>");
          
//WASSERSTAND          
          client.print("<tr><td align=\"center\">");
               client.print("Wasserstand : ");
               client.print("</td><td align=\"center\">");
               client.print(getVoltage());
               client.print(" mV");
               client.print("</td><td align=\"center\">");
               client.print("Zustand :");
               if(ALLESAUS == 0) // wasserstand 0=OK
               {
                 client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
                 client.print("OK");
               }else{
                 client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
                 client.print("NOT OK");
               }
               client.println("</td></tr>");
               
//TEMP HUMY               
          client.print("<tr><td align=\"center\">");
               client.print("Temperatur :");
               client.print("</td>");
               if(Temp > Tmin && Temp < Tmax) // wasserstand 0=OK
               {
                 client.print("<td bgcolor=\"#00FF00\" align=\"center\">");
                 client.print(Temp);
                 client.print(" °C");
               }else{
                 client.print("<td bgcolor=\"#FF0000\" align=\"center\">");
                 client.print(Temp);
                 client.print(" °C");
               }
               client.print("</td><td align=\"center\">");
               client.print("Humidity :");
               client.print("</td>");
               if(Humy > Hmin) // wasserstand 0=OK
               {
                 client.print("<td bgcolor=\"#00FF00\" align=\"center\">");
                 client.print(Humy);
                 client.print(" %");
               }else{
                 client.print("<td bgcolor=\"#FF0000\" align=\"center\">");
                 client.print(Humy);
                 client.print(" %");
               }
               client.println("</td></tr>");
              
//RELAISBOARD R1            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(0) == LOW)
            {
               client.print("Realais 230V ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF1\" >OFF</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }else{
               client.print("Realais 230V ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON1\" >ON</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }
            
//RELAISBOARD R2            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(1) == LOW)
            {
               client.print("Heizung 1 ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF2\" >OFF</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("temp = ");
               client.print(temperature_1);
               client.print(" °C");
               client.println("</td></tr>");
            }else{
               client.print("Heizung 1 ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON2\" >ON</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("temp = ");
               client.print(temperature_1);
               client.print(" °C");
               client.println("</td></tr>");
            }
            
//RELAISBOARD R3            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(2) == LOW)
            {
               client.print("Wasserfallpumpe ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF3\" >OFF</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }else{
               client.print("Wasserfallpumpe ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON3\" >ON</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }
            
//RELAISBOARD R4            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(3) == LOW)
            {
               client.print("Wasserheizung ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF4\" >OFF</a>"); 
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }else{
               client.print("Wasserheizung ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON4\" >ON</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            } 
            
//RELAISBOARD R5            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(4) == LOW)
            {
               client.print("Nebler ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF5\" >OFF</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }else{
               client.print("Nebler ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON5\" >ON</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }  
            
//RELAISBOARD R6            
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(5) == LOW)
            {
               client.print("Regenpumpe ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF6\" >OFF</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("Ausschalten in : ");
               client.print(RAINNEXT);
               client.print(" s");
               client.println("</td></tr>");
            }else{
               client.print("Regenpumpe ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON6\" >ON</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("Einschalten in : ");
               client.print(RAINNEXT);
               client.print(" s");
               client.println("</td></tr>");
            }  
          
//RELAISBOARD R7          
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(6) == LOW)
            {
               client.print("Heizung 2 ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF7\" >OFF</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("temp = ");
               client.print(temperature_2);
               client.print(" °C");
               client.println("</td></tr>");
            }else{
               client.print("Heizung 2 ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON7\" >ON</a>");
               }
               client.print("</td><td align=\"center\">");
               client.print("temp = ");
               client.print(temperature_2);
               client.print(" °C");
               client.println("</td></tr>");
            }
          
//RELAISBOARD R8          
          client.print("<tr><td>");  
          if (relais_board_1.digitalRead(7) == LOW)
            {
               client.print("Licht ist ");
               client.print("</td><td bgcolor=\"#00FF00\" align=\"center\">");
               client.print("EIN");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?OFF8\" >OFF</a>"); 
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }else{
               client.print("Licht ist ");
               client.print("</td><td bgcolor=\"#FF0000\" align=\"center\">");
               client.print("AUS");
               client.print("</td><td align=\"center\">");
               if(modus==0){client.print("AUTO");}else
               { 
                     client.print("<a href=\"/?ON8\" >ON</a>");
               }
               client.print("</td><td>");
               client.print("");
               client.println("</td></tr>");
            }
          
          
          
          client.println("</table>");
          
          
          client.println("</BODY>");
          client.println("</html>");
          client.println();
          break;
        }
        if (c == '\n')
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);
    // close the connection:
  //  Serial.println("close connection");
    client.stop();
    
    
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  
  /////////////////////////////////////////////////////////MANUAL CONTROL///////////////////////////////////////////////////////
  
  if(modus==1)
  {     
          
// R1
          if(readString.indexOf("ON1") >0)//checks for on
          {
            relais_board_1.digitalWrite(0, LOW);
            Serial.println("Relais 1 On");
          }
          if(readString.indexOf("OFF1") >0)//checks for off
          {
            relais_board_1.digitalWrite(0, HIGH);
            Serial.println("Relais 1 Off");
          }
// R2          
          if(readString.indexOf("ON2") >0)//checks for on
          {
            relais_board_1.digitalWrite(1, LOW);
            Serial.println("Relais 2 On");
          }
          if(readString.indexOf("OFF2") >0)//checks for off
          {
            relais_board_1.digitalWrite(1, HIGH);
            Serial.println("Relais 2 Off");
          }
// R3          
          if(readString.indexOf("ON3") >0)//checks for on
          {
            relais_board_1.digitalWrite(2, LOW);
            Serial.println("Relais 3 On");
          }
          if(readString.indexOf("OFF3") >0)//checks for off
          {
            relais_board_1.digitalWrite(2, HIGH);
            Serial.println("Relais 3 Off");
          }
// R4          
          if(readString.indexOf("ON4") >0)//checks for on
          {
            relais_board_1.digitalWrite(3, LOW);
            Serial.println("Relais 4 On");
          }
          if(readString.indexOf("OFF4") >0)//checks for off
          {
            relais_board_1.digitalWrite(3, HIGH);
            Serial.println("Relais 4 Off");
          }
// R5          
          if(readString.indexOf("ON5") >0)//checks for on
          {
            relais_board_1.digitalWrite(4, LOW);
            Serial.println("Relais 5 On");
          }
          if(readString.indexOf("OFF5") >0)//checks for off
          {
            relais_board_1.digitalWrite(4, HIGH);
            Serial.println("Relais 5 Off");
          }
// R6          
          if(readString.indexOf("ON6") >0)//checks for on
          {
            relais_board_1.digitalWrite(5, LOW);
            Serial.println("Relais 6 On");
          }
          if(readString.indexOf("OFF6") >0)//checks for off
          {
            relais_board_1.digitalWrite(5, HIGH);
            Serial.println("Relais 6 Off");
          }
// R7          
          if(readString.indexOf("ON7") >0)//checks for on
          {
            relais_board_1.digitalWrite(6, LOW);
            Serial.println("Relais 7 On");
          }
          if(readString.indexOf("OFF7") >0)//checks for off
          {
            relais_board_1.digitalWrite(6, HIGH);
            Serial.println("Relais 7 Off");
          }
// R8          
          if(readString.indexOf("ON8") >0)//checks for on
          {
            relais_board_1.digitalWrite(7, LOW);
            Serial.println("Relais 8 On");
          }
          if(readString.indexOf("OFF8") >0)//checks for off
          {
            relais_board_1.digitalWrite(7, HIGH);
            Serial.println("Relais 8 Off");
          }
//clearing string for next read
          Serial.println(readString);
          readString="";
  }        
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
  //  Serial.println("client disconnected");
  }else{
  delay(1000);
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


          
