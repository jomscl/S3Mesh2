// Version 1.0
// prototipo requerido por visualmicro. Me parece que es un error
void loop();

// modo de funcionamiento. Hay que comentar los que no haya que complilar
#define ID1 // casa 1 y GSM.
//#define ID2 // casa 2
//#define ID3 // casa 3

// modo depuracion, comentar para apagar.
#define MODO_DEBUG

// includes
#include <SoftwareSerial.h>
#include <Metro.h>
#include <XBee.h>
#include <avr/pgmspace.h>
#include <debug.h>

// defines
// numero telefonico para los SMS
#define numeroDestinatario 92300776

// puertos
// seriales original
//#define pDebugRX 0
//#define pDebugTX 1
//#define pXbeeRX 3  //r
//#define pXbeeTX 2  //r
//#define pGSMRX 4	//r amarillo
//#define pGSMTX 5	// r blanco

// Seriales
#define pGSMRX 0 //r amarillo
#define pGSMTX 1 // r blanco
#define pXbeeRX 3  //r
#define pXbeeTX 2  //r
#define pDebugRX 4	
#define pDebugTX 5	
#define pLED0 12 //r
#define pLED1 11  //r
#define pLED2 10  //r
#define pbuzzer 9  //r
#define pSirena 6 //r
#define pboton 8  //r
#define psensor1 13 // puerta r
#define psensor2 7 // ventanas r
#define pllave 15 //r A1
#define pOnkey 16 //GSM A2, verde
#define pRandom A0 // para el randomize

// constantes
#define SegSalida 20//120 // en cuartos de segundo
#define SegEntrada 12//120
#define SegAlarma 40//150
#define SegAlarmaPausa 5//50
#define SegPing 144000 // 250 ms * 4s * 60s * 60m * 10h
#define VarSegPing 57600 // 250 ms * 4s * 60s * 60m * 4h
#define SegInformeLED 40//150
#define freqBuzzer 1200 // hz

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
#define eBuzzerPulsoLento 2
#define eBuzzerPulsoRapido 3

// constantes de estado de la sirena
#define eSirenaOff 0
#define eSirenaOn 1

// constantes de estado de los LED
#define eLEDOff 0
#define eLEDOn 1
#define eLEDPulsoLento 2
#define eLEDPulsoRapido 3

// variables de tipo de despacho de mensaje
#define tXBeeComunitario 0
#define tXBeeReporte 1

// variables de tipo de mensaje
#define mArmada 0
#define mDesarmada 1
#define mInicioAlarma 2
#define mDespachoAlarma 3
#define mPing 4
#define mEncendida 5
#define nMensajes 6

struct stMensaje{
  byte tipoMensaje;
  char mXbee;
  byte idMensaje;
};

stMensaje mensajesXbee[nMensajes];

// mensajes SMS
prog_char string_0[] PROGMEM = "Armada ";
prog_char string_1[] PROGMEM = "Desarmada ";
prog_char string_2[] PROGMEM = "Inicio Alarma ";
prog_char string_3[] PROGMEM = "Despacho Alarma ";
prog_char string_4[] PROGMEM = "Ping ";
prog_char string_5[] PROGMEM = "Encendida ";

char buffer[16];    // debe ser tan grande como el string más grande.

// Vector de mensajes
PROGMEM const char *string_table[] = 	 
{  
  string_0, string_1, string_2, string_3, string_4, string_5
};

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
unsigned int tiempoPing=0;
unsigned int tiempoInformeLED0=0;
unsigned int tiempoInformeLED1=0;
unsigned int tiempoInformeLED2=0;

// variables de estado
byte estadoAlarma=eDesarmado;
byte estadoBuzzer=eBuzzerOff;
byte estadoSirena=eSirenaOff;
byte estadoLED0=eLEDOff;
byte estadoLED1=eLEDOff;
byte estadoLED2=eLEDOff;

// punteros al led que corresponda, segun el ID de cada placa
#ifdef ID1
byte pLEDPropio=pLED0;
byte *estadoLEDPropio = &estadoLED0;
char IDCasa = '1';
#endif

#ifdef ID2
byte pLEDPropio=pLED1;
byte* estadoLEDPropio = &estadoLED1;
char IDCasa = '2';
#endif

#ifdef ID3
byte pLEDPropio=pLED2;
byte* estadoLEDPropio = &estadoLED2;
char IDCasa = '3';
#endif

// objeto Xbee
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// buffer de datos del xbee
uint8_t mensajeXbeeSalida[] = { 0, 0 };
uint8_t mensajeXbeeEntrada[] = { 0, 0 };

// parametros del Xbee
// SH + SL Address of receiving XBee
// con la direccion ffff se hace un broadcast
XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x0000ffff);

// objetos del xbee
ZBTxRequest zbTx = ZBTxRequest(addr64, mensajeXbeeSalida, sizeof(mensajeXbeeSalida));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

// Objeto serial para el Xbee
SoftwareSerial SerialXbee(pXbeeRX, pXbeeTX); // RX, TX

// puerto serial para el debug, importante para que funcione la librería
SoftwareSerial SerialDebug(pDebugRX, pDebugTX); // RX, TX
Print* pSerialDebug = &SerialDebug;

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
  
  atiendeXbee();  
    
  #ifdef ID1
  if (Serial.available()){
    atiendeGSM();  
  }
  #endif  
}

// rutina para recuperar textos desde flash
char* t(int texto){
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[texto])));
  return buffer;
}

