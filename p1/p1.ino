const int buttonPin =3 ;
const int verde =5;
const int rojo = 10;
volatile int estado = 0;
int buttonState=0;
unsigned long time;
volatile unsigned long time2;
unsigned long timeResult;
void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(buttonPin, INPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
}

void detector(){
  if(digitalRead(buttonPin)== HIGH)
    estado=1;
}
void getTime(){
  if(digitalRead(buttonPin)== HIGH)
    time2= millis();
}

void loop(){
  digitalWrite(verde, HIGH);
  digitalWrite(rojo, LOW);
  Serial.print("pulsar para empezar\n");
  while (digitalRead(buttonPin)== LOW);
  digitalWrite(verde, LOW);
  digitalWrite(rojo, LOW);
  attachInterrupt(1,detector,RISING);
  estado = 0;
  delay(400);
  delay(random(0,800));
  detachInterrupt(1);
  if(estado==HIGH){
    Serial.print("NO SE PRECIPITE. Intente de nuevo \n");
  }
  else{
    time=0;
    time2=0;
    Serial.print("empieza el juego \n"); 
    digitalWrite(rojo, HIGH);
    time = millis();
    attachInterrupt(1,getTime,RISING);
    while (time2==0);
    detachInterrupt(1);
    timeResult=time2-time;
    Serial.print(timeResult);
    if(timeResult<100)
    {
      Serial.print("ms ENHORABUENA Excelentes reflejos\n");
    }
    else if(timeResult>200){
      Serial.print("ms Intente de nuevo \n");
    }
    else{
      Serial.print("ms BIEN, buenos reflejos \n");
    }

  }
  delay(2000);
}




