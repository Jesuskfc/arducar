#include <Ultrasonic.h>

#include <math.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

char buf[12];
String resultado;
const byte TR1 = 5;
const byte TR2 = 6;
const byte TR3 = 7;
const byte EC1 = 8;
const byte EC2 = 9;
const byte EC3 = 10;

TinyGPS gps;
SoftwareSerial ss(4, 3);
Ultrasonic us1(TR1, EC1);
Ultrasonic us2(TR2, EC2);
Ultrasonic us3(TR3, EC3);

const float MILESTOKM = 1.609344;
unsigned long velocidad;

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if (newData)
  {
    velocidad = gps.f_speed_mph() * MILESTOKM;
    Serial.print("v");
    Serial.print(velocidad);
    Serial.print("f");
    delay(100);
  }
  Serial.print("z");
  Serial.print(us1.Ranging(CM));
  Serial.print("f");
  delay(100);
  Serial.print("x");
  Serial.print(us2.Ranging(CM));
  Serial.print("f");
  delay(100);
  Serial.print("c");
  Serial.print(us3.Ranging(CM));
  Serial.print("f");
  delay(100);
}
