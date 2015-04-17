/*
    +XBEE conectado tal cual al Arduino a través de los cables
    +Tierra (GND) a una de las hileras de la protoboard y 5V a la otra
    +Pantalla LCD con todos sus pines conectados, los que correspondan 
  a la Arduino y los que correspondan a la XBEE, los 5V de la LCD a la 
  hilera de la protoboard de 5V y la GND de la LCD a la hilera de GND
  de la protoboard
    +SHIELD de LEDs con VCC a la hilera de 5V de la protoboard y con GND a
  GND de la protoboard y DIN soldado al pin 7 digital en el LCD, CS al 6
  y CLK al 5
    +Sensor de luz soldado junto con la resistencia al pin analogico 5 del
  LCD, la resistencia al GND de la protoboard y la otra pata del sensor a
  la hilera 5V de la protoboard
    +Sensor de temperatura, exactamente igual que el de luz pero en el pin
  numero 4
*/



#include <LiquidCrystal.h>
#include "LedControl.h"

#define DINLED 7
#define CSLED 6
#define CLKLED 5
#define QTD_DISP 1

#define SENSLUZ 5
#define SENSTEMP 4


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Creamos el LCD
LedControl ledMatrix = LedControl(DINLED, CLKLED, CSLED, QTD_DISP);  //Creamos la SHIELD de LEDs

int luzamb;  //Almacen para el valor obtenido por el sensor de luz
int tempamb;  //Alamacen para el valor obtenido por el sensor de temperatura



void setup() {
  lcd.begin(16, 2);  //Inicializamos el LCD con 16 columnas y 2 filas
  ledMatrix.shutdown(0, false);  //Ponemos el estado apagado a falso en la matriz de LEDs 0, es decir, encendemos la matriz de LEDs
  ledMatrix.setIntensity(0, 5);  //Colocamos un valor de intensidad de brillo de 5 (0~16)
  ledMatrix.clearDisplay(0);     //Limpiamos la matriz 0
}

void loop() {
  lcd.clear()  //Limpiamos el LCD
  lcd.setCursor(0, 0);  //Colocamos el cursor al inicio del LCD
  lcd.print("Luz:");
  luzamb = analogRead(SENSLUZ);  //Obtenemos el valor del sensor de luz
  lcd.print(luzamb);  //Mostramos el valor obtenido por el sensor, se puede utilizar esto para conocer los valores que obtiene en funcion de la luz que haya
  lcd.setCursor(0, 1);  //Colocamos el cursor al inicio de la segunda fila
  lcd.print("Temp:");
  luzamb = analogRead(SENSTEMP);  //Obtenemos el valor del sensor de temperatura
  lcd.print(tempamb);  //Mostramos el valor obtenido por el sensor, se puede utilizar esto para conocer los valores que obtiene en funcion de la temperatura que haya
  ledMatrix.setColumn(0, 3, B11111111);  //Endendemos la cuarta columna
  delay(50);
  ledMatrix.setColumn(0, 3, B00000000);  //Apagamos la cuarta columna
}
