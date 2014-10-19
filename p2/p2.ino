#include <Time.h>

/*
p2 DHI andres baamonde lozano
hora en hh:mm:ss(actualiza mediante interrupcion) por USART led rojo y verde encendidos con ss%2 (alternando)
la puesta en hora enviando los datos desde la misma ventana de monitorización del
puerto serie del IDE Arduino 

->poner en modo de bajo consumo power-down mediante el envío de un valor de hora,
minuto o segundo que exceda el valor máximo permitido, se apagan los 2 leds
y con el pulsador (mediante interrupt ) se reinicia 

*/
#include <MsTimer2.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
const int buttonPin =3;
const int verde =5;
const int rojo = 10;
time_t t;
boolean output = HIGH;
String str="";
char hora[3];
char minuto[3];
char segundo[3];

void flash() {
  digitalWrite(rojo, output);
  digitalWrite(verde, !output);
  output = !output;
  t = now();
  Serial.print(hour(t));
  Serial.print(":");
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
  Serial.print("\n");
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  MsTimer2::set(1000, flash); // 1000ms period
  MsTimer2::start();
}
void wakeUpNow(){
  sleep_disable(); 
}

void sleepNow(){
  sleep_enable();    
  sleep_mode();
}

// the loop routine runs over and over again forever:
void loop() {
  /* si lo introducido no es valido */
    if(Serial.available() > 0)
    {
        str = Serial.readStringUntil('\n');
        hora[0]=str.charAt(0);hora[1]=str.charAt(1);minuto[0]=str.charAt(3);minuto[1]=str.charAt(4);segundo[0]=str.charAt(6);segundo[1]=str.charAt(7);
        if(atoi(hora)>23 || atoi(minuto)>59 ||atoi(segundo)>59){
          attachInterrupt(1, wakeUpNow, HIGH);
          Serial.print("paso a modo power-down");
          digitalWrite(rojo, LOW);
          digitalWrite(verde, LOW);
          sleepNow();
          detachInterrupt(1); 
        }else{
          setTime(atoi(hora),atoi(minuto),atoi(segundo),0,0,0);
        }
    }
  
}
