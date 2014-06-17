void config(){
  // configuracion del DEBUG
  #ifdef MODO_DEBUG
    Serial.begin(9600);
  #endif

  // iniciar el puerto serial del XBee
  SerialXbee.begin(9600);
  
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
  despachaMensaje(F("Encendida "), IDCasa, tXBeeReporte);
  
  // inicializar el ping
  randomSeed(analogRead(pRandom));
  nuevoTiempoPing();
}
