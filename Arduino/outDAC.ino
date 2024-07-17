#include <DueTimer.h>


//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB
//
#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8

int incomingByte = 0;
int i1 = 0, i2 = 0, i3 = 0;
int pb=0, qb=0, pd=0, qd=0;
int flag=0;
//int i3 = 0, vl = 0;
const int ledPin =  LED_BUILTIN;
char aa, tramRasp[9];
String a, spm, sqm, svl, ssoc;
//String si3;
int pm, qm, vl,vl2, soc;

//volatile boolean pinra = false;
bool pinra=false;
bool bi1 = false, bi2 = false,bi3 = false, bi4 = false;
int cont=0;
char frst, scnd, thrd, frth;

void setup()
{
 analogReadResolution(12);
 analogWriteResolution(12);
  Serial.begin(115200);
}

void lectura(){
  pinra = true;
 } 

void sendToRasp() {
  memset(tramRasp,0,sizeof(tramRasp));
      sprintf(tramRasp, "f%07de",i1);
      Serial.print(tramRasp);
      Serial.flush();  
}

void receiveRaspData() {
  memset(&aa,0,sizeof(aa));
  while (Serial.available() > 0) {
    aa = Serial.read();  
    a += aa;
    if (aa == '\n') {
      frst = a.charAt(0);
      if (frst == 'v') {
        flag=1;
        svl= a.substring(1, 8);        
        vl = svl.toInt();
        a = "";
      }
      Serial.flush();
      a = "";
    }    
  }
}

void loop() {
  i1=analogRead(A0);
  sendToRasp(); 
  receiveRaspData();
  //vl2=map(vl, -500, 500, 0, 4095);
  //vl2=map(vl, -22000, 22000, 0, 4095);
  analogWrite(DAC1,vl);
  //Serial.println(vl);
  delay(13);
}
