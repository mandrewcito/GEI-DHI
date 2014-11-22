#include <Wire.h>
const int buttonPin =7;
#define DIRECCION 0x38
#define L1 0x0E
#define L2 0x0D
#define L3 0x0B
#define L4 0x07
const int dir1=0;
const int dir2=1;
const int dir3=2;
const int dir4=3;
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <avr/wdt.h>
int addr = 0;
int P1;
int P2;
int P3;
int P4;
int T1=1000;
int T2=500;
int T3=250;
LiquidCrystal lcd(12,11,5,4,3,2);

void clearEEprom(){
for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
}

void iniciar(){
EEPROM.write(dir1,1);
EEPROM.write(dir2,2);
EEPROM.write(dir3,5);
EEPROM.write(dir4,10);
}
void vuelta(int del){
delay(del);
escribir(L1);
delay(del);
escribir(L2);
delay(del);
escribir(L3);
delay(del);
escribir(L4);
wdt_reset();
}
void ruleta(){
vuelta(T1);
vuelta(T2);
vuelta(T3);
vuelta(T3);
}
void flash() {

}
void rellenaLcd(){
lcd.clear();
lcd.setCursor(0,0);
lcd.print("P1:");
lcd.print(P1);
lcd.setCursor(8,0);
lcd.print("P2:");
lcd.print(P2);
lcd.setCursor(0,1);
lcd.print("P3:");
lcd.print(P3);
lcd.setCursor(8,1);
lcd.print("P4:");
lcd.print(P4);
analogWrite(6,60);
}
void escribir(byte txData){
    Wire.beginTransmission(DIRECCION);
    Wire.write(txData);
    Wire.endTransmission(DIRECCION);
}
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  Wire.begin();
  if (digitalRead(buttonPin)== HIGH){
    iniciar();
  }
  wdt_enable(WDTO_8S);
  lcd.begin(16,2);
}
void loop() {
  Serial.print("Pulsa para empezar");
  wdt_reset();
  P1=EEPROM.read(dir1);
  P2=EEPROM.read(dir2);
  P3=EEPROM.read(dir3);
  P4=EEPROM.read(dir4);
  rellenaLcd();
  while(digitalRead(buttonPin)!= HIGH);
    int P =P1+P2+P3+P4;
    ruleta();
    escribir(0x00);
    delay(250);
    if (P==0){
    Serial.println("No quedan premios");
    }else{
      int num=random(1,P);
      if(num<=P1){
        P1--;
        EEPROM.write(dir1,P1);
        escribir(L1);
      }else if(num<=P1+P2){
        P2--;
        EEPROM.write(dir2,P2);
        escribir(L2);
      }else if (num<=P1+P2+P3){
        P3--;
        EEPROM.write(dir3,P3);
        escribir(L3);
      }else{
        P4--;
        EEPROM.write(dir4,P4);
        escribir(L4);
      }
    
  }  // dejar encencida la lampara del premio y guardar en EEPROM
    Serial.print("pulsado boton");
 
  delay(2000);
}

