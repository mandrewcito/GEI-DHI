//p5 visorduino 

  #include <MsTimer2.h>
  
  // Valores constantes
  const float pi= 3.1416;
  const unsigned int N=200; // Número de puntos por ciclo
  const unsigned int Ts= 5;// Intervalo entre puntos en ms
  const unsigned int Vm= 50; // Voltaje máximo*10 en V

  // Variables globales
  volatile unsigned int n= 0; // Número del punto (0 a N-1)
  volatile unsigned int ch[7]; // Valores calculados  
  
  void setup()  
  {  
    Serial.begin(1000000);  
    MsTimer2::set(Ts, actualizar); // Int. Timer2 cada Ts ms
    MsTimer2::start();
  }  
      
  void loop() { // El bucle principal no hace nada  
  }

  // Rutina de atención de interrupción de Timer2 cada Ts ms
  void actualizar() {
    float seno2 = sin(n*2*pi/N);
    float coseno20 =cos(n*20*pi/N);
    float coseno256= 256*coseno20;
    ch[0]= 512 + 256*seno2; // DC + Seno en CH1 de 1 Hz    
    ch[1]= 512 + coseno256; // DC + Coseno en CH2 de 2 Hz   
    ch[2]= 512 +(204+102*seno2)*(coseno20);
    // CH4 .....
    float t=(float)(4.0*n)/N;
    if (n>=0 && n<N/4){
        // A -+ m*t    // 51
        ch[3]=(float)512.0-(256.0*(0.0-t));
    }
    else{ 
      if (n<N*3/4 && n>=N/4){
          ch[3]=(float)512.0+(256.0*(2-t));
      }
         else{
          if (n>=N*3/4 && n<N){
          ch[3]=(float)512.0-(256.0*(4.0-t));
          }
         }
    }
    //CH5 .....
    if (n<(N/5)){
      ch[4]=768;
    }
    else{
      ch[4]=256;
    }
    //CH6 .....
    if (n<(N/5)){
      ch[5]=512+coseno256;
    }
    else{
      ch[5]=512-coseno256;
    }
      
    if (n++== N) { // Incrementa número índice del punto
    n= 0;
    }  
    int ver=0;
    // Envía mensaje de datos cada Ts ms a Visorduino
    Serial.print("A6"); // Cabecera de mensaje
    Serial.write(Ts); // Envía intervalo tiempo Ts  
    Serial.write(Vm); // Envía voltaje máximo   
    for (int i=0; i<7; i++) {
      Serial.write(ch[i] & 0xFF); //  Envía Byte bajo de Chi
      Serial.write((ch[i] >> 8) & 0x03); // y byte alto  
    }
  }
  
