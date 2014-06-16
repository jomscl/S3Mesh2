void config(){
  // iniciar el puerto serial del XBee
  SerialXbee.begin(9600);
  
  // iniciar el puerto serial del modem, si aplica
#ifdef ID1
  SerialGSM.begin(9600);
#endif

  // Configuración pines
  pinMode(pLED1,OUTPUT);
  pinMode(pLED2,OUTPUT);
  pinMode(pLED3,OUTPUT);
  pinMode(pbuzzer,OUTPUT);
  pinMode(pSirena,OUTPUT);
  pinMode(pboton,INPUT_PULLUP);
  pinMode(psensor1,INPUT_PULLUP);
  pinMode(psensor2,INPUT_PULLUP);
  pinMode(pllave,INPUT_PULLUP);
  
  
}