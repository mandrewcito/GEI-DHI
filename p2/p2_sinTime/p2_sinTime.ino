
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
boolean output = HIGH;
String str="";
volatile int hora=0;
volatile int minuto=0;
volatile int segundo=0;
char horaE[3];
char minutoE[3];
char segundoE[3];

void flash() {
  digitalWrite(rojo, output);
  digitalWrite(verde, !output);
  output = !output;
  segundo++;
  if(segundo == 60) {
    minuto++;
    segundo=0;
    if (minuto==60){
      hora++;
      minuto=0;
     if (hora==24){
     minuto=0;segundo=0;hora=0;
     }
  }
  }
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);
  Serial.print(":");
  Serial.print(segundo);
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
  sleep_enable();    
}
void wakeUpNow(){
  sleep_disable(); 
}

void sleepNow(){
  sleep_mode();
}

// the loop routine runs over and over again forever:
void loop() {
  /* si lo introducido no es valido */
    while(Serial.available() > 0)
    {
        str = Serial.readStringUntil('\n');
        horaE[0]=str.charAt(0);horaE[1]=str.charAt(1);minutoE[0]=str.charAt(3);minutoE[1]=str.charAt(4);segundoE[0]=str.charAt(6);segundoE[1]=str.charAt(7);
        if(atoi(horaE)>23 || atoi(minutoE)>59 ||atoi(segundoE)>59){
          Serial.println("paso a modo power-down");
          delay(30);
          digitalWrite(rojo, LOW);
          digitalWrite(verde, LOW);
          attachInterrupt(1, wakeUpNow, HIGH);
          sleepNow();
          detachInterrupt(1); 
        }else{
          hora=atoi(horaE);
          minuto=atoi(minutoE);
          segundo=atoi(segundoE);
        }
    }
}
