void atiendeTimer(){
  // actualiza las entradas. Son invertidas por el pull up resistor.
  llave=digitalRead(pllave);
  sensor1=!digitalRead(psensor1);
  sensor2=digitalRead(psensor2);
  boton=!digitalRead(pboton);
  
  // analisis según estado
  if (estadoAlarma==eDesarmado){ // alarma desarmada
    if (llave){ // se acaba de iniar el armado de la alarma
		tiempoSalida=SegSalida;
		estadoAlarma=eArmandose;
		imprimeEstadoAlarma();
		estadoBuzzer=eBuzzerPulsoLento;
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
    despachaMensaje(mPing, IDCasa); // reporte
    nuevoTiempoPing();
  }
  else{
    tiempoPing--;
  }

  // actualizar las alertas de las otras casas
	#ifndef ID1
		if (tiempoInformeLED0>0){tiempoInformeLED0--;}
		else{estadoLED0=eLEDOff;}
	#endif
	#ifndef ID2
		if (tiempoInformeLED1>0){tiempoInformeLED1--;}
		else{estadoLED1=eLEDOff;}
	#endif
	#ifndef ID3
		if (tiempoInformeLED2>0){tiempoInformeLED2--;}
		else{estadoLED2=eLEDOff;}
	#endif
  // ver si de debe apagar el buzzer
  if (tiempoInformeLED0+tiempoInformeLED1+tiempoInformeLED2+tiempoEntrada+tiempoSalida+tiempoAlarma+tiempoAlarmaPausa ==0){estadoBuzzer=eBuzzerOff;}
  // si se presiona el boton de reset
  if (boton){
	  DEBUGLN(F("Boton"));
	  tiempoInformeLED0=0;
	  estadoLED0=eLEDOff;
	  tiempoInformeLED1=0;
	  estadoLED1=eLEDOff;
	  tiempoInformeLED2=0;
	  estadoLED2=eLEDOff;
	  estadoBuzzer=eBuzzerOff;
  }

  // actualizar salidas
  actualizaSalida(pbuzzer,estadoBuzzer,true);
  actualizaSalida(pSirena,estadoSirena,false);

  #ifdef ID1
  	actualizaSalida(pLEDPropio,*estadoLEDPropio,false);
  #else
  	actualizaSalida(pLED0,estadoLED0,false);
  #endif
  
  #ifdef ID2
  	actualizaSalida(pLEDPropio,*estadoLEDPropio,false);
  #else
  	actualizaSalida(pLED1,estadoLED1,false);
  #endif
  
  #ifdef ID3
  	actualizaSalida(pLEDPropio,*estadoLEDPropio,false);
  #else
  	actualizaSalida(pLED2,estadoLED2,false);
  #endif

}

void armarAlarma(){
  estadoAlarma=eArmada;
  imprimeEstadoAlarma();
  estadoBuzzer=eBuzzerOff;
  *estadoLEDPropio=eLEDOn;
  despachaMensaje(mArmada, IDCasa); // reporte
}

void desarmarAlarma(){
  estadoAlarma=eDesarmado;
  imprimeEstadoAlarma();
  tiempoSalida=0;
  tiempoEntrada=0;
  tiempoAlarma=0;
  tiempoAlarmaPausa=0;
  estadoBuzzer=eBuzzerOff;
  estadoSirena=eSirenaOff;
  *estadoLEDPropio=eLEDOff;
  despachaMensaje(mDesarmada, IDCasa); // reporte
}

void checkSensores(){
	if (sensor1){
		estadoAlarma=eEntrada;
		imprimeEstadoAlarma();
		tiempoEntrada=SegEntrada;
		estadoBuzzer=eBuzzerPulsoLento;
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
		imprimeEstadoAlarma();
    	tiempoAlarma=SegAlarma;
    	estadoSirena=eSirenaOn;
    	estadoBuzzer=eBuzzerPulsoRapido;
    	*estadoLEDPropio=eLEDPulsoRapido;
    	// envia mensaje
        despachaMensaje(mInicioAlarma, IDCasa); // reporte
}

void activaAlarmaPausa(){
	estadoAlarma=eAlarmaPausa;
	imprimeEstadoAlarma();
	tiempoAlarmaPausa=SegAlarmaPausa;
	estadoSirena=eSirenaOff;
        despachaMensaje(mDespachoAlarma, IDCasa ); // comunitario
}

void actualizaSalida(byte pin, byte estado, boolean tono){
	boolean comando=false;
	
        // analisis del tipo de accion, basado en el tipo de estado
        if (estado==0){comando=false;}
	if (estado==1){comando=true;}
	if (estado==2){
		if ((cicloTimer/4)%2==0){comando=true;}else{comando=false;}
	}
	if (estado==3){
		if (cicloTimer%2==0){comando=true;}else{comando=false;}
	}

        // analisis de como se debe efectuar la salida, ya sea por tono o por digital write
	if (tono){
          if (comando){tone(pin,freqBuzzer);}
          else{noTone(pin);}
        }
        else{digitalWrite(pin,comando);}
}

void nuevoTiempoPing(){
  tiempoPing=SegPing+random(VarSegPing); 
}

void imprimeEstadoAlarma(){
	DEBUG(F("Estado = "));
	DEBUGLN(estadoAlarma);
}
