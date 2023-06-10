#define BLYNK_TEMPLATE_ID "TMPLSi-0o3tk"
#define BLYNK_TEMPLATE_NAME "Black Box"
#define BLYNK_AUTH_TOKEN "7165VbR_mjrlqkrxS_zj0bCeTlHch2fs"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial


int val = 0;
static const int RXPin = 4, TXPin = 5;   // GPIO 13=D7(conneect Rx of GPS) and GPIO 15=D8(Connect Tx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object
WidgetMap myMap(V6);  // V0 for virtual pin of Map Widget

const int buzzer =  16;         //D0
const int RelayPin = 12;        //D6
int relayOP = 0;  

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;


float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

//char auth[] = "j4uyBBcXvjK0zbJU8ne47wiJY-pyrQ7Q";              //Your Project authentication key
char auth[] = "7165VbR_mjrlqkrxS_zj0bCeTlHch2fs";


char ssid[] = "OnePlus 9R"; //Enter your WIFI name
char pass[] = "psp250800"; //Enter your WIFI password

unsigned int move_index = 1;       // fixed location for now

BLYNK_WRITE(V7) // Executes when the value of virtual pin 7 changes
{
      if(param.asInt() == 1)
      {
        // execute this code if the switch widget is now ON
        digitalWrite(RelayPin,LOW);  
        digitalWrite(buzzer, HIGH);
      }
      else
      {
        // execute this code if the switch widget is now OFF
        digitalWrite(RelayPin,HIGH);
        digitalWrite(buzzer, LOW);
      }
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  ss.begin(GPSBaud);
  timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
  pinMode(buzzer, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
}


void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}

void loop()
{
    while (ss.available() > 0)
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }
   Blynk.run();
   timer.run();
}

void displayInfo()
{
  if (gps.location.isValid() )
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon.
    float longitude = (gps.location.lng());
   
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));  
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);      
   
}

 
 Serial.println();
}
