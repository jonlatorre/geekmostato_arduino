int rele = 4; 
int lm35 = 5; //pin de conexion del sensor
int temperatura = 0; //variable para la temperatura
int temperatura_limite = 16; //variable para la temperatura 
boolean encendida = 0; //
char inByte = 0;
char decena = 0;
char unidad = 0;

long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 5000;           // interval at which to blink (milliseconds)

void establishContact() {
  while (Serial.available() <= 0) {
    //Serial.print('A');   // send a capital A
    delay(300);
  }
}

void leer_temperatura () {
  //Calcula la temperatura usando como referencia 5v
   temperatura = (5.0 * analogRead(lm35)*100.0)/1024.0;
}

int convertirTempChar(char ch) {
  int res;
  //pasamos el char con el numero en ASCII a decimal (el cero es la posicion ASCII 48)
  res = ch - 48  ;
  return res;
}

void leer_limite () {
    establishContact();
    decena = Serial.read();
    unidad = Serial.read();
    temperatura_limite = convertirTempChar(decena)*10+convertirTempChar(unidad);

    /*Serial.print("Vamos a poner el limite: ");
    Serial.println(temperatura_limite);*/
    //devolvemos 1 para decir que todo a ido OK
    Serial.println("1");
}

void controlar_calefa() {
  //Controlamos el intervalo
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // si ya hemos sobrepasado el intervalo nos ponemos a trabajar!
    previousMillis = currentMillis;  
    leer_temperatura();
    //Aqui debera haber una histeresis que evite que salte, por ejemplo esperar 2 cilcos antes de saltar.
    if ( temperatura < temperatura_limite ) {
      //Serial.println("Hace frio");
      digitalWrite(rele, HIGH);
      encendida = 1;
    }
    else {
      //Serial.println("Ya hace calor");
      digitalWrite(rele, LOW);    
      encendida = 0;
    } 
  }
}

void mandar_temperatura() {
  leer_temperatura();
  //Serial.print("Tenemos una temp de : ");
  Serial.print(temperatura);
  Serial.print(":");
  Serial.print(temperatura_limite);
  Serial.print(":");
  Serial.println(encendida);
} 

void setup(){
   pinMode(lm35, INPUT); //declara pin del LM de entrada
   //initialize the digital pin as an output.
   pinMode(rele, OUTPUT);    
   Serial.begin (9600); //inicia comunicacion serial
   digitalWrite(rele, LOW);    // turn the LED off by making the voltage LOW
   //Serial.println("Conf. OK");
   //establishContact();
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (inByte=='g') {
      mandar_temperatura();
    } 
    else if (inByte=='s') {
      leer_limite();
    }
  }
  controlar_calefa();
}
