//#include <SPI.h>
#define __SAM3X8E__

//volatile uint8_t data;

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB
//
#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8

double i1 = 0, i2 = 0, i3 = 0;
char aa, tramRasp[9];
String a,svl, sv2, sv3;
int vl, v2, v3;
int flag=0;

char frst, scnd, thrd, frth;
int sensor0;
int sensor1;
int sensor2;
int sensor3;

double cont=0;
double ma, mb, mc;

// Stores data value read from ADC
int analog_data;
int wave = 1;
int i = 0;
char data;

void serialEvent(){//v0001500w0001200z0001000
  memset(&aa,0,sizeof(aa));
  while (Serial.available()>0) {
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
  Serial.println(ma);
  Serial.println(mb);
  Serial.println(mc);
  sendToRasp(); 
}


void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
}

//void UART_Handler (void) __attribute__ ((weak));
//void UART_Handler(void) {
// 
//  if (UART->UART_SR & UART_SR_RXRDY) {
//
//    data = UART->UART_RHR;
//    Serial.println(data);
//  }
//}

void setup()
{
  //Counicación serial
  Serial.begin(115200);

  //Salidas
  pinMode(13,OUTPUT);
  pinMode(7,OUTPUT); //Verificar tiempo interrupcion
  
  //Configuracion interrupción
  startTimer(TC2, 2, TC8_IRQn, 1666);               //TC2 channel 2, the IRQ for TC8

  //Triangular a 10KHz -- TC6 (TC2 Channel 0) -- Salidas (4,5)
  REG_PMC_PCER1 |= PMC_PCER1_PID33;                 // Enable peripheral TC6 (TC2 Channel 0)
  REG_PIOC_ABSR |= PIO_ABSR_P26 | PIO_ABSR_P25;     // Switch the multiplexer to peripheral B for TIOA6 and TIOB6
  REG_PIOC_PDR |= PIO_PDR_P26 | PIO_PDR_P25;        // Disable the GPIO on the corresponding pins

  REG_TC2_CMR0 = TC_CMR_BCPC_SET |                  // Set TIOB on counter match with RC0
                 TC_CMR_ACPC_SET |                  // Set TIOA on counter match with RC0
                 TC_CMR_BCPB_CLEAR |                // Clear TIOB on counter match with RB0
                 TC_CMR_ACPA_CLEAR |                // Clear TIOA on counter match with RA0
                 TC_CMR_WAVE |                      // Enable wave mode
                 TC_CMR_WAVSEL_UP_RC |              // Count up with automatic trigger on RC compare
                 TC_CMR_EEVT_XC0 |                  // Set event selection to XC0 to make TIOB an output
                 TC_CMR_TCCLKS_TIMER_CLOCK1;        // Set the timer clock to TCLK1 (MCK/2 = 84MHz/2 = 48MHz)
  REG_TC2_RC0 = 4200;                               // Load the RC0 register  
  REG_TC2_CCR0 = TC_CCR_SWTRG | TC_CCR_CLKEN;       // Enable the timer TC6

//Triangular a 10KHz -- TC7 (TC2 Channel 1) -- Salidas (3,10)
  REG_PMC_PCER1 |= PMC_PCER1_PID34;                 // Enable peripheral TC7 (TC2 Channel 1)
  REG_PIOC_ABSR |= PIO_ABSR_P29 | PIO_ABSR_P28;     // Switch the multiplexer to peripheral B for TIOA6 and TIOB6
  REG_PIOC_PDR |= PIO_PDR_P29 | PIO_PDR_P28;        // Disable the GPIO on the corresponding pins
  
  REG_TC2_CMR1 = TC_CMR_BCPC_SET |                  // Set TIOB on counter match with RC1
                 TC_CMR_ACPC_SET |                  // Set TIOA on counter match with RC1
                 TC_CMR_BCPB_CLEAR |                // Clear TIOB on counter match with RB1
                 TC_CMR_ACPA_CLEAR |                // Clear TIOA on counter match with RA1
                 TC_CMR_WAVE |                      // Enable wave mode
                 TC_CMR_WAVSEL_UP_RC |              // Count up with automatic trigger on RC compare
                 TC_CMR_EEVT_XC0 |                  // Set event selection to XC0 to make TIOB an output
                 TC_CMR_TCCLKS_TIMER_CLOCK1;        // Set the timer clock to TCLK1 (MCK/2 = 84MHz/2 = 48MHz)
  REG_TC2_RC1 = 4200;                               // Load the RC1 register  
  REG_TC2_CCR1 = TC_CCR_SWTRG | TC_CCR_CLKEN;       // Enable the timer TC7
  
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
  digitalWrite(7, HIGH);
  sensor0 = analogRead(A0);
  sensor1 = analogRead(A1);
  sensor2 = analogRead(A2);
  sensor3 = analogRead(A3);
//  receiveRaspData();
  
  //Acondicionamiento señal recibida
  i1 = ((((int(vl)/1000.0)-1)));//v0001500w0001200z0001000 -- v000000w0000000z0001000 -- v0002000w0002000z0001000
  i2 = ((((int(v2)/1000.0)-1)));
  i3 = ((((int(v3)/1000.0)-1)));

  if (i1 > 1){i1 = 1;}
  else if (i1 < -1){i1 = -1;}
  else {i1 = i1;}
  
  if (i2 > 1){i2 = 1;}
  else if (i2 < -1){i2= -1;}
  else {i2 = i2;}

  //Generar indice de modulación para señales SPWM
  ma=(1400*(PrimeraABC(i1,i2,cont)+1.5))+10;
  mb=(1400*(SegundaABC(i1,i2,cont)+1.5))+10;
  mc=(1400*(TerceraABC(i1,i2,cont)+1.5))+10; 
//  ma=(2000*(PrimeraABC(-1,-1,cont)+1))+50;
//  mb=(2000*(SegundaABC(-1,-1,cont)+1))+50;
//  mc=(2000*(TerceraABC(-1,-1,cont)+1))+50; 
//  Serial.println(mb,4);
  digitalWrite(7, LOW);

}

//Interrupción TC8
void TC8_Handler()
{
  TC_GetStatus(TC2, 2);

  //Registros de comparación
  REG_TC2_RA0 = ma;                              // Load the RA0 register
  REG_TC2_RB0 = mb;                              // Load the RB0 register
  REG_TC2_RA1 = mc;                              // Load the RA0 
  if ( cont < 0.016 )   { cont+=0.0006; }   else { cont = 0.0; }  
}
