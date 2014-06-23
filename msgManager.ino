void despachaMensaje(byte msg, char casa){
  // esta rutina solo la llama el timer, por lo que solo son mensajes generados por este kit
  #ifdef ID1
    // todo lo recibido de manda por SMS
    despachaSMS(msg,casa);
    // para no sobrecargar la red con mensajes que no le intersan a otras casas
    // solo se transmite los mensajes comunitarios
    if (mensajesXbee[msg].tipoMensaje==tXBeeComunitario){ 
      despachaXbee(msg,casa);
    }
  #else
    despachaXbee(msg,casa);
  #endif
}

void recibeMensaje(char msg, char casa){
  // si es ID1, despacha el mensaje por SMS
  #ifdef ID1
    // despacho el mensaje por sms
    despachaSMS(msg,casa);
  #endif

  // idetifico que tipo de mensaje se recibió
  byte idMsg=IdMensaje(msg);
  // ver si son mensajes comunitarios
  if (mensajesXbee[idMsg].tipoMensaje==tXBeeComunitario){
    // tomar acciones
    informaAlarma(casa);
  }
}

byte IdMensaje(byte msg){
  for (int i=0;i<nMensajes;i++){
    if (mensajesXbee[i].mXbee==msg) {return i;} 
  }
}

void informaAlarma(char casa){
	DEBUG(F("Alarma casa "));
	DEBUGW(casa);
	DEBUGLN();
	estadoBuzzer=eBuzzerPulsoRapido;
	if (casa=='1'){
		tiempoInformeLED0=SegInformeLED;
		estadoLED0=eLEDPulsoRapido;
	}
	if (casa=='2'){
		tiempoInformeLED1=SegInformeLED;
		estadoLED1=eLEDPulsoRapido;
	}
	if (casa=='3'){
		tiempoInformeLED2=SegInformeLED;
		estadoLED2=eLEDPulsoRapido;
	}
}