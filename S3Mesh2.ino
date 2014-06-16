// Version 1.0

// modo de funcionamiento. Hay que comentar los que no haya que complilar
#define ID1 // casa 1 y master
//#define ID2 // casa 2
//#define ID3 // casa 3

// includes
#include <SoftwareSerial.h>
#include <Metro.h>

// defines
// puertos
#define pXbeeRX 3
#define pXbeeTX 4
#define pGSMRX 5
#define pGSMTX 6
#define pLED1 7
#define pLED2 8
#define pLED3 9
#define pbuzzer 10
#define pSirena 11
#define pboton 12
#define psensor1 13
#define psensor2 14
#define pllave 15

// constantes
#SegSalida 120; // en cuartos de segundo

// constantes de estado
#define eDesarmado 0
#define eArmandose 1
#define eArmada 2



// variables
boolean llave;
boolean sensor1;
boolean sensor2;
boolean boton;
unsigned int estado=eDesarmado;
unsigned int tiempoSalida=0;

// Objeto serial para el Xbee
SoftwareSerial SerialXbee(pXbeeRX, pXbeeTX); // RX, TX

// Objeto serial para el modem, si aplica
#ifdef ID1
SoftwareSerial SerialGSM(pGSMRX, pGSMTX); // RX, TX
#endif

  // objeto timer
  Metro timerMetro = Metro(250); 

void setup() {
  config();

}

void loop() {
  if (timerMetro.check()==1){
    // realiza las tareas por timer
    atiendeTimer();  
  }
  if (SerialXbee.available()){
    atiendeXbee();  
  }
  if (SerialGSM.available()){
    atiendeGSM();  
  }
}
