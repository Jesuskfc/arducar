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

/*
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
*/

#define mdeINICIO  0
#define mdeAPARCA  1
#define mdeVELOC   2


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Creamos el LCD
LedControl ledMatrix = LedControl(DINLED, CLKLED, CSLED, QTD_DISP);  //Creamos la SHIELD de LEDs

int luzamb;  //Almacen para el valor obtenido por el sensor de luz
int tempamb;  //Almacen para el valor obtenido por el sensor de temperatura
float tempgrados;  //Temperatura en grados centigrados
int modo;  //Modo actual del modulo delantero (inicio, aparcamiento, velocidad)
float velocidad;
//int adc_key_in = 0;
int cm1;
int cm2;
int cm3;
//char* lectura;
char leida;
char id;

/*
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}
*/

int getMode() {  //Obtiene el modo actual en funcion de la velocidad
  if (velocidad >= 15) {
    return mdeVELOC;
  } else {
    return mdeAPARCA;
  }
}

void mngMode(int modo) {  //Realiza las acciones correspondientes al modo actual
  if (modo == mdeINICIO) {  //Si estamos en el modo de INICIO
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Precaucion,     ");
    lcd.setCursor(0, 1);
    lcd.print("amigo conductor!");
    delay(2500);  //Esperamos 2.5s
    tempgrados = calcularGrados(); //Esta la cabecera de la funcion creada mas abajo, cread la funcion o si encontrais una ya creada eliminad esto
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("La temperatura  ");
    lcd.setCursor(0, 1);
    lcd.print("actual es ");
    lcd.print(analogRead(5));  //Muestra la temperatura actual en grados con un decimal
    delay(2500);  //Esperamos 2.5s
  } else if (modo == mdeVELOC) {  //Si estamos en el modo de VELOCIDAD
    lcd.clear();
    lcd.setCursor(0, 0);
    if (velocidad < 100) {  //Esto es para crear un espacio en caso de que sean menos de 100km/h
      lcd.print(" ");
    }
    lcd.print(velocidad, 2);  //Muestra la velocidad con 2 decimales
    lcd.print("km/h");
    lcd.setCursor(0, 1);
    if (velocidad <= 50) {
      lcd.print("Poblacion");
    } else if (velocidad <= 90) {
      lcd.print("Comarc/Nac/Local");
    } else if (velocidad <= 120) {
      lcd.print("Autovia/Autopist");
    } else {
      lcd.print("SOBRE LIMITE");
    }
  } else if (modo == mdeAPARCA) {  //Si estamos en modo de aparcamiento
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SIz SMd SDr");
    lcd.setCursor(0, 1);
    lcd.print(cm1);
    lcd.setCursor(4, 1);
    lcd.print(cm2);
    lcd.setCursor(8, 1);
    lcd.print(cm3);
    lcd.setCursor(14, 1);
    lcd.print("cm");
    ledMatrix.clearDisplay(0);     //Limpiamos la matriz 0
    ledMatrix.setColumn(0, 2, B00000000);  //Endendemos la cuarta columna
    ledMatrix.setColumn(0, 5, B00000000);
    if (cm1 > 40) {
      ledMatrix.setColumn(0, 0, B00000000);
      ledMatrix.setColumn(0, 1, B00000000);
    } else if (cm1 > 35) {
      ledMatrix.setColumn(0, 0, B00000001);
      ledMatrix.setColumn(0, 1, B00000001);
    } else if (cm1 > 30) {
      ledMatrix.setColumn(0, 0, B00000011);
      ledMatrix.setColumn(0, 1, B00000011);
    } else if (cm1 > 25) {
      ledMatrix.setColumn(0, 0, B00000111);
      ledMatrix.setColumn(0, 1, B00000111);
    } else if (cm1 > 20) {
      ledMatrix.setColumn(0, 0, B00001111);
      ledMatrix.setColumn(0, 1, B00001111);
    } else if (cm1 > 15) {
      ledMatrix.setColumn(0, 0, B00011111);
      ledMatrix.setColumn(0, 1, B00011111);
    } else if (cm1 > 10) {
      ledMatrix.setColumn(0, 0, B00111111);
      ledMatrix.setColumn(0, 1, B00111111);
    } else if (cm1 > 5) {
      ledMatrix.setColumn(0, 0, B01111111);
      ledMatrix.setColumn(0, 1, B01111111);
    } else if (cm1 >= 0) {
      ledMatrix.setColumn(0, 0, B11111111);
      ledMatrix.setColumn(0, 1, B11111111);
    }

    if (cm2 > 40) {
      ledMatrix.setColumn(0, 3, B00000000);
      ledMatrix.setColumn(0, 4, B00000000);
    } else if (cm2 > 35) {
      ledMatrix.setColumn(0, 3, B00000001);
      ledMatrix.setColumn(0, 4, B00000001);
    } else if (cm2 > 30) {
      ledMatrix.setColumn(0, 3, B00000011);
      ledMatrix.setColumn(0, 4, B00000011);
    } else if (cm2 > 25) {
      ledMatrix.setColumn(0, 3, B00000111);
      ledMatrix.setColumn(0, 4, B00000111);
    } else if (cm2 > 20) {
      ledMatrix.setColumn(0, 3, B00001111);
      ledMatrix.setColumn(0, 4, B00001111);
    } else if (cm2 > 15) {
      ledMatrix.setColumn(0, 3, B00011111);
      ledMatrix.setColumn(0, 4, B00011111);
    } else if (cm2 > 10) {
      ledMatrix.setColumn(0, 3, B00111111);
      ledMatrix.setColumn(0, 4, B00111111);
    } else if (cm2 > 5) {
      ledMatrix.setColumn(0, 3, B01111111);
      ledMatrix.setColumn(0, 4, B01111111);
    } else if (cm2 >= 0) {
      ledMatrix.setColumn(0, 3, B11111111);
      ledMatrix.setColumn(0, 4, B11111111);
    }

    if (cm3 > 40) {
      ledMatrix.setColumn(0, 6, B00000000);
      ledMatrix.setColumn(0, 7, B00000000);
    } else if (cm3 > 35) {
      ledMatrix.setColumn(0, 6, B00000001);
      ledMatrix.setColumn(0, 7, B00000001);
    } else if (cm3 > 30) {
      ledMatrix.setColumn(0, 6, B00000011);
      ledMatrix.setColumn(0, 7, B00000011);
    } else if (cm3 > 25) {
      ledMatrix.setColumn(0, 6, B00000111);
      ledMatrix.setColumn(0, 7, B00000111);
    } else if (cm3 > 20) {
      ledMatrix.setColumn(0, 6, B00001111);
      ledMatrix.setColumn(0, 7, B00001111);
    } else if (cm3 > 15) {
      ledMatrix.setColumn(0, 6, B00011111);
      ledMatrix.setColumn(0, 7, B00011111);
    } else if (cm3 > 10) {
      ledMatrix.setColumn(0, 6, B00111111);
      ledMatrix.setColumn(0, 7, B00111111);
    } else if (cm3 > 5) {
      ledMatrix.setColumn(0, 6, B01111111);
      ledMatrix.setColumn(0, 7, B01111111);
    } else if (cm3 >= 0) {
      ledMatrix.setColumn(0, 6, B11111111);
      ledMatrix.setColumn(0, 7, B11111111);
    }


  }
}

float calcularGrados() {
  /*
    Obtener el valor del sensor de calor y transformar ese valor abstracto a grados
    centigrados
  */
}

void setup() {
  //inicio del lcd
  lcd.begin(16, 2);
  modo = mdeINICIO;
  velocidad = 0.0;
  ledMatrix.shutdown(0, false);  //Ponemos el estado apagado a falso en la matriz de LEDs 0, es decir, encendemos la matriz de LEDs
  ledMatrix.setIntensity(0, 16);  //Colocamos un valor de intensidad de brillo de 5 (0~16)
  ledMatrix.clearDisplay(0);     //Limpiamos la matriz 0
  Serial.begin(9600);  //Inicia la conexion entre XBEEs
}

void loop() {
  //lectura = "";
  //Serial.println("Lectura reseteada");
  String lectura;
  //Serial.println(lectura);
  while (Serial.available() > 0) {
    //Serial.println("Hay datos en el serial");
    id=Serial.read();
    if (id == 'v') {
      //Serial.println("EL caracter es v");
      leida = Serial.read();
      while (leida != 'f') {
       // Serial.println("Continua la lectura");
        //Serial.println(leida);
        //Serial.println("Valor leido mostrado");
        lectura = lectura + leida;
        //Serial.println("Mostrando string:");
        //Serial.println(lectura);
        //Serial.println("String mostrado");
        if (Serial.available() > 0) {
          leida = Serial.read();
        } else {
          leida = 'f';
        }

      }
      //Serial.println("Fin de lectura");
      velocidad = lectura.toInt();
      //Serial.println("Mostrando velocidad:");
      //Serial.println(velocidad);
    }
    else if (id == 'z') {
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println(leida);
        lectura = lectura + leida;
        //Serial.println(lectura);
        if (Serial.available() > 0) {
          leida = Serial.read();
        } else {
          leida = 'f';
        }
      }
      cm1 = lectura.toInt();
      //Serial.println(cm1);
    } else if (id == 'x') {
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println(leida);
        lectura = lectura + leida;
        //Serial.println(lectura);
        if (Serial.available() > 0) {
          leida = Serial.read();
        } else {
          leida = 'f';
        }
      }
      cm2 = lectura.toInt();
      //Serial.println(cm2);
    } else if (id == 'c') {
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println(leida);
        lectura = lectura + leida;
        //Serial.println(lectura);
        if (Serial.available() > 0) {
          leida = Serial.read();
        } else {
          leida = 'f';
        }
      }
      cm3 = lectura.toInt();
      //Serial.println(cm3);
    }
  }
  if (analogRead(5) < 900) {  //Enciende la luz si esta el sensor recibe un valor de mas de 900, si no la apaga
    pinMode(10, INPUT);
  } else {
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
  }
  mngMode(modo);  //Gestiona el modo
  modo = getMode();  //Obtiene el nuevo modo
}
