void atiendeXbee(){
  
}

void despachaXbee(byte msg, char casa){
  // componer el mensaje
  mensajeXbee[0]=mXbee[msg];
  mensajeXbee[1]=casa;
  
  //despachar el mensaje
  xbee.send(zbTx);
  
  // analizar la respuesta
  if (xbee.readPacket(500)) {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // El dato llegó correctamente
        DEBUGLN(F("Transmisión XBee OK"));
      }
      else {
        // El dato no llegó a destino
        DEBUGLN(F("Sin respuesta XBee remoto"));
      }
    }
  }
  else
  {
     // ocurre potencialmente por falla en el modulo mismo
     DEBUGLN(F("Sin comunicacion con el Xbee local"));
  } 
}
