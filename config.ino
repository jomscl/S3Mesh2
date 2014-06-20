void config(){
  // configuracion del DEBUG
  #ifdef MODO_DEBUG
    Serial.begin(9600);
  #endif

  // iniciar el puerto serial del XBee
  SerialXbee.begin(9600);
  
  // iniciar el xbee
  xbee.setSerial(SerialXbee);
  
  // iniciar el puerto serial del modem, si aplica
  #ifdef ID1
    SerialGSM.begin(9600);
    configModem();
  #endif

  // Configuración pines
  pinMode(pLED0,OUTPUT);
  pinMode(pLED1,OUTPUT);
  pinMode(pLED2,OUTPUT);
  pinMode(pbuzzer,OUTPUT);
  pinMode(pSirena,OUTPUT);
  pinMode(pboton,INPUT_PULLUP);
  pinMode(psensor1,INPUT_PULLUP);
  pinMode(psensor2,INPUT_PULLUP);
  pinMode(pllave,INPUT_PULLUP);
  pinMode(pRandom,INPUT);
  
  DEBUGLN(F("Encendida"));
  despachaMensaje(mEncendida, IDCasa); // reporte
  
  // inicializar el ping
  randomSeed(analogRead(pRandom));
  nuevoTiempoPing();


  // crear los mensajes
  creaMensaje(tXBeeReporte,'A',0);
  creaMensaje(tXBeeReporte,'D',1);
  creaMensaje(tXBeeReporte,'I',2);
  creaMensaje(tXBeeComunitario,'S',3);
  creaMensaje(tXBeeReporte,'P',4);
  creaMensaje(tXBeeReporte,'E',5);
}

void creaMensaje(boolean tipoMensaje, char mXbee, byte idMensaje){
  mensajesXbee[idMensaje].idMensaje=idMensaje;
  mensajesXbee[idMensaje].mXbee=mXbee;
  mensajesXbee[idMensaje].tipoMensaje=tipoMensaje;
}
