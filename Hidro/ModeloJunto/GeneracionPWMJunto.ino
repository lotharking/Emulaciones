//#include <TimerOne.h>


int incomingByte = 0;
int i1 = 0, i2 = 0, i3 = 0;
int pb=0, qb=0, pd=0, qd=0;
int flag=0;
//int i3 = 0, vl = 0;
const int ledPin =  LED_BUILTIN;
char aa, tramRasp[9],tramRasp2[9];
String a,b, spm, sqm, svl, sv2, sv3, ssoc;
//String si3;
int pm, qm, vl, v2, v3,vl2, soc;

//volatile boolean pinra = false;
bool pinra=false;
bool bi1 = false, bi2 = false,bi3 = false, bi4 = false;
int cont=0;
char frst, scnd, thrd, frth;

int dutycycle_156 [625] = {128, 129, 130, 130, 131, 132, 133, 134, 135, 135, 136, 137, 138, 139, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 148, 149, 150, 151,
                           152, 153, 153, 154, 155, 156, 157, 157, 158, 159, 160, 161, 162, 162, 163, 164, 165, 166, 167, 167, 168, 169, 170, 171, 171, 172, 173, 174, 175,
                           176, 176, 177, 178, 179, 180, 180, 181, 182, 183, 184, 185, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 194, 195, 196, 197, 198, 198,
                           199, 200, 201, 202, 203, 203, 204, 205, 206, 207, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 216, 217, 218, 219, 220, 221, 221, 222,
                           223, 224, 225, 225, 226, 227, 228, 229, 230, 230, 231, 232, 233, 234, 234, 235, 236, 237, 238, 239, 239, 240, 241, 242, 243, 244, 244, 245, 246,
                           247, 248, 248, 249, 250, 251, 252, 253, 253, 254, 255, 255, 255, 254, 253, 253, 252, 251, 250, 249, 248, 248, 247, 246, 245, 244, 244, 243, 242,
                           241, 240, 239, 239, 238, 237, 236, 235, 234, 234, 233, 232, 231, 230, 230, 229, 228, 227, 226, 225, 225, 224, 223, 222, 221, 221, 220, 219, 218,
                           217, 216, 216, 215, 214, 213, 212, 212, 211, 210, 209, 208, 207, 207, 206, 205, 204, 203, 203, 202, 201, 200, 199, 198, 198, 197, 196, 195, 194,
                           194, 193, 192, 191, 190, 189, 189, 188, 187, 186, 185, 185, 184, 183, 182, 181, 180, 180, 179, 178, 177, 176, 176, 175, 174, 173, 172, 171, 171,
                           170, 169, 168, 167, 167, 166, 165, 164, 163, 162, 162, 161, 160, 159, 158, 157, 157, 156, 155, 154, 153, 153, 152, 151, 150, 149, 148, 148, 147,
                           146, 145, 144, 144, 143, 142, 141, 140, 139, 139, 138, 137, 136, 135, 135, 134, 133, 132, 131, 130, 130, 129, 128, 127, 126, 126, 125, 124, 123,
                           122, 121, 121, 120, 119, 118, 117, 117, 116, 115, 114, 113, 112, 112, 111, 110, 109, 108, 108, 107, 106, 105, 104, 103, 103, 102, 101, 100, 99,
                            99, 98, 97, 96, 95, 94, 94, 93, 92, 91, 90, 89, 89, 88, 87, 86, 85, 85, 84, 83, 82, 81, 80, 80, 79, 78, 77, 76, 76, 75, 74, 73, 72, 71, 71, 70,
                            69, 68, 67, 67, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 58, 57, 56, 55, 54, 53, 53, 52, 51, 50, 49, 49, 48, 47, 46, 45, 44, 44, 43, 42, 41, 40,
                            40, 39, 38, 37, 36, 35, 35, 34, 33, 32, 31, 31, 30, 29, 28, 27, 26, 26, 25, 24, 23, 22, 22, 21, 20, 19, 18, 17, 17, 16, 15, 14, 13, 12, 12, 11,
                            10, 9, 8, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 22, 23, 24,
                            25, 26, 26, 27, 28, 29, 30, 31, 31, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 49, 49, 50, 51, 52, 53, 53,
                            54, 55, 56, 57, 58, 58, 59, 60, 61, 62, 62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 71, 72, 73, 74, 75, 76, 76, 77, 78, 79, 80, 80, 81, 82, 83,
                            84, 85, 85, 86, 87, 88, 89, 89, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99, 99, 100, 101, 102, 103, 103, 104, 105, 106, 107, 108, 108, 109, 110,
                           111, 112, 112, 113, 114, 115, 116, 117, 117, 118, 119, 120, 121, 121, 122, 123, 124, 125, 126, 126, 127, 128};

          
int puntero_sin0   = 157;    // Usada para moverse por los valores de la tabla. Su máximo será 625. Representa la señal senoidal sin desfase. Apunta a DC = 255.
int puntero_sin120 = 547;    // Usada para moverse por los valores de la tabla. Su máximo será 625. Representa la señal senoidal con desfase = 120. Apunta a DC = 64.
int puntero_sin240 = 391;    // Usada para moverse por los valores de la tabla. Su máximo será 625. Representa la señal senoidal con desfase = 240. Apunta a DC = 64.

  /* Ahora se llevará acabo la configuración de los timers de la placa arduino, ya que se requiere la modificación de estos
     para la correcta generación de las señales PWM. Los registros de control serán modificados mediante hexadecimal. */

void setup() {

  // Configuramos los pines de PWM como salidas

  Serial.begin(115200);
  pinMode (3, OUTPUT);  // Conectado a transistor Q6. Par transistores para senoidal con fase 240
  pinMode (5, OUTPUT);  // Conectado a transistor Q2. Par transistores para senoidal con fase 0
  pinMode (6, OUTPUT);  // Conectado a transistor Q1. Par transistores para senoidal con fase 0
  pinMode (9, OUTPUT);  // Conectado a transistor Q3. Par transistores para senoidal con fase 120
  pinMode (10, OUTPUT); // Conectado a transistor Q4. Par transistores para senoidal con fase 120
  pinMode (11, OUTPUT); // Conectado a transistor Q5. Par transistores para senoidal con fase 240
  
  // Configuramos los timers mediante los registros de control.
  
  TCCR0A = 0xA1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 1. Salidas no invertidas.
  TCCR0B = 0xC1;
  TCCR1A = 0xA1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 1. Salidas no invertidas.
  TCCR1B = 0xC1;
  TCCR2A = 0xA1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 1. Salidas no invertidas.
  TCCR2B = 0xC1;
}

void sendToRasp() {
  memset(tramRasp,0,sizeof(tramRasp));
      //sprintf(tramRasp, "f%07de",i1);
      Serial.print('f');
      Serial.print(i1);
      Serial.print(',');
      Serial.print(i2);
      Serial.print(',');
      Serial.print(i3);
      Serial.print(',');
      Serial.print(i2);
      Serial.println('e');
      Serial.flush();  
}

//void receiveRaspData() {//v0001500w0002000z0002500\n
//  memset(&aa,0,sizeof(aa));
//  while (Serial.available() > 0) {
//    aa = Serial.read();  
//    a += aa;
//    if (aa == '\n') {
//      frst = a.charAt(0);
//      if (frst == 'v') {
//        flag=1;
//        svl= a.substring(1, 8);        
//        vl = svl.toInt();
//        a = "";
//      }
//      Serial.flush();
//      a = "";
//    }    
//  }
//}

void receiveRaspData() {
  memset(&aa,0,sizeof(aa));
  while (Serial.available() > 0) {
    aa = Serial.read();  
    a += aa; 
    if (aa == '\n') {
      frst = a.charAt(0);
      frth = a.charAt(16);
      scnd = a.charAt(8);
      if (frst == 'v') {
        flag=1;
        svl= a.substring(1, 8); 
        vl = svl.toInt();
        //a = "";
      }      
      if (scnd == 'w') {
        flag=1;
        sv2= a.substring(9, 16);        
        v2 = sv2.toInt();
        //a = "";
      }
      
      if (frth == 'z') {
        flag=1;
        sv3= a.substring(17, 24);        
        v3 = sv3.toInt();
        //a = "";
      }
      Serial.flush();
      a = "";
    }    
  }
}

void loop() {

  // Programa principal, se usarán directamente los registros de comparación porque es más rápido que usar las funciones propias de arduino.
  sendToRasp();
  receiveRaspData();
  //Serial.println(vl);
  i1 = int((((int(vl)-1000)*0.128)));
  i2 = int((((int(v2)-1000)*0.128)));
  i3 = int((((int(v3)-1000)*0.128)));
  //Serial.println(vl);
  OCR0A = i2;    // Registro de comparación para pin A del timer 0. Genera la PWM para Q1.

  /* Este es un retardo de 4.875 us utilizado para crear un tiempo muerto entre pares de transistores y evitar cortocircuitos en los mismos.
     Demostración: (1+1+(1+2)*25+1)/16000000 = 4.875 us*/

//  asm volatile (
//        "clr r16 \n"       // 1 ciclo de reloj.
//        "ldi r16, 0x14 \n" // 1 ciclo de reloj. 26 vueltas cargadas.
//        "1: dec r16 \n"    // 1 ciclo de reloj
//        "brne 1b  \n"      // 2 ciclos de reloj para retornar y 1 para salir
//      );
      
   OCR0B = i2;    // Registro de comparación para pin B del timer 0. Genera la PWM para Q2.
   OCR1A = i2;  // Registro de comparación para pin A del timer 1. Genera la PWM para Q3.
  
  // Retardo 4.875 us.
  
//   asm volatile (
//      "clr r16 \n"       // 1 ciclo de reloj.
//      "ldi r16, 0x14 \n" // 1 ciclo de reloj. 26 vueltas cargadas.
//      "1: dec r16 \n"    // 1 ciclo de reloj
//      "brne 1b  \n"      // 2 ciclos de reloj para retornar y 1 para salir
//    );
    
   OCR1B = i3;  // Pin B del timer 1. Genera la PWM para Q4
   
   OCR2A = i1;//puntero_sin240;  // Pin A del timer 2. Genera la PWM para Q5
   
   
   // Retardo 4.875 us.
    
//   asm volatile (
//      "clr r16 \n"       // 1 ciclo de reloj
//      "ldi r16, 0x14 \n" // 1 ciclo de reloj. 26 vueltas cargadas.
//      "1: dec r16 \n"    // 1 ciclo de reloj
//      "brne 1b  \n"      // 2 ciclos de reloj para retornar y 1 para salir
//    );
    
    OCR2B = i2;  // Registro de comparación para pin B del timer 2. Genera la PWM para Q6
  
  /////-------------------Falta el retardo de 32 us---------------------------------------/////

  // Ahora establecemos las condiciones para moverse por la tabla de valores y volver al comienzo una vez completado un período de señal.
  
//    if ( puntero_sin0++ < 625 )   { puntero_sin0++; }   else { puntero_sin0 = 0; }   // Si no se ha llegado al final de la tabla, avanza. Si se ha llegado, vuelve a 0.
//    if ( puntero_sin120++ < 625 ) { puntero_sin120++; } else { puntero_sin120 = 0; } // Si no se ha llegado al final de la tabla, avanza. Si se ha llegado, vuelve a 0.
//    if ( puntero_sin240++ < 625 ) { puntero_sin240++; } else { puntero_sin240 = 0; } // Si no se ha llegado al final de la tabla, avanza. Si se ha llegado, vuelve a 0.
    
}
