#ifdef ID1
void configModem(){
  pinMode(pOnkey,OUTPUT);
  digitalWrite(pOnkey,LOW);
  resetGsm();
}

void atiendeGSM(){
  // por ahora, solo vaciar el buffer
  char c;
  while (SerialGSM.available()){
      c=SerialGSM.read();
  }
}

void resetGsm(){
  char c;
  boolean ok=false;
 
  while (!ok){
    // descargar el buffer gsm
    while (SerialGSM.available()){
      c=SerialGSM.read();  
    }  
    
    // ciclo de reinicio
    digitalWrite(pOnkey,HIGH);
    DEBUGLN(F("MODEM Apagado"));
    delay(5000);
    digitalWrite(pOnkey,LOW);
    DEBUGLN(F("MODEM Encendido"));
    delay(5000);
    
    // revisión de actividad
    SerialGSM.println("AT");
    delay(200);
    while (SerialGSM.available()){
      c=SerialGSM.read();
      if(c==48){ok=true;}
    }
  }
  DEBUGLN(F("MODEM OK"));
}

void despachaSMS(byte mensaje, char casa){
  
  // numero del destinatario
  SerialGSM.print(F("AT+CMGS=\""));
  SerialGSM.print(numeroDestinatario);
  SerialGSM.println("\"");
  delay(10);
  
  // mensaje
  SerialGSM.print(t(mensaje));
  SerialGSM.print(" C:");
  SerialGSM.print(casa);
  SerialGSM.write(26); // control z
  delay(1000);
  
  // borrado de mensajes anteriores
  borrado_sms();
  DEBUGLN(F("SMS despachado"));
}

void borrado_sms(){
  SerialGSM.println(F("AT+CMGD=1,4"));
  delay(200);
}

#endif // fin de la compilación para el ID1
