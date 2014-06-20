void atiendeXbee(){
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    // hay un paquete disponible
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // hay un packet RX disponible
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
            
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // El emisor recibió un ACK
            DEBUGLN(F("El emisor recibio un ACK")); 
        } else {
            // No se recibió un ACK de vuelta
            DEBUGLN(F("No se recibio un ACK de vuelta")); 
        }
        // Decodificar el mensaje
        mensajeXbeeEntrada[0]=rx.getData(0);
        mensajeXbeeEntrada[1]=rx.getData(1);
        
        DEBUG(F("Mensaje recibido por Xbee: "));
        DEBUG(mensajeXbeeEntrada[0]);
        DEBUGLN(mensajeXbeeEntrada[1]);
        
        // procesar el mensaje
        recibeMensaje(mensajeXbeeEntrada[0],mensajeXbeeEntrada[1]);
        
      } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
        xbee.getResponse().getModemStatusResponse(msr);
        // pauquetes de stado de mode, como asociacion y disociasión
        
        if (msr.getStatus() == ASSOCIATED) {
          // asociado
          DEBUGLN(F("Xbee Asociado"));
        } else if (msr.getStatus() == DISASSOCIATED) {
          // Desasociado
          DEBUGLN(F("Xbee Desasociado"));
        } else {
          // otro status
          DEBUGLN(F("Xbee Con otro status"));
        }
      } else {
      	// error inesperado
        DEBUGLN(F("Xbee con error inesperado"));  
      }
  } else if (xbee.getResponse().isError()) {
      DEBUGLN(F("Paquete de XBee remoto con error")); 
      DEBUGLN(xbee.getResponse().getErrorCode());
  }  
}

void despachaXbee(byte msg, char casa){
  // componer el mensaje
  mensajeXbeeSalida[0]=mensajesXbee[msg].mXbee;
  mensajeXbeeSalida[1]=casa;
  
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
