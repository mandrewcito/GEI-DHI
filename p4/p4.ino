#include <MsTimer2.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int buttonPin =7;
#define L1 A0
#define L2 A1
#define L3 A2
#define L4 A3
int brillos[5]={255,255,255,255} ;// todas apagadas al principio 
String str="";
int modificado=0;
volatile int pulsado=0;
volatile int ciclos=0;

void rellenaLcd(){
lcd.clear();
lcd.setCursor(0,0);
lcd.print("L1:");
lcd.print(abs(brillos[0]-255));
lcd.setCursor(8,0);
lcd.print("L2:");
lcd.print(abs(brillos[1]-255));
lcd.setCursor(0,1);
lcd.print("L3:");
lcd.print(abs(brillos[2]-255));
lcd.setCursor(8,1);
lcd.print("L4:");
lcd.print(abs(brillos[3]-255));
analogWrite(6,60);
}
void incrementar(int incremento){
brillos[0]=brillos[0]-incremento;
brillos[1]=brillos[1]-incremento;
brillos[2]=brillos[2]-incremento;
brillos[3]=brillos[3]-incremento;
Serial.println("rellenado!");
if (brillos[0]<0)
  brillos[0]=0;
  if (brillos[1]<0)
  brillos[1]=0;
  if (brillos[2]<0)
  brillos[2]=0;
  if (brillos[3]<0)
  brillos[3]=0;
rellenaLcd();
luces();
}
void flash(){
  if (digitalRead(buttonPin)==HIGH){
    pulsado=1;
    ciclos=ciclos+1;
    Serial.println(ciclos);
    if (ciclos>4){
    ciclos=0;
    incrementar(50);
    }
  }else{
    if (pulsado==1){
      if (ciclos==4){
        incrementar(50);
        ciclos=0;
      }else{
      incrementar(1); 
      ciclos=0;
      }
    }
    pulsado=0;
  }
}
void luces(){
analogWrite(L1,brillos[0]);
analogWrite(L2,brillos[1]);
analogWrite(L3,brillos[2]);
analogWrite(L4,brillos[3]);

}

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(L4,OUTPUT);
  lcd.begin(16,2);
  MsTimer2::set(250, flash); // 1000ms period
  MsTimer2::start();
  rellenaLcd();
  luces();
}

void loop() {
    while(Serial.available() > 0)
    {
      str = Serial.readStringUntil('\n');
      char charBuf[50];
      str.toCharArray(charBuf, 50);
      char* command = strtok(charBuf, " ");
      int i=0;
      while(command!=0){
        int num = atoi(command);
        if (num<=255 &&num>=0)
          brillos[i]=abs(num-255);
        command=strtok(0, " ");   
        i++;     
      }
      modificado=1;
     
     if (modificado){
       rellenaLcd();
       modificado=0;
     }
     luces();
    }
}
