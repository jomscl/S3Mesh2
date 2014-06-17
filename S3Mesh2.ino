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
#define pLED0 7
#define pLED1 8
#define pLED2 9
#define pbuzzer 10
#define pSirena 11
#define pboton 12
#define psensor1 13 // puerta
#define psensor2 14 // ventanas
#define pllave 15

// constantes
#define SegSalida 120; // en cuartos de segundo
#define SegEntrada 120;
#define SegAlarma 150;
#define SegAlarmaPausa 50;

// constantes de estado de la alarma
#define eDesarmado 0
#define eArmandose 1
#define eArmada 2
#define eAlarma 3
#define eAlarmaPausa 4
#define eEntrada 5

// constantes de estado del buzzer
#define eBuzzerOff 0
#define eBuzzerOn 1
#define eBuzzerOPulsoLento 2
#define eBuzzerOPulsoRapido 3

// constantes de estado de la sirena
#define eSirenaOff 0
#define eSirenaOn 1

// constantes de estado de los LED
#define eLEDOff 0
#define eLEDOn 1
#define eLEDPulsoLento 2
#define eLEDPulsoRapido 3

// variables de hardware
boolean llave;
boolean sensor1; // puerta
boolean sensor2; // ventanas
boolean boton;

// variables de tiempo
unsigned int tiempoSalida=0;
unsigned int tiempoEntrada=0;
unsigned int tiempoAlarma=0;
unsigned int tiempoAlarmaPausa=0;
byte cicloTimer=0;

// variables de estado
byte estadoAlarma=eDesarmado;
byte estadoBuzzer=eBuzzerOff;
byte estadoSirena=eSirenaOff;
byte estadoLED0=eLEDOff;
byte estadoLED1=eLEDOff;
byte estadoLED2=eLEDOff;

// punteros al led que corresponda, según el ID de cada placa
#ifdef ID1
byte pLEDPropio=pLED0;
byte *estadoLEDPropio = &estadoLED0;
#endif

#ifdef ID2
byte pLEDPropio=pLED1;
byte* estadoLEDPropio = &estadoLED1;
#endif

#ifdef ID3
byte pLEDPropio=pLED2;
byte* estadoLEDPropio = &estadoLED2;
#endif



// Objeto serial para el Xbee
SoftwareSerial SerialXbee(pXbeeRX, pXbeeTX); // RX, TX

// Objeto serial para el modem, si aplica
#ifdef ID1
SoftwareSerial SerialGSM(pGSMRX, pGSMTX); // RX, TX
#endif

 // objeto timer
 Metro timerMetro = Metro(250); 

void setup() {
	// config de variables
	*estadoLEDPropio=eLEDOn;

	// config general
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
