//TIEMPO DE REFRESCO >
//ACTUALIZACIONES INNECESARIAS <
//MAYOR MODULARIZACION LEDS

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

#define mdeINICIO  0
#define mdeAPARCA  1
#define mdeVELOC   2

#define D0  B00000000
#define D1  B00000001
#define D2  B00000011
#define D3  B00000111
#define D4  B00001111
#define D5  B00011111
#define D6  B00111111
#define D7  B01111111
#define D8  B11111111

byte dibuja[9]={D8,D7,D6,D5,D4,D3,D2,D1,D0};
int cm[3];
int cols[3][2]={{0,1},{3,4},{6,7}};


LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //Creamos el LCD
LedControl ledMatrix = LedControl(DINLED, CLKLED, CSLED, QTD_DISP);  //Creamos la SHIELD de LEDs

int luzamb;  //Almacen para el valor obtenido por el sensor de luz
int modo;  //Modo actual del modulo delantero (inicio, aparcamiento, velocidad)

float velocidad;

char leida;

int getMode() {  //Obtiene el modo actual en funcion de la velocidad
  if (velocidad >= 15) {
    return mdeVELOC;
  } else {
    return mdeAPARCA;
  }
}

void mngLEDs(){
  int divent;
  for (int i=0; i<3; i++){
    divent=cm[i]/5;
    if (divent>9){
      divent=9;
    }
    ledMatrix.setColumn(0, cols[i][0], dibuja[divent]);
    ledMatrix.setColumn(0, cols[i][1], dibuja[divent]);
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
    lcd.print(cm[0]);
    lcd.setCursor(4, 1);
    lcd.print(cm[1]);
    lcd.setCursor(8, 1);
    lcd.print(cm[2]);
    lcd.setCursor(14, 1);
    lcd.print("cm");
    ledMatrix.clearDisplay(0);     //Limpiamos la matriz 0
    ledMatrix.setColumn(0, 2, D0);  //Endendemos la cuarta columna
    ledMatrix.setColumn(0, 5, D0);
    if (cm[0] > 40) {
      ledMatrix.setColumn(0, 0, D0);
      ledMatrix.setColumn(0, 1, D0);
    } else if (cm[0] > 35) {
      ledMatrix.setColumn(0, 0, D1);
      ledMatrix.setColumn(0, 1, D1);
    } else if (cm[0] > 30) {
      ledMatrix.setColumn(0, 0, D2);
      ledMatrix.setColumn(0, 1, D2);
    } else if (cm[0] > 25) {
      ledMatrix.setColumn(0, 0, D3);
      ledMatrix.setColumn(0, 1, D3);
    } else if (cm[0] > 20) {
      ledMatrix.setColumn(0, 0, D4);
      ledMatrix.setColumn(0, 1, D4);
    } else if (cm[0] > 15) {
      ledMatrix.setColumn(0, 0, D5);
      ledMatrix.setColumn(0, 1, D5);
    } else if (cm[0] > 10) {
      ledMatrix.setColumn(0, 0, D6);
      ledMatrix.setColumn(0, 1, D6);
    } else if (cm[0] > 5) {
      ledMatrix.setColumn(0, 0, D7);
      ledMatrix.setColumn(0, 1, D7);
    } else if (cm[0] >= 0) {
      ledMatrix.setColumn(0, 0, D8);
      ledMatrix.setColumn(0, 1, D8);
    }

    if (cm[1] > 40) {
      ledMatrix.setColumn(0, 3, D0);
      ledMatrix.setColumn(0, 4, D0);
    } else if (cm[1] > 35) {
      ledMatrix.setColumn(0, 3, D1);
      ledMatrix.setColumn(0, 4, D1);
    } else if (cm[1] > 30) {
      ledMatrix.setColumn(0, 3, D2);
      ledMatrix.setColumn(0, 4, D2);
    } else if (cm[1] > 25) {
      ledMatrix.setColumn(0, 3, D3);
      ledMatrix.setColumn(0, 4, D3);
    } else if (cm[1] > 20) {
      ledMatrix.setColumn(0, 3, D4);
      ledMatrix.setColumn(0, 4, D4);
    } else if (cm[1] > 15) {
      ledMatrix.setColumn(0, 3, D5);
      ledMatrix.setColumn(0, 4, D5);
    } else if (cm[1] > 10) {
      ledMatrix.setColumn(0, 3, D6);
      ledMatrix.setColumn(0, 4, D6);
    } else if (cm[1] > 5) {
      ledMatrix.setColumn(0, 3, D7);
      ledMatrix.setColumn(0, 4, D7);
    } else if (cm[1] >= 0) {
      ledMatrix.setColumn(0, 3, D8);
      ledMatrix.setColumn(0, 4, D8);
    }

    if (cm[2] > 40) {
      ledMatrix.setColumn(0, 6, D0);
      ledMatrix.setColumn(0, 7, D0);
    } else if (cm[2] > 35) {
      ledMatrix.setColumn(0, 6, D1);
      ledMatrix.setColumn(0, 7, D1);
    } else if (cm[2] > 30) {
      ledMatrix.setColumn(0, 6, D2);
      ledMatrix.setColumn(0, 7, D2);
    } else if (cm[2] > 25) {
      ledMatrix.setColumn(0, 6, D3);
      ledMatrix.setColumn(0, 7, D3);
    } else if (cm[2] > 20) {
      ledMatrix.setColumn(0, 6, D4);
      ledMatrix.setColumn(0, 7, D4);
    } else if (cm[2] > 15) {
      ledMatrix.setColumn(0, 6, D5);
      ledMatrix.setColumn(0, 7, D5);
    } else if (cm[2] > 10) {
      ledMatrix.setColumn(0, 6, D6);
      ledMatrix.setColumn(0, 7, D6);
    } else if (cm[2] > 5) {
      ledMatrix.setColumn(0, 6, D7);
      ledMatrix.setColumn(0, 7, D7);
    } else if (cm[2] >= 0) {
      ledMatrix.setColumn(0, 6, D8);
      ledMatrix.setColumn(0, 7, D8);
    }
  }
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
  while (Serial.available() > 0) {
    String lectura;
    //Serial.println("Lectura reseteada");
    //Serial.println("Hay datos en el serial");
    leida = Serial.read();
    if (leida == 'v') {
      //Serial.println("EL caracter es v");
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println("Continua la lectura");
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
      //Serial.println("Fin de lectura pertinente");
      velocidad = lectura.toInt();
      //Serial.println("Mostrando velocidad:");
      //Serial.println(velocidad);

    } else if (leida == 'z') {
      //Serial.println("EL caracter es z");
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println("Continua la lectura");
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
      //Serial.println("Fin de lectura pertinente");
      cm[0] = lectura.toInt();
      //Serial.println("Mostrando cm:");
      //Serial.println(cm[0]);
    } else if (leida == 'x') {
      //Serial.println("EL caracter es x");
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println("Continua la lectura");
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
      //Serial.println("Fin de lectura pertinente");
      cm[1] = lectura.toInt();
      //Serial.println("Mostrando cm:");
      //Serial.println(cm[1]);
    } else if (leida == 'c') {
      //Serial.println("EL caracter es c");
      leida = Serial.read();
      while (leida != 'f') {
        //Serial.println("Continua la lectura");
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
      //Serial.println("Fin de lectura pertinente");
      cm[2] = lectura.toInt();
      //Serial.println("Mostrando cm:");
      //Serial.println(cm[2]);
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
