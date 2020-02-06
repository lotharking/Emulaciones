
int incomingByte = 0;
double i1 = 0, i2 = 0, i3 = 0;
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
char frst, scnd, thrd, frth;
int sensor0;
int sensor1;
int sensor2;
int sensor3;

double cont=0;
double ma, mb, mc;

void setup() {

  // Configuramos los pines de PWM como salidas

  Serial.begin(115200);
 Serial.println("comunicacion"); 

  //Salidas
  pinMode (4, OUTPUT);  //Timer0 
  pinMode (5, OUTPUT);  // Interrupcion Timer3
  pinMode (9, OUTPUT);  // Timer2
  pinMode (10, OUTPUT); // Timer2
  pinMode (11, OUTPUT); // Timer1
  pinMode (12, OUTPUT); // Timer1
  pinMode (13, OUTPUT); //Timer0 

  //Cambio freq
//   noInterrupts();
//  CLKPR = _BV(CLKPCE);
//  CLKPR = _BV(CLKPS1);
//   interrupts();
  
  // Configuramos los timers mediante los registros de control
  TCCR0A = 0xB1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 8. Con salidas invertidas.
  TCCR0B = 0xC1; //C1-7KHz
  TCCR1A = 0xB1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 8. Con salidas invertidas.
  TCCR1B = 0xC1;
  TCCR2A = 0xB1; // Habilitamos las dos salidas en modo Phase-correct - 8 bits. Pre-escaler = 8. Con salidas invertidas.
  TCCR2B = 0xC1;


  TCCR3A = 0;                        // El registro de control A queda todo en 0, pines OC1A y OC1B deshabilitados
  TCCR3B = 0;                        //limpia el registrador
  TCCR3B |= (1<<CS30);   // configura sin prescaler
 
   TCNT3 = 0xFF38;//0xF830;
   TIMSK3 |= (1 << TOIE3);
}

void sendToRasp() {
  memset(tramRasp,0,sizeof(tramRasp));
      //sprintf(tramRasp, "f%07de",i1);
      Serial.print('f');
      Serial.print(sensor0);
      Serial.print(',');
      Serial.print(sensor1);
      Serial.print(',');
      Serial.print(sensor2);
      Serial.print(',');
      Serial.print(sensor3);
      Serial.println('e');
      Serial.flush();  
}

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

double PrimeraABC(double Vd, double Vq, double ConT)
{
  double V=1;
  double w = 2*PI*60;
  double V1 = sin(w*ConT);
  double V2 = cos(w*ConT);
  V = (Vd * V1) + (Vq * V2);
  return V;
}

double SegundaABC(double Vd, double Vq, double ConT)
{
  double V=1;
  double w = 2*PI*60;
  double V1 = -(sin(w*ConT)* 0.5)-(cos(w*ConT) * (sqrt(3)/2));
  double V2 = -(cos(w*ConT) * 0.5)+(sin(w*ConT) * (sqrt(3)/2));
  V = (Vd * V1)+ (Vq * V2);
  return V;
}

double TerceraABC(double Vd, double Vq, double ConT)
{
  double V=1;
  double w = 2*PI*60;
  double V1 = -(-(sin(w*ConT)* 0.5)-(cos(w*ConT) * (sqrt(3)/2)))-(sin(w*ConT));
  double V2 = -(-(cos(w*ConT) * 0.5)+(sin(w*ConT) * (sqrt(3)/2)))-(cos(w*ConT));
  V = (Vd * V1) + (Vq * V2);
  return V;
}


void loop() {
//  sensor0 = analogRead(A0);
//  sensor1 = analogRead(A1);
//  sensor2 = analogRead(A2);
//  sensor3 = analogRead(A3);
//
//  sendToRasp();
  receiveRaspData();
//  //Serial.println(vl);
  i1 = ((((int(vl)/1000.0)-1)));//v0001500w0001200z0001000
  i2 = ((((int(v2)/1000.0)-1)));
  i3 = ((((int(v3)/1000.0)-1)));
  ma=125*(PrimeraABC(i1,i2,cont)+1);
  mb=125*(SegundaABC(i1,i2,cont)+1);
  mc=125*(TerceraABC(i1,i2,cont)+1);
  Serial.println(ma,4);

}

ISR(TIMER3_OVF_vect)                              //interrupcion del TIMER3 
{
  TCNT3 = 0xFF38;  
  digitalWrite(5,HIGH);
  OCR0A = ma;
  
//  delayMicroseconds(0.4);
  
  OCR0B = ma;
  OCR1A = mb;

//  delayMicroseconds(0.4);
  
  OCR1B = mb;
  OCR2A = mc;

//  delayMicroseconds(0.4);
  
  OCR2B = mc;
  if ( cont < 0.017 )   { cont+=0.0001; }   else { cont = 0.0; }  
  
  digitalWrite(5,LOW);
  
}
