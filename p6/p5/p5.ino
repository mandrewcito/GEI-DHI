//p5 visorduino 
  #include <MsTimer2.h>
  const int buttonPin1 =2 ;
  const int buttonPin2 =3 ;
  const int salida1= 9;
  const int salida2 =10;
  #define ENTRADA1 A0
  #define ENTRADA2 A1
  #define AMP1 A2
  #define AMP2 A3
  #define FREC A4
  volatile int par=0;
  volatile int impar=1;
  // Valores constantes
  const float pi= 3.1416;
  volatile int nmuestreo=0;
  volatile int N=200; // Número de puntos por ciclo
  const unsigned int Ts= 5;// Intervalo entre puntos en ms
  const unsigned int Tg=5; // muestreo
  volatile int TgTemp=0; // acumulador para muestrear
  const unsigned int Vm= 50; // Voltaje máximo*10 en V
  volatile int VmPar=512;
  volatile int VmImpar=512;
  volatile int VefPar=512;
  volatile int VefImpar=512;
  // Variables globales
  volatile unsigned int n= 0; // Número del punto (0 a N-1)
  volatile unsigned long ch[7]; // Valores calculados  
  volatile unsigned long envio[7]; // Valores calculados  
  float Xmed1=0.0;
  float xmed2=0.0;
  float xef1=0.0;
  float xef2=0.0;
  volatile float factor_amp1;
  volatile float factor_amp2;
  volatile int factor_frec;
  unsigned long onda1=0;
  unsigned long onda2=0;
  void setup()  
  { Serial.begin(1000000);
    pinMode(salida1, OUTPUT);
    pinMode(salida2, OUTPUT);
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    //attachInterrupt(1,detector,RISING);
   // attachInterrupt(2,detector2,RISING);
    MsTimer2::set(Ts, actualizar); // Int. Timer2 cada Ts ms
    MsTimer2::start();
  }
  /*
    void detector2(){
    impar=impar+2;
    if (impar>=6)impar=1; 
}
  void detector(){
    par=par+2;
    if (par>=6)par=0;   
}*/
  void loop() {
    factor_amp1=analogRead(AMP1)/1023.0;
    factor_amp2=analogRead(AMP2)/1023.0;
    // mapeamos la frecuencia
    factor_frec=map(analogRead(FREC),0,1023,50,4000);
    if (digitalRead(buttonPin1)==0){
          par=par+2;
          if (par>=6)par=0;  
    }
    if (digitalRead(buttonPin2)==0){
          impar=impar+2;
           if (impar>=6)impar=1; 
    }
  }
  
 void enviar(){
    Serial.print("A6"); // Cabecera de mensaje
    Serial.write(Ts); // Envía intervalo tiempo Ts  
    Serial.write(Vm); // Envía voltaje máximo   
    for (int i=0; i<7; i++) {
      Serial.write(envio[i] & 0xFF); //  Envía Byte bajo de Chi
      Serial.write((envio[i] >> 8) & 0x03); // y byte alto 
    }
 }
  // Rutina de atención de interrupción de Timer2 cada Ts ms
  void actualizar() {
    /*valores funciones */
    float seno2 = sin(n*2*pi/N);
    float coseno20 =cos(n*20*pi/N);
    float coseno256= 256*coseno20;
    ch[0]= 512.0 + factor_amp1*256.0*seno2; // DC + Seno en CH1 de 1 Hz    SENO
    ch[1]= 512.0 + factor_amp2*coseno256; // DC + Coseno en CH2 de 2 Hz   SENO
    // CH2 ..... triangulo
    float t=(float)(4.0*n)/N;
    if (n>=0 && n<N/4){
        // A -+ m*t    // 51
        ch[2]=(float)512.0-(factor_amp1*256.0*(0.0-t));
    }
    else{ 
      if (n<N*3/4 && n>=N/4){
          ch[2]=(float)512.0+(factor_amp1*256.0*(2-t));
      }
         else{
          if (n>=N*3/4 && n<N){
          ch[2]=(float)512.0-(factor_amp1*256.0*(4.0-t));
          }
         }
    }
    //CH4 .....
    if (n<(N/5)){
      ch[3]=factor_amp2*768.0;
    }
    else{
      ch[3]=factor_amp2*256.0;
    }
   ch[4]=factor_amp1*768.0;
   ch[5]=factor_amp2*256.0;
   /* fin funciones */
    
   /* hallar valores eficaz y medio cada N intervalos*/
   /*salida -> ondas par e impar con map */
   /* muestrear las ondas cada Tg */
    if (TgTemp++==Tg){
    noInterrupts();
    analogWrite(salida1,ch[par]);
    analogWrite(salida2,ch[impar]);
    //onda1 = analogRead(ENTRADA1);
    //onda2 = analogRead(ENTRADA2);
    onda1=ch[par];
    onda2=ch[impar];
    interrupts();
       /* acumular valores eficaz y medio */
    Xmed1=Xmed1+onda1;
    xmed2=xmed2+onda2;
    xef1=xef1+(onda1*onda1);
    xef2=xef2+(onda2*onda2);
    if (nmuestreo++==N)
    {
      VmPar = Xmed1/(float)N;
      VmImpar = xmed2/(float)N;
      float xef11=xef1;
      float xef22=xef2;
      VefPar= sqrt((float)xef1/(float)N);
      VefImpar= sqrt((float)xef2/(float)N);
      Xmed1=0;
      xmed2=0;
      xef1=0;
      xef2=0;
      nmuestreo=0;
      N=factor_frec;
    }
    TgTemp=0;
    }
    if (n++== N) { // Incrementa número índice del punto
    n=0;
    }  
    envio[0]=onda1;//v instantaneo par 
    envio[1]=onda2; //v instantaneo impar 
    envio[2]= VmPar;// v medio par 
    envio[3]= VmImpar;//v medio impar
    envio[4]= VefPar;// v eficaz par
    envio[5]= VefImpar;//v eficaz impar 
    enviar();
  }
  
