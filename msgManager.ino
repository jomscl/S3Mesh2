void despachaMensaje(byte msg, char casa, boolean tipoMensaje){
  #ifdef ID1
    if (tipoMensaje==tXBeeComunitario){
      despachaSMS(msg,casa);
      despachaXbee(msg,casa);
    }
    if (tipoMensaje==tXBeeReporte){
      despachaSMS(msg,casa);
    }
  #else
    despachaXbee(msg,casa);
  #endif
}
