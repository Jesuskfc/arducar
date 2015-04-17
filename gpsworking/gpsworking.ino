#include <math.h>
#include <SoftwareSerial.h>

#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
 It requires the use of SoftwareSerial, and assumes that you have a
 4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
 */




TinyGPS gps;
SoftwareSerial ss(4,3);

float flatAnt, flonAnt;
float flat, flon;

float cmillis,cmillisAnt;
unsigned long velocidad;


void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  flat=0.0;
  flon=0.0;
  
  
  Serial.print("Simple TinyGPS library v. "); 
  Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  
  cmillis=millis();

  
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }


  if (newData)
  {
    flatAnt = flat;
    flonAnt = flon;
    unsigned long age;
    cmillisAnt=cmillis;
    cmillis = millis();
    gps.f_get_position(&flat, &flon, &age);
    
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());



  }




  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

/*
  int R = 6371000; // metres
  float l1 = toRad(flat);
  float l2 = toRad(flatAnt); 
  float dLat = toRad(flatAnt-flat);
  float dLong = toRad(flonAnt-flon);

  float a = sin(dLat/2) * sin(dLat/2) +
    cos(l1) * cos(l2) *
    sin(dLong/2) * sin(dLong/2);

  float c = 2 * atan2(sqrt(a), sqrt(1-a));

  float d = R * c;

  velocidad = d/(cmillisAnt-cmillis);

  velocidad = velocidad * 3600;
  
  Serial.print(d,5); Serial.println(" m");
  Serial.print(velocidad,5);Serial.println(" Km/h");

*/

 velocidad = gps.f_speed_mph() * 1.609344;
 
 Serial.print(velocidad);
 Serial.println(" km/h");

}



double toRad (float grados){

  return (grados * M_PI) / 180; //3.14159265359

}

