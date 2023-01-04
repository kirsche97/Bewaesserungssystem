//#include <Adafruit_VL53L1X.h>
#include <I2CSoilMoistureSensor.h>

#include <WiFi.h>
#include <esp_wpa2.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Preferences.h>
#include <VL53L1X.h>


// NOTE : Below is a String I use to add to the data sent to the Nextion Display
//        And to verify the end of the string for incoming data.
//        Replace 3 x Serial2.write(0xff);
String  endChar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));
String  dfd  = ""; // data from display

//Variablen Daten auf SD-Karte speichern
String formattedDate;
String dayStamp;
String timeStamp;

long aktuelleZeit;
long letzterMessdruchlauf;
long letzerGiessdurchlauf;
long akt_mil;
int akthour;


#define SD_CS 5         // Define CS pin for the SD card module
String dataMessage;
WiFiUDP ntpUDP;         // Define NTP Client to get time
NTPClient timeClient(ntpUDP);
File file;
const char *filename = "/backup.json";
const char *Messdaten= "/Messdaten.txt";
File myFileSDCart;
const int size_Jason = 30000;


// NOTE : General Async Delay 
unsigned	long asyncDelay = 0;// NOTE : 4,294,967,295
int delayLength = 1500;

//Variablen WLAN Verbindung
Preferences preferences;
String SSIDname = "";
String SSIDpassword = "";
boolean start=false;  //Variable für den start nach Stromausfall oder initial bei beginn
int AnzNetwerk = 0;    //Anzahl der Netzwerke
String verfuegbare_Netzwerke = "";

//Variablen Motor
int motorRichtung = 0; //motorRichtung = 0 --> aus; motorRichtung = 1 --> vorwaerts; motorRichtung = 2 --> rueckwärts
unsigned long motorDelay = 0;
int motorDelayLength = 5;

//Variablen Näherungsschalter
int SensorPin_R = 34;     //Näherungsschalter rechts
int SensorPin_L = 35;     //Näherungsschalter links
int Status_R;
int Status_L;

//Variablen Pumpen
#define Steuerpin_Pumpe1 13
#define Steuerpin_Pumpe2 12
#define Steuerpin_Pumpe3 14
#define Steuerpin_Pumpe4 15
#define Steuerpin_Pumpe5 2
#define Steuerpin_Pumpe6 4

//Variablen Seitenwahl
int Seitenwahl = 0;

const int T_max_Anzahl = 999;     //Anzhal maximale Töpfe (max 999) wird für das Struct Array benötigt
int tAnzahl = 0;                  // Tatsächlich vorhandene Anzhal an Töpfen
int test = 0;
int Topfnummer = 0;
int sAnzahl = 0;      
boolean auto_mode=true;
//tatsächliche Anzahl an Sensoren (max. 12)
boolean geladen = false;          //.path nur einmal füllen bei Methode Top()
long aktuellePosition;
int feuchte[12];
int Giessintervall = 1;
int Speicherintervall = 1;



struct Topf
{
  int T_Nummer;
  long Position;
  int minFeuchte;
  int WasserMenge;
  int Duenger;
  int Sensornummer;
  boolean gegossen;     //für Stromausfall
};

Topf Pflanzen[T_max_Anzahl]; //Struct mit dem Namen Pflanzen und der max. Topfanzahl definieren
Topf Kopie[T_max_Anzahl]; //Struct für den Automatikbetrieb. Wird nach der Sensornummer sortiert, damit nicht doppelt oder zu wenig gegossen werden kann

/*
//Variablen Automatikbetrieb
int Distanz=0;
int16_t distance;
Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(40, 42);
*/

I2CSoilMoistureSensor Sensor1;
I2CSoilMoistureSensor Sensor2;
I2CSoilMoistureSensor Sensor3;
I2CSoilMoistureSensor Sensor4;
I2CSoilMoistureSensor Sensor5;
I2CSoilMoistureSensor Sensor6;
I2CSoilMoistureSensor Sensor7;
I2CSoilMoistureSensor Sensor8;
I2CSoilMoistureSensor Sensor9;
I2CSoilMoistureSensor Sensor10;
I2CSoilMoistureSensor Sensor11;
I2CSoilMoistureSensor Sensor12;

VL53L1X distsensor;
long dist;

void setup() 
{
  
  Serial.begin(9600);
  Serial2.begin(9600);

  //Distanzsensor
   Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  distsensor.setTimeout(500);
  if (!distsensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  distsensor.setDistanceMode(VL53L1X::Long);
  distsensor.setMeasurementTimingBudget(50000);
  distsensor.startContinuous(50);
  //NACHHER WIEDER LÖSCHEN BITTE IST NUR FÜR DAS TESTEN DER SPEICHER METHODE
/*
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Ich verbinde mich mit dem Internet...");
  }
  Serial.println("Ich bin mit dem Internet verbunden!");

*/

preferences.begin("WLAN", false); //Ordner erstellen auf eeprom für WLAN-passwort etc
Serial.println("klfjdhalkfjhalsjhfd");
if(start==false) //damit bei Stromausfall auf jeden Fall wieder auf die Page Home gegangen wird 
{
  dfd="Home";
  start = true;
 // WLAN_Automatik();
  for(int i = 0; i < tAnzahl; i++)      //Array befüllen mit Topfnummer 1 bis Anzahl der vorhandenen Töpfe
  { 
        
          Pflanzen[i]={i+1,random(1,6467),random(1,50),0,0,random(1,13),false};            
  }
 for(int i=0;i<sAnzahl; i++)
 {
        int u = random(1,101);
        feuchte[i] = u;
        Serial.println(String(i) + " " + String(u));
 }
}
    // Initialize a NTPClient to get time
  if(WiFi.status() == WL_CONNECTED) timeClient.begin();
  if(WiFi.status() == WL_CONNECTED) timeClient.setTimeOffset(3600);
  
  //Wire.begin();
  //Wire.setClockStretchLimit(4000);

//Pinbelegung Antriebsmotor
  pinMode(32, OUTPUT); //Enable
  pinMode(33, OUTPUT); //Puls
  pinMode(25, OUTPUT); //Direction
  digitalWrite(32,LOW);

  //Pinbelegung Pump / Ventil
  pinMode(Steuerpin_Pumpe1, OUTPUT);
  digitalWrite(Steuerpin_Pumpe1, HIGH);
  pinMode(Steuerpin_Pumpe2, OUTPUT);
  digitalWrite(Steuerpin_Pumpe2, HIGH);
  pinMode(Steuerpin_Pumpe3, OUTPUT);
  digitalWrite(Steuerpin_Pumpe3, HIGH);
  pinMode(Steuerpin_Pumpe4, OUTPUT);
  digitalWrite(Steuerpin_Pumpe4, HIGH);
  pinMode(Steuerpin_Pumpe5, OUTPUT);
  digitalWrite(Steuerpin_Pumpe5, HIGH);
  pinMode(Steuerpin_Pumpe6, OUTPUT);
  digitalWrite(Steuerpin_Pumpe6, HIGH);

 //Pinbelegung Näherungsschalter
 pinMode(SensorPin_R, INPUT);
 pinMode(SensorPin_L, INPUT);




}
void loop() 
{
  //ACHTUNG vor dem Einfügen einer neuen Pflanzensammlung muss das System neugestarted werden. Bei einem Stromausfall kann es zum Verlust von EINEN Datensatz kommen
 
  //Automatik();
  //Spuelen();
  //delay(5000);
 
 //Distanzmessen();
//Backup_erstellen();


page();
while (Seitenwahl == 0) //Page = Home
{
  page();
  //Automatik();
   if(Serial2.available())
   {
      dfd += char(Serial2.read());
      Serial.println(dfd);
   }
   if(dfd.length() == 3 && dfd == "Str")
   {
    Serial.print("Start der Automatik");
    dfd="";
    auto_mode=true; 
   }
}

while (Seitenwahl == 1) //Page = Übersicht
{
  Topf();
  page();
}

while (Seitenwahl == 3) //Page = Übersicht
{
  WLAN_Verbinden();
  page();
}

while (Seitenwahl == 7) //Page = Neu
{
  Neu();
  page();
}

while (Seitenwahl == 5) //Page = Spülen
{
  Spuelen();
  page();
}
    
//Status_R = digitalRead(SensorPin_R);
//Status_L = digitalRead(SensorPin_L);

//Serial.println("Status_R = " + String(Status_R));
//Serial.println("Status_L = " + String(Status_L));


}
