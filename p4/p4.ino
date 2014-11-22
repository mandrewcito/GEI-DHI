#include <MsTimer2.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
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

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  lcd.begin(16,2);
}

void loop() {

}
