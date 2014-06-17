void atiendeTimer(){
  // actualiza las entradas. Son invertidas por el pull up resistor.
  llave=!digitalRead(pllave);
  sensor1=!digitalRead(psensor1);
  sensor2=!digitalRead(psensor2);
  boton=!digitalRead(pboton);
  
  // analisis según estado
  if (estadoAlarma==eDesarmado){ // alarma desarmada
    if (llave){ // se acaba de iniar el armado de la alarma
      tiempoSalida=SegSalida;
      estadoAlarma=eArmandose;
      estadoBuzzer=eBuzzerOPulsoRapido;
      *estadoLEDPropio=eLEDPulsoRapido;
    }
  }

  if (estadoAlarma==eArmandose){ // alarma armandose, descontando tiempo
    if (tiempoSalida>0){
      tiempoSalida--;
	  if (!llave){ // si se corta la llave por dentro
		desarmarAlarma();
	  }
    }
    else{
      armarAlarma();
	}
  }
  
  if (estadoAlarma==eArmada){ // alarma armada
    if (!llave){ // si se corta la llave por dentro
      desarmarAlarma();
    }
    else{
      checkSensores();
    }
  }

  if (estadoAlarma==eEntrada){ // puerta abierta, timer entrada
	if (!llave){ // si se corta la llave por dentro
      desarmarAlarma();
    }
    else{
      if (tiempoEntrada>0){
		  tiempoEntrada--;
	  }
	  else
	  {
		  activaAlarma();
	  }
    }
  }
  
  if (estadoAlarma==eAlarma){ // alarma sonando
	if (!llave){ // si se corta la llave por dentro
      desarmarAlarma();
    }
    else{
		if (tiempoAlarma>0){
			tiempoAlarma--;
		}
		else{
			activaAlarmaPausa();
		}
	}
  }

  if (estadoAlarma==eAlarmaPausa){ // alarma sonando, pero en pausa
	if (!llave){ // si se corta la llave por dentro
      desarmarAlarma();
    }
    else{
		if (tiempoAlarmaPausa>0){
			tiempoAlarmaPausa--;
		}
		else{
			activaAlarma();
		}
	}
  }
  
  DEBUG(F("Estado = "));
  DEBUGDEC(estadoAlarma);
  
  // fin analisis según estado
  
  // actualizar el cicloTimer
  if (cicloTimer<16){
	  cicloTimer++;
  }
  else{
	  cicloTimer=0;
  }
  
  // actualizar el tiempo ping
  if (tiempoPing==0){
    despachaMensaje(F("Ping "), IDCasa, tXBeeReporte);
    nuevoTiempoPing();
  }
  else{
    tiempoPing--;
  }
  // actualizar salidas
  actualizaSalida(pbuzzer,estadoBuzzer);
  actualizaSalida(pSirena,estadoSirena);

  #ifdef ID1
  	actualizaSalida(pLEDPropio,*estadoLEDPropio);
  #else
  	actualizaSalida(pLED0,estadoLED0);
  #endif
  
  #ifdef ID2
  	actualizaSalida(pLEDPropio,*estadoLEDPropio);
  #else
  	actualizaSalida(pLED1,estadoLED1);
  #endif
  
  #ifdef ID3
  	actualizaSalida(pLEDPropio,*estadoLEDPropio);
  #else
  	actualizaSalida(pLED2,estadoLED2);
  #endif

}

void armarAlarma(){
  estadoAlarma=eArmada;
  estadoBuzzer=eBuzzerOff;
  *estadoLEDPropio=eLEDOn;
  despachaMensaje(F("Armada "), IDCasa, tXBeeReporte);
}

void desarmarAlarma(){
  estadoAlarma=eDesarmado;
  tiempoSalida=0;
  tiempoEntrada=0;
  tiempoAlarma=0;
  tiempoAlarmaPausa=0;
  estadoBuzzer=eBuzzerOff;
  estadoSirena=eSirenaOff;
  *estadoLEDPropio=eLEDOn;
  despachaMensaje(F("Desarmada "), IDCasa, tXBeeReporte);
}

void checkSensores(){
	if (sensor1){
		estadoAlarma=eEntrada;
		tiempoEntrada=SegEntrada;
		estadoBuzzer=eBuzzerOPulsoRapido;
		*estadoLEDPropio=eLEDPulsoRapido;
	}
	else{
		if (sensor2){
			activaAlarma();
		}
	}
}

void activaAlarma(){
    	estadoAlarma=eAlarma;
    	tiempoAlarma=SegAlarma;
    	estadoSirena=eSirenaOn;
    	estadoBuzzer=eBuzzerOPulsoRapido;
    	*estadoLEDPropio=eLEDPulsoRapido;
    	// envia mensaje
        despachaMensaje(F("Inicio Alarma "), IDCasa, tXBeeReporte);
}

void activaAlarmaPausa(){
	estadoAlarma=eAlarmaPausa;
	tiempoAlarmaPausa=SegAlarmaPausa;
	estadoSirena=eSirenaOff;
        despachaMensaje(F("Despacho Alarma "), IDCasa, tXBeeComunitario);
}

void actualizaSalida(byte pin, byte estado){
	boolean comando=false;
	if (estado==0){comando=false;}
	if (estado==1){comando=true;}
	if (estado==2){
		if ((cicloTimer/4)%2==0){comando=true;}else{comando=false;}
	}
	if (estado==3){
		if (cicloTimer%2==0){comando=true;}else{comando=false;}
	}
	digitalWrite(pin,comando);
}

void nuevoTiempoPing(){
  tiempoPing=SegPing+random(VarSegPing); 
}
