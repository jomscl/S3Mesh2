#ifdef ID1
void configModem(){
  pinMode(pOnkey,OUTPUT);
  digitalWrite(pOnkey,LOW);
  resetGsm();
}

void atiendeGSM(){
  // por ahora, solo vaciar el buffer
  char c;
  while (Serial.available()){
      c=Serial.read();
  }
}

void resetGsm(){
  char c;
  boolean ok=false;
 
  while (!ok){
    // descargar el buffer gsm
    while (Serial.available()){
      c=Serial.read();  
    }  
    
    // ciclo de reinicio
    //digitalWrite(pOnkey,HIGH);
    DEBUGLN(F("MODEM Apagado"));
    delay(5000);
    //digitalWrite(pOnkey,LOW);
    DEBUGLN(F("MODEM Encendido"));
    delay(5000);
    
    // revisión de actividad
    Serial.println("AT");
    delay(200);
    while (Serial.available()){
      c=Serial.read();
      //DEBUG(c);
      if(c=='O'){ok=true;}
    }
  }
  DEBUGLN(F("MODEM OK"));
}
void despachaSMS2(byte mensaje, char casa){
}

void despachaSMS(byte mensaje, char casa){
  
  // numero del destinatario
  Serial.print(F("AT+CMGS=\""));
  Serial.print(numeroDestinatario);
  Serial.println(F("\""));
  delay(10);
  
  // mensaje
  Serial.print("Mensaje S3:");
  Serial.print(t(mensaje));
  Serial.print(F(" M:"));
   Serial.print(mensajesXbee[mensaje].mXbee);
  Serial.print(F(" C:"));
  Serial.print(casa);
  Serial.write(26); // control z
  delay(1000);
  limpiaGSM();
  // borrado de mensajes anteriores
  borrado_sms();
  DEBUGLN(F("SMS despachado"));
}

void borrado_sms(){
	Serial.println(F("AT+CMGD=1,4"));
	delay(200);
	limpiaGSM();
}

#endif // fin de la compilación para el ID1

void limpiaGSM(){
	Serial.flush();
}
