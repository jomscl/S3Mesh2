void atiendeTimer(){
  // actualiza las entradas
  llave=digitalRead(pllave);
  sensor1=digitalRead(psensor1);
  sensor2=digitalRead(psensor2);
  boton=digitalRead(pboton);
  
  // analisis según estado
  if (estado==eDesarmado){ // alarma desarmada
    if (llave){ // se acaba de iniar el armado de la alarma
      tiempoSalida=SegSalida;
      estado=eArmandose;
    }
  }
  if (estado==eArmandose){ // alarma armandose, descontando tiempo
    if (tiempoSalida>0){
      tiempoSalida--;
    }
    else{
      armar();
     
    }
  }
  
  if (estado==eArmada){ // alarma armada
    if (!llave){ // si se corta la llave por dentro
      desarmar();
    }
    else{
      
    }
  }
  
  // fin analisis según estado
  
  
  // actualizar salidas
}

void armar(){
  estado=eArmada;
}

void desarmar(){
  estado=eDesarmado;
}
