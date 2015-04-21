#include <Ultrasonic.h>

#include <math.h>
#include <SoftwareSerial.h>
//#include <string>
#include <TinyGPS.h>


/* This sample code demonstrates the normal use of a TinyGPS object.
 It requires the use of SoftwareSerial, and assumes that you have a
 4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
 */

char buf[12];
String resultado;
const byte TR1 = 5;
const byte TR2 = 6;
const byte TR3 = 7;
const byte EC1 = 8;
const byte EC2 = 9;
const byte EC3 = 10;

TinyGPS gps;
SoftwareSerial ss(4,3);
Ultrasonic us1(TR1,EC1);
Ultrasonic us2(TR2,EC2);
Ultrasonic us3(TR3,EC3);


float flat, flon;
const float MILESTOKM = 1.609344;

unsigned long velocidad;


String csv;

void setup()
{
  //Serial.begin(115200);
  Serial.begin(9600);
  ss.begin(9600);
  
  //Serial.print("Simple TinyGPS library v. "); 
  //Serial.println(TinyGPS::library_version());
  //Serial.println("by Mikal Hart");
  //Serial.println();
  
  

  
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
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    
    //Serial.print("LAT=");
    //Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Serial.print(" LON=");
    //Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    //Serial.print(" SAT=");
    //Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    //Serial.print(" PREC=");
    //Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());



  }




  gps.stats(&chars, &sentences, &failed);
  //Serial.print(" CHARS=");
  //Serial.print(chars);
  //Serial.print(" SENTENCES=");
  //Serial.print(sentences);
  //Serial.print(" CSUM ERR=");
  //Serial.println(failed);
  //if (chars == 0)
    //Serial.println("** No characters received from GPS: check wiring **");



 velocidad = gps.f_speed_mph() * MILESTOKM;
 
 //Serial.print(velocidad);
 //Serial.println(" km/h");
 Serial.print("v");
 Serial.print(velocidad);
 Serial.print("f");
 delay(100);
 Serial.print("z");
 Serial.print(us1.Ranging(CM));
 Serial.print("f");
  delay(100);
 //Serial.println(" cm");
 Serial.print("x");
 Serial.print(us2.Ranging(CM));
 Serial.print("f");
  delay(100);
 //Serial.println(" cm");
 Serial.print("c");
 Serial.print(us3.Ranging(CM));
 Serial.print("f");
  delay(100);
 //Serial.println(" cm");





}
