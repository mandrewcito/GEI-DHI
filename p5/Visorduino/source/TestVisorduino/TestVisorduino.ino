          
// Genera dos ondas sinusoidales y una cuadrada y las
// envía al puerto serie para visualización en Visorduino.
// José J. Lamas. UDC. 2013.

  #include <MsTimer2.h>
  
  // Valores constantes
  const float pi= 3.1416;
  const unsigned int N= 200; // Número de puntos por ciclo
  const unsigned int Ts= 5; // Intervalo entre puntos en ms
  const unsigned int Vm= 50; // Voltaje máximo*10 en V

  // Variables globales
  volatile unsigned int n= 0; // Número del punto (0 a N-1)
  volatile unsigned int ch[6]; // Valores calculados  
  
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
    ch[0]= 512 + 511*sin(n*2*pi/N); // DC + Seno en CH1 de 1 Hz    
    ch[1]= 512 + 256*cos(n*4*pi/N); // DC + Coseno en CH2 de 2 Hz   
    if (n < N/2) ch[2]= 1023; // Cuadrada de 1 Hz
    else ch[2]= 0;   
    ch[3]= 768; // DC
    ch[4]= 512; // DC
    ch[5]= 0; // DC  
    if (n++== N) { // Incrementa número índice del punto
    n= 0;
    }  
    // Envía mensaje de datos cada Ts ms a Visorduino
    Serial.print("A6"); // Cabecera de mensaje
    Serial.write(Ts); // Envía intervalo tiempo Ts  
    Serial.write(Vm); // Envía voltaje máximo   
    for (int i=0; i<6; i++) {
      Serial.write(ch[i] & 0xFF); //  Envía Byte bajo de Chi
      Serial.write((ch[i] >> 8) & 0x03); // y byte alto  
    }
  }
  
