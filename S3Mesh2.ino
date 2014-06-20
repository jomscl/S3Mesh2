// Version 1.0
// prototipo requerido por visualmicro. Me parece que es un error
void loop();


// modo de funcionamiento. Hay que comentar los que no haya que complilar
#define ID1 // casa 1 y GSM.
//#define ID2 // casa 2
//#define ID3 // casa 3

// modo depuracion, comentar para apagar.
#define MODO_DEBUG

// macros de debug
#ifdef MODO_DEBUG
  #define DEBUG(x)  Serial.print (x)
  #define DEBUGLN(x)  Serial.println (x)
  #define DEBUGDEC(x)  Serial.print (x, DEC)
  #define DEBUGFULL(x)    \
    Serial.print(millis());     \
    Serial.print(": ");    \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');      \
    Serial.print(__FILE__);     \
    Serial.print(':');      \
    Serial.print(__LINE__);     \
    Serial.print(' ');      \
    Serial.println(x);
#else
  #define DEBUG(x)
  #define DEBUGLN(x) 
  #define DEBUGDEC(x)
  #define DEBUGFULL(x)
#endif

// includes
#include <SoftwareSerial.h>
#include <Metro.h>
#include <XBee.h>
#include <avr/pgmspace.h>

// defines
// numero telefonico para los SMS
#define numeroDestinatario 92300776

// puertos
#define pXbeeRX 3  //r
#define pXbeeTX 2  //r
#define pGSMRX 5
#define pGSMTX 6
#define pLED0 12 //r
#define pLED1 11  //r
#define pLED2 10  //r
#define pbuzzer 9  //r
#define pSirena 11
#define pboton 8  //r
#define psensor1 13 // puerta
#define psensor2 7 // ventanas r
#define pllave 15
#define pOnkey 16 //GSM
#define pRandom A0 // para el randomize

// constantes
#define SegSalida 120 // en cuartos de segundo
#define SegEntrada 120
#define SegAlarma 150
#define SegAlarmaPausa 50
#define SegPing 144000 // 250 ms * 4s * 60s * 60m * 10h
#define VarSegPing 57600 // 250 ms * 4s * 60s * 60m * 4h
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
//char mXbee[] ={'A','D','I','S','P','E'};

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
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403e0f30);

// objetos del xbee
ZBTxRequest zbTx = ZBTxRequest(addr64, mensajeXbeeSalida, sizeof(mensajeXbeeSalida));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

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
  
  #ifdef ID1
  if (SerialGSM.available()){
    atiendeGSM();  
  }
  #endif  

}


// rutina para recuperar textos desde flash
char* t(int texto){
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[texto])));
  return buffer;
}

