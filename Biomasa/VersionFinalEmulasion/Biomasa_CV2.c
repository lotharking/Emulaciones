//gcc -o Prueba Biomasa_CV2.c -lrt -Wall -lm -lwiringPi


#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>                                      
#include <wiringSerial.h>
#include <errno.h>
#include <string.h> 
#include <unistd.h> 
#include <termios.h>                                    
#include <wiringPi.h>                                     
#include <stdint.h>
#include <sched.h>
#include <math.h> 
#include <stdbool.h> 
//#include <pthread.h>
#include <semaphore.h>


const int minutes = 60;
const int seconds = 1;

//Constantes
#define PI 3.14159
#define LOW_SPIN 2
#define MID_SPIN 5
#define NSEC_PER_SEC    1000000000//1000000000
int estado=0;

//Contadores
double cont = 0;
int contVect = 0;
double contP=0;
//entrada
double Mg = 0.0002*15;//50-15

//arreglos
double Promedio[200];
double PteR=0;
double PtcR=0;
double u[45][185];
//Acondicionaamiento Torque
double teR = 0;
double tcR = 0;

//Temperatura
double N = 45;//45
double ku = 15;
double rho = 7830;
double cp = 500;
double L = 0.04;
double alf= 0;
double dt = 0;
double hi = 0;
double ttt = 20;
double jmax = 0;
double TW1,TW2,TW3,TW4,TW5,TW6,Tg1,Tg2,Tg3,Tg4,Tg5,Tg6;
double retro1=0,retro2=0,retro3=0,retro4=0,retro5=0,retro6=0;

//Variables globales
double h =1000; //coeficiente global de transferencia de calor (W/m^2*K)
double Awr = 0.0058; //area de transferencia de calor de la rejilla
double dg = 1.23; //densidad del gas de trabajo(kg/m^3)
double cpg = 1006; //calor especifico del gas de trabajo (J/Kg*k)
double Rg = 0.287; //constante de gas ideal(K/Kg)
double Xwr = 0.04; //Longitud del Regenerador
double lwr = 0.06; //Longitud del regenerador en metros.
double b = 0.5; //Coeficiente de fricción
double c = 0.012; //coeficiente de carga !!OJO CAMBIO!!
double Patm = 101.325; //presion atmosferica (kpa)//HACER PRUEBA CON 101325--->PREGUNTAR A KEVIN
double Ap = 0.00614; //area del piston (m^2)
double Fg = 0,Fg1=0,Fg2=0;
double r = 0.04; //radio del cigueñal (m)
double l = 0.12; //longitud de la biela (m)
double PHI = PI/2; //angulo de desfasamiento (rad)
double rp = 0.04421;//0.015; //radio del piston (m) OJO LO MULTIPLIQUE X 2 PARA VER QUE PASA CON LA POTENCIA
double J = 0.04; //momento de inercia (Kg*m^2)
double Tgn = 0;
double Xg = 0;
double T11 = 0;
double T22 = 0;
double T111 = 300;
double T222 = 300;
double Vg = 1,vol=0,ant,ant2;
double teta = 0;
double Pg = 0;
double Xe = 0;
double Xc = 0;
double W =0;
//Torques
double tf = 0;
double te = 0; //Torque de expansión
double tc = 0; //Torque de compresión
double tq = 0; //Carga
double tmec = 0;
double Pmec = 0;
//
double fi = 0;
double Fi_e = 0;
double Ce = 0;
double Cc = 0;
double Fi_c = 0;
double Alfa = 0;
double Vel = 0;
double ini = 10; //Valor inicial
double VelMax=0;
double VelMax2=0;
double VelMax3=0;
double VelMin=0;
double VelTot=0;

//Variables termicas y termodinamicos
double Acilc = 0;
double Acile = 0;
double T1 = 0;
double Tg1ant = 0;
double T2 = 0;
double Tg2ant = 0;
double Tcld = 0;
double Tht = 0;
//Parametros frio
double Tcold = 50; //temperatura de entrada (K)
double F1 = 0.001; //flujo de chaqueta (kg/s)
double V1 = 0.001; //volumen de chaqueta (m^3)
//Parametros caliente
double dv = 14.71; //densidad (kg/m^3)
double cpv = 1873.3; //calor especifico (Kg*K)
double Thot = 300; //temperatura de entrada(K)
double V2 = 0.001; //volumen de chaqueta (m^3)
double F2 = 0.001; //flujo de chaqueta (kg/s)

//Parametros termodinamicos
double pcs = 0,pcs2=0;
double TG = 1;
//Parametros integral
double IntIn1 = 1;
double IntOut1 = 1;
double Int2In1 = 1;
double Int2Out1 = 1;
double Int3In1 = 1,Int3In2 = 1,Int3In3 = 1,Int3In4 = 1,Int3In5 = 1,Int3In6 = 1;
double Int3Out1 = 1,Int3Out2 = 1,Int3Out3 = 1,Int3Out4 = 1,Int3Out5 = 1,Int3Out6 = 1;
double Int4In1 = 1;
double Int4Out1 = 1;
double Int5In1 = 1;
double Int5Out1 = 1;
double Int6In1 = 1;
double Int6Out1 = 1;
double Int7In1 = 1;
double Int7Out1 = 1;

//Parametros motor DC
double Ri=0.973;
double Ji=1.72;
double Kp=0.516;//5.6621
double Kb=1.25;//6.2
double Bi=0.1;
double Vout;
double Iout=0;
double Rout=25;
double PTelec=0;
double Pelec=0;

//Conversor
double DcVo=0, DcVoSup = 0, DcVoInf = 0, Duty = 0, Duty1 = 0, DCDiv = 0, DCDiv2 = 0;
double ContIIn = 0, ContIIn1 = 0, ContIIn2 = 0, ContIOut1 = 0, ContIOut2 = 0;
double VoContrEntrada = 0,ContrDcVo = 0;
double SalidaVDCDeseada = 300;

//Inversor
double Ws= 2* PI * 60;
double SumaVD = 0;
double InContrVD1 = 0;
double OutContrVD1 = 0;
double SumaVQ = 0;
double InContrVQ1 = 0;
double OutContrVQ1 = 0;
double VoRef = 0;
double Vod = 0;
double Voq = 0;
double Vo0 = 0;
double Iinvd = 0;
double Iinvq = 0;
double Iinv0 = 0;
double Cinv = 0.00001;
double Linv = 0.025;
double Vbus = 150;
double ContrVod = 0;
double ContrVoq = 0;
double DesDinVD = 0;
double DesDinVQ = 0;
double SumaID = 0;
double SumaIQ = 0;
double InContrID1 = 0;
double OutContrID1 = 0;
double InContrIQ1 = 0;
double OutContrIQ1 = 0;
double ContrIod = 0;
double ContrIoq = 0;
double DesDinID = 0;
double DesDinIQ = 0;
double Md = 0, Mq = 0;
double MA = 0, MB = 0, MC = 0;
double InvSupA = 0, InvInfA = 0, InvSupB = 0, InvInfB = 0, InvSupC = 0, InvInfC = 0;
double InvSupAIn1 = 0,InvSupAOut1 = 0,InvSupAIn2 = 0,InvSupAOut2 = 0;
double InvInfAIn1 = 0,InvInfAOut1 = 0,InvInfAIn2 = 0,InvInfAOut2 = 0;
double InvSupBIn1 = 0,InvSupBOut1 = 0,InvSupBIn2 = 0,InvSupBOut2 = 0;
double InvInfBIn1 = 0,InvInfBOut1 = 0,InvInfBIn2 = 0,InvInfBOut2 = 0;
double InvSupCIn1 = 0,InvSupCOut1 = 0,InvSupCIn2 = 0,InvSupCOut2 = 0;
double InvInfCIn1 = 0,InvInfCOut1 = 0,InvInfCIn2 = 0,InvInfCOut2 = 0;
double RefIn1 = 0,RefOut1 = 0,RefIn2 = 0,RefOut2 = 0;
double InversorA = 0, InversorB = 0, InversorC = 0;
double IoutInversorA = 0, IoutInversorB = 0, IoutInversorC = 0;
double CruceXCero = 0, InvAnterior = 0, PLL = 0;
double FilIn1D = 0, FilIn2D = 0, FilOut1D = 0, FilOut2D = 0, VodPrueba = 0;
double FilIn1Q = 0, FilIn2Q = 0, FilOut1Q = 0, FilOut2Q = 0, VoqPrueba = 0;
double VodAR[16], VoqAR[16];
double Pot3Phase = 0;

//Txt
FILE* fichero;
char text1[20],text2[20],text3[20],text4[20],text5[20],text6[20],text7[20],text8[20],text9[20],text10[20],text11[20],text12[20],text13[20];
char text14[20],text15[20],text16[20],text17[20],text18[20],text19[20],text20[20],text21[20];


static inline void tsnorm(struct timespec *ts)
{
   while (ts->tv_nsec >= NSEC_PER_SEC) {
      ts->tv_nsec -= NSEC_PER_SEC;
      ts->tv_sec++;
   }
}

extern int clock_nanosleep(clockid_t __clock_id, int __flags,
      __const struct timespec *__req,
      struct timespec *__rem);

double integral1(double IntIn1, double IntOut1){
	double intg=1;
	intg=(0.0001*IntIn1)+(IntOut1);
	return intg;
}

double tempGas(double Tgn, double Vg,double retro){
  double TgAnt = retro;//solo funciona si inicializo igualandola a la Temperatura de entrada (CORREGIR)
  double Sum = 0;
  double Div = 0;
  double Mult = 0;
  double Cte = (h*Awr)/(dg*cpg);
  Div = Cte/Vg;
  Sum = Tgn - TgAnt;
  Mult = Sum * Div;
  TgAnt = integral1(Int3In1,Int3Out1);
  Int3In1 = Mult;
  Int3Out1 = TgAnt;
  //printf("\n Vg: %f",Vg);
  //printf("\n Div: %f",Div);
  //printf("\n Cte: %f",Cte);
  //printf("\n Sum: %f",Sum);
  //printf("\n Mult: %f",Mult);
  //printf("\n Tgn: %f",Tgn);
  //printf("\n Tg1: %f",TgAnt);
  return TgAnt;
}

double Temperatura(double pcs2,double Temp1, double Temp2){
  //variables inicio
  double temp = 0;
  double Sum1 = 0;
  double Sum2 = 0;
  double Sum3 = 0;
  double Sum4 = 0;
  double Sum5 = 0;
  double Sum6 = 0;
  double Div = 0;
  double Mult1 = 0;
  double Mult2 = 0;
  double Mult3 = 0;
  double Mult4 = 0;
  double Mult5 = 0;
  double Mult6 = 0;
  double Cte = (h*Awr)/(dg*cpg);
  Div = Cte/Vg;
  //int Ni = (int) N;
  double q = 0;
  double Ti = 350;
  double T0 = Temp2;
  double TL = Temp1;
  double ra = (ku*dt)/(rho*cp*(pow(hi,2)));

  //Variables selección Regnerador
  //double a=0.0337;
  //double b=0.040;
  //double c=0.0482;
  //double d=0.0555;
  //double e=0.0639;

  if (ra>0.5){printf("disminuya el intervalo de tiempo dt");}
  
//hace el u(:,1)
for(int i=0;i<N;i++){
  for(int j=0;j<1;j++){
    u[i][j]=Ti;
  }
}
//hace el u(1,:)
for(int i=0;i<1;i++){
	for(int j=0;j<jmax;j++){
    u[i][j]=T0;
  }
}

////hace el u(N,:)
for(int j=0;j<jmax;j++){
	u[44][j]=TL;
	//printf("aqui: %f\n",u[44][j]);
}

for(int j=0;j<jmax-1;j++){
  for(int i=1;i<N-1;i++){
    u[i][j+1]= ((ra*(u[i-1][j]))+(1-(2*ra))*(u[i][j])+(ra*(u[i+1][j]))) + (q*dt);
  }
}
////Salidas del ciclo
////Salida1
TW1= u[44][(int) jmax-1];
////printf("\n %5.4f",TW1);
//Tg1 = tempGas(TW1, Vg,retro1);
Sum1 = TW1 - retro1;
Mult1 = Sum1 * Div;
Tg1 = integral1(Int3In1,Int3Out1);
Int3In1 = Mult1;
Int3Out1 = Tg1;
retro1=Tg1;

////Salida2
TW2= u[29][(int) jmax-1];
////printf("\n %5.4f",TW2);
//Tg2 = tempGas(TW2, Vg,retro2);
Sum2 = TW2 - retro2;
Mult2 = Sum2 * Div;
Tg2 = integral1(Int3In2,Int3Out2);
Int3In2 = Mult2;
Int3Out2 = Tg2;
retro2=Tg2;

////Salida3
TW3= u[20][(int) jmax-1];
////printf("\n %5.4f",TW3);
//Tg3 = tempGas(TW3, Vg,retro3);
Sum3 = TW3 - retro3;
Mult3 = Sum3 * Div;
Tg3 = integral1(Int3In3,Int3Out3);
Int3In3 = Mult3;
Int3Out3 = Tg3;
retro3=Tg3;

////Salida4
TW4= u[12][(int) jmax-1];
////printf("\n %5.4f",TW4);
//Tg4 = tempGas(TW4, Vg,retro4);
Sum4 = TW4 - retro4;
Mult4 = Sum4 * Div;
Tg4 = integral1(Int3In4,Int3Out4);
Int3In4 = Mult4;
Int3Out4 = Tg4;
retro4=Tg4;

////Salida5
TW5= u[6][(int) jmax-1];
////printf("\n %5.4f",TW5);
//Tg5 = tempGas(TW5, Vg,retro5);
Sum5 = TW5 - retro5;
Mult5 = Sum5 * Div;
Tg5 = integral1(Int3In5,Int3Out5);
Int3In5 = Mult5;
Int3Out5 = Tg5;
retro5=Tg5;

////Salida5
TW6= u[0][(int) jmax-1];
////printf("\n %5.4f",TW6);
//Tg6 = tempGas(TW6, Vg,retro6);
Sum6 = TW6 - retro6;
Mult6 = Sum6 * Div;
Tg6 = integral1(Int3In6,Int3Out6);
Int3In6 = Mult6;
Int3Out6 = Tg6;
retro6=Tg6;

if (pcs2==1){
  temp=Tg1;
}else if (pcs2==2){
  temp=Tg2;
}else if (pcs2==3){
  temp=Tg3;
}else if (pcs2==4){
  temp=Tg4;
}else if (pcs2==5){
  temp=Tg5;
}else if (pcs2==6){
  temp=Tg6;
}else if (pcs2==7){
  temp=Tg6;
}else if (pcs2==8){
  temp=Tg5;
}else if (pcs2==9){
  temp=Tg4;
}else if (pcs2==10){
  temp=Tg3;
}else if (pcs2==11){
  temp=Tg2;
}else if (pcs2==12){
  temp=Tg1;
}else{temp=0;}

//printf("\n Xg: %f",Xg);
//printf("\n Temp1: %f",Tg1);
//printf("\n Temp2: %f",Tg2);
//printf("\n Temp3: %f",Tg3);
//printf("\n Temp4: %f",Tg4);
//printf("\n Temp5: %f",Tg5);
//printf("\n Temp6: %f",Tg6);
//printf("\n out: %f",temp);
return temp;
}

double Telec(double kp, double iout){
	double x= kp*iout;
	return x;
}

//Generador
double tension(double Ri,double Kb,double Iout,double vel){
	double w=Kb*vel*3;
	double w1=Ri*Iout;
	double v=w+w1;
	return v;
}

double ControlDcVo(double ContIIn, double ContIIn1,double ContIOut1){
	double Vo=1;
	//Vo=(0.00125*ContIIn)+(0.00125*ContIIn1)+(ContIOut1);//Funciona con modelo pero es ideal
	Vo=(0.001002*ContIIn)-(0.0009984*ContIIn1)+(ContIOut1);
	return Vo;
}

//INVERSOR
//Control de tensión
double ControlVDQ(double SVDQ, double InCtrVDQ1, double OutCtrVDQ1)
{
	double Vo=1;
	//Vo=(SVDQ * 0.5) - (0.495 * InCtrVDQ1) + (OutCtrVDQ1);
	Vo=(SVDQ * 0.2001) - (0.2 * InCtrVDQ1) + (OutCtrVDQ1);
	return Vo;
} 

//Control de corriente
double ControlIDQ(double SIDQ, double InCtrIDQ1, double OutCtrIDQ1){
	double Vo=1;
	Vo=(SIDQ * 5) - (4.99 * InCtrIDQ1) + (OutCtrIDQ1);
	return Vo;
}

//DQ a ABC
//Primera linea
double PrimeraABC(double Vd, double Vq, double Vo){
	double V=1;
	double w = 2*PI*60;
	double V1 = sin(w*cont);
	double V2 = cos(w*cont);
	V = (Vd * V1) + (Vq * V2) + (Vo);
	return V;
}

//Segunda linea
double SegundaABC(double Vd, double Vq, double Vo){
	double V=1;
	double w = 2*PI*60;
	double V1 = -(sin(w*cont)* 0.5)-(cos(w*cont) * (sqrt(3)/2));
	double V2 = -(cos(w*cont) * 0.5)+(sin(w*cont) * (sqrt(3)/2));
	V = (Vd * V1)+ (Vq * V2) + (Vo);
	return V;
}

//Tercera linea
double TerceraABC(double Vd, double Vq, double Vo){
	double V=1;
	double w = 2*PI*60;
	double V1 = -(-(sin(w*cont)* 0.5)-(cos(w*cont) * (sqrt(3)/2)))-(sin(w*cont));
	double V2 = -(-(cos(w*cont) * 0.5)+(sin(w*cont) * (sqrt(3)/2)))-(cos(w*cont));
	V = (Vd * V1) + (Vq * V2) + (Vo);
	return V;
}

//ABC a DQ
//Primera linea
double PrimeraD(double VA, double VB, double VC)
{
	double V=1;
	double w = 2*PI*60;	
	double V1 = sin(w*cont);	
	double V2 = -0.5*sin(w*cont)-sqrt(3)/2.0*cos(w*cont);
	double V3 = -V2-sin(w*cont);
	V = (2/3.0)*(VA*V1+VB*V2+VC*V3);
	return V;
}

//Segunda linea
double SegundaQ(double VA, double VB, double VC)
{
	double V = 1;
	double w = 2*PI*60;
	double V1 = cos(w*cont);	
	double V2 = sqrt(3)/2.0*sin(w*cont)-0.5*cos(w*cont);
	double V3 = -V2-cos(w*cont);
	V = (2/3.0)*(VA*V1+VB*V2+VC*V3);
	return V;
}

//Tercera linea
double Tercera0(double VA, double VB, double VC)
{
	double V=1;
	V = (1/3.0)*(VA + VB + VC);
	return V;
}

double InversorSup(double in, double InvInS1,double InvInS2,double InvOutS1,double InvOutS2){
	double Vo=1;
	//Vo=(0.01993*InvInS1)+(0.01993*InvInS2)+(1.96*InvOutS1)-(InvOutS2);
	Vo=(0.009901*in)+(0.0198*InvInS1)+(0.009901*InvInS2)+(1.96*InvOutS1)-(InvOutS2);
	//Vo=(0.006653*in)+(0.02656*InvInS1)+(0.006653*InvInS2)+(1.96*InvOutS1)-(InvOutS2);
	//Vo=(0.03973*InvInS1)+(1.96*InvOutS1)-(InvOutS2);
	//Vo=(0.01993*InvInS1)+(0.01993*InvInS2)+(1.96*InvOutS1)-(InvOutS2);
	return Vo;
}   

double InversorInf(double inI, double InvInI1,double InvInI2,double InvOutI1,double InvOutI2){
	double Vo=1;
	//Vo=(-9.933*InvInI1)+(9.933*InvInI2)+(1.96*InvOutI1)-(InvOutI2);//ZOH
	Vo=(-4.95*inI)+(4.95*InvInI2)+(1.96*InvOutI1)-(InvOutI2);//TUSTIN--Funcionamiento bueno
	//Vo=(-4.983*inI)+(4.983*InvInI2)+(1.96*InvOutI1)-(InvOutI2);//FOH
	//Vo=(-10*inI)+(9.801*InvInI1)+(1.96*InvOutI1)-(InvOutI2);//IMPULSE--Mejor funciona pero sigue teniendo error
	//Vo=(-9.972*InvInI1)+(9.972*InvInI2)+(1.96*InvOutI1)-(InvOutI2);//Matched--Peor funcionamiento
	return Vo;
}

double Referencia(double Retin, double RetIn1,double RetIn2,double RetOut1,double RetOut2){
	double Vo=1;
	Vo=(0.0198*RetIn1)+(0.9802*RetOut1);
	//Vo=(0.000999*Retin)+(0.000999*RetIn1)+(0.998*RetOut1);
	return Vo;
}

int out(){

	//Lectura Vrio serial-- aqui lee la velocidad del rio

	//while(read_buffer[0]!='f' && read_buffer[8]!='e'){
		//bytes_read=read(fd,&read_buffer,9);
		//serialFlush(fd);
		//tcflush(fd,TCIOFLUSH);
		//Vrio_char=(read_buffer[4]-48)*1000;
		//Vrio_char+=(read_buffer[5]-48)*100;
		//Vrio_char+=(read_buffer[6]-48)*10;
		//Vrio_char+=(read_buffer[7]-48);
		//vrio=((Vrio_char)*3.3)/4095;
	//}
		//read_buffer[0]='0';
		//read_buffer[1]='0';
		//read_buffer[2]='0';
		//read_buffer[3]='0';
		//read_buffer[4]='0';
		//read_buffer[5]='0';
		//read_buffer[6]='0';
		//read_buffer[7]='0';
		//read_buffer[8]='0';
		
		
		
  //for (cont=0;cont<500;cont++){
    Tgn = Temperatura(pcs2,T111,T222); //Temperatura actual del gas
    //Parte mecanica
    Fg =((Pg*1000)-(Patm*1000))*Ap;
    if(Fg<0){Fg=0;}
    fi = 1.57079;
    Fi_e = asin((r*sin(teta))/l);
    Ce = (r*cos(teta))+(l*cos(Fi_e));
    Fi_c = asin((r*sin(teta-fi))/l);
    Cc = (r*cos(teta-fi))+(l*cos(Fi_c));
    Xe = r+l-Ce; //Carrera de expansión
    Xc = r+l-Cc; //Carrera de compresión
    //te = Fg*r*sin(teta)*(1+((r/l)*cos(teta)));
    //tc = Fg*r*sin(teta-fi)*(1+((r/l)*cos(teta-fi)));
    //te = Fg*tan(Fi_e)*(l-((pow(r,2))/(4*l))+(r*(cos(teta))+(pow(r,2))/(4*l)*cos(2*teta)));
    //tc = Fg*tan(Fi_c)*(l-((pow(r,2))/(4*l))+r*(cos(teta-fi)+(pow(r,2))/(4*l)*cos(2*teta-2*fi)));
    if (pcs==1){Fg1=Fg;}
    else{Fg1=0;}
    te = Fg1*tan(Fi_e)*(l-((pow(r,2))/(4*l))+r*(cos(teta)+(r/(4*l))*cos(2*teta)));
    if (pcs==3){Fg2=Fg;}
    else{Fg2=0;}
    tc = Fg2*tan(Fi_e)*(l-((pow(r,2))/(4*l))+r*(cos(teta-fi)+(r/(4*l))*cos(2*teta-2*fi)));
    tf = Vel*b;
    Xg = ((r*4)+lwr-Xe-Xc)*0.5;

    //acondicionamiento par
    if (pcs==1){
	if (te>PteR){PteR=te;}else{PteR=PteR;}
    PtcR=PtcR;	
	} else if (pcs==2){
    PteR=PteR;
    PtcR=PtcR;
	}	else if (pcs==3){
    PteR=PteR;    
	if (tc<PtcR){PtcR=tc;}else{PtcR=PtcR;}	
	} else if (pcs==4){
    PteR=PteR;
    PtcR=PtcR;
	}
	if (PteR==0 && PtcR==0){tmec=tmec;}
	else{tmec=(PteR+PtcR);}
    
    if (cont>10){ini=0;} //Recordar añadirle una condición inicial
    //
    if (cont>150){tq=10;}
    Alfa =  (te + tc+ini-tf-tq)/(J*15.0); //Sumarle ini
    //Alfa =  (te + tc+ini-tf)/(J*15.0); //Sumarle ini
    //Vel += Alfa;
    //teta += Vel;
    Vel = integral1(IntIn1,IntOut1);
    IntIn1 = Alfa;
    IntOut1 = Vel;
    teta = integral1(Int2In1, Int2Out1);
    Int2In1 = Vel;
    Int2Out1 = teta;
    
    //Caja multiplicadora
    W=Vel*3*9.549;
    //tmec=(te+tc-tf)/3.0;
    Pmec=Vel*tmec;
    //if (pcs2==12 && ant2 ==11) {contP++;}
    if (contP==150) {VelMax=Vel;contP=0;}
    if (contP==100) {VelMax2=Vel;}
    if (contP==50) {VelMax3=Vel;}
    //if (Vel>VelMax){VelMax=Vel;} else{VelMax=VelMax;}
    VelTot = (VelMax+VelMax2+VelMax3)/3.0;
    //printf("\n contP: %f",contP);
    printf("\n VelTot: %f",VelTot);
    
    
    //Modelo termico
    Acilc = (rp*2*PI)*Xc;
    Tg1ant = ((Acilc*(h/(dg*cpg)))/Vg) * (T11-T1);//el signo en h
    T1 = integral1(Int4In1,Int4Out1);
    Int4In1 = Tg1ant;
    Int4Out1 = T1;
    Tcld = ((Tcold-T11)* F1/(V1*dv)-h*Acilc/(V1*dv*cpv)*(T11-T1));
    T11 += Tcld;
    T111 = T11 + 300;
    Acile = (rp*2*PI)*Xe;
    Tg2ant = ((Acile*(h/(dg*cpg)))/Vg) * (T22-T2);
    T2 = integral1(Int6In1,Int6Out1);
    Int6In1 = Tg2ant;
    Int6Out1 = T2;
    Tht = (Thot-T22)* F2/(V2*dv)-h*Acile/(V2*dv*cpv)*(T22-T2);
    T22 += Tht;
    T222 = T22 + 300;

    //Modelo termodinamico
    //pcs = floor(4*((teta/(2*3.1426))-floor(teta/(2*3.1426))))+1;
    //if (pcs == 1){TG=Tgn;}
    //else if (pcs == 2){TG=T2;}
    //else if (pcs == 3){TG=Tgn;}
    //else if (pcs == 4){TG=T1;} 
    if (pcs == 1){TG=Tgn;}
    else if (pcs == 2){TG=Tgn;}
    else if (pcs == 3){TG=Tgn;}
    else if (pcs == 4){TG=Tgn;} 
    Pg = (Mg * Rg * TG) / Vg;
    Vg = (Xc + Xe + Xwr) * Ap;
    
    //Proceso
    vol=Vg*100000;
    if (vol<=55.24)
    {pcs=1;} 
    else if (vol>55.24 && vol<=104.10 && ant ==1)
    {pcs=2;}
    else if (vol>55.24 && vol<=104.10 && ant ==2)
    {pcs=2;}    
    else if (vol>104.10)
    {pcs=3;}
	else if (vol>55.24 && vol<=104.10 && ant ==3)
     {pcs=4;}
	else if (vol>55.24 && vol<=104.10 && ant ==4)
     {pcs=4;}
     
    if (pcs==1 && ant==4){PteR=0;PtcR=0;}
     ant=pcs;
     
    if (vol<104.10 && vol>=51.2 && ant2 ==12)
    {pcs2=1;}
	else if (vol<104.10 && vol>=51.2 && ant2 ==1)
    {pcs2=1;}
	else if (vol<51.2 && vol>=47.15 && ant2 ==1)
    {pcs2=2;}
	else if (vol<51.2 && vol>=47.15 && ant2 ==2)
    {pcs2=2;}     
	else if (vol<47.15 && vol>=43.2 && ant2 ==2)
    {pcs2=3;}
	else if (vol<47.15 && vol>=44 && ant2 ==3)
    {pcs2=3;}     
	else if (vol<44 && vol>=40 && ant2 ==3)
    {pcs2=4;}    
	else if (vol>40 && vol<=47.15 && ant2 ==4)
    {pcs2=4;}      
	else if (vol>47.15 && vol<=51.2 && ant2 ==4)
    {pcs2=5;}  
	else if (vol>47.15 && vol<=51.2 && ant2 ==5)
    {pcs2=5;}     
	else if (vol>51.2 && vol<=104.10 && ant2 ==5)
    {pcs2=6;}   
	else if (vol>51.2 && vol<=104.10 && ant2 ==6)
    {pcs2=6;}    
	else if (vol>104.10 && vol<=106.9 && ant2 ==6)
    {pcs2=7;}  
	else if (vol>104.10 && vol<=106.9 && ant2 ==7)
    {pcs2=7;}		
	else if (vol>106.9 && vol<=109.7 && ant2 ==7)
    {pcs2=8;}  
	else if (vol>106.9 && vol<=109.7 && ant2 ==8)
    {pcs2=8;}      
	else if (vol>109.7 && vol<=112 && ant2 ==8)
    {pcs2=9;} 
	else if (vol>109.7 && vol<=112 && ant2 ==9)
    {pcs2=9;}
	else if (vol<120 && vol>=109.7 && ant2 ==9)
    {pcs2=10;}
	else if (vol<120 && vol>=109.7 && ant2 ==10)
    {pcs2=10;}     
	else if (vol<109.7 && vol>=106.9 && ant2 ==10)
    {pcs2=11;} 
	else if (vol<109.7 && vol>=106.9 && ant2 ==11)
    {pcs2=11;}     
	else if (vol<106.9 && vol>=104.10 && ant2 ==11)
    {pcs2=12;}
	else if (vol<106.9 && vol>=104.10 && ant2 ==12)
    {pcs2=12;contP++;}
    else {pcs2=1;}
	ant2=pcs2;
    
        
    //Generador Dc
    Vout= tension(Ri,Kb, Iout,VelTot); //se esta usando la velocidad maxima
    
    //Par electrico
	//if(cont>30){
	tq= Telec(Kp,Iout);//No olvidar el signo
	//} else {tq= 0;}//Solo funciona para las pruebas
	
	if (cont>5){
    
    //CONVERSOR DC-DC
	DcVo = DcVoSup + 0.001;
	Duty=(Vout/SalidaVDCDeseada);
	if (Duty>0.9){Duty=0.9;}
	DCDiv = Vout/(Duty);
	
	
	//Control
	VoContrEntrada = ControlDcVo(SalidaVDCDeseada - DcVo,ContIIn1,ContIOut1);
	ContIOut1 = VoContrEntrada;
	ContIIn1 = SalidaVDCDeseada - DcVo;
	
	//Conversor
	DcVoSup = Vout/(1 - VoContrEntrada);
    
    //Corriente
    Iout=Pot3Phase/(DcVo);//Pot3Phase/DcVo;
	if (Iout>15){Iout=15;}
	if (Iout<0){Iout=0;}
	if (Vel<0){Iout=0;}
	
}
	if(cont>5){
	////INVERSOR
	//Referencia
	VoRef = Referencia(180,RefIn1,RefIn2,RefOut1,RefOut2);
	RefIn1 = 180;
	RefOut1 = VoRef;
	
	//Control de tensión
	SumaVD = VoRef - Vod;
	SumaVQ = 0 - Voq;
	ContrVod = ControlVDQ(SumaVD, InContrVD1, OutContrVD1);
	InContrVD1 = SumaVD;
	OutContrVD1 = ContrVod;
	ContrVoq = ControlVDQ(SumaVQ, InContrVQ1, OutContrVQ1);
	InContrVQ1 = SumaVQ;
	OutContrVQ1 = ContrVoq;
	
	//Desacople de tensión
	DesDinVD = ContrVod - (Cinv * Voq * Ws);
	DesDinVQ = ContrVoq + (Cinv * Vod * Ws);
	
	//Control de corriente
	SumaID = DesDinVD - Iinvd;
	SumaIQ = DesDinVQ - Iinvq;
	ContrIod = ControlIDQ(SumaID, InContrID1, OutContrID1);
	InContrID1 = SumaID;
	OutContrID1 = ContrIod;
	ContrIoq = ControlIDQ(SumaIQ, InContrIQ1, OutContrIQ1);
	InContrIQ1 = SumaIQ;
	OutContrIQ1 = ContrIoq;
	
	//Desacople de Corriente
	DesDinID = ContrIod - (Linv * Iinvq * Ws);
	DesDinIQ = ContrIoq + (Linv * Iinvd * Ws);
	Md = (DesDinID + Vod) * (2/Vbus);
	Mq = (DesDinIQ + Voq) * (2/Vbus);
	
	if (Md > 1){Md = 1;}
	else if (Md < -1){Md = -1;}
	else {Md = Md;}
	
	if (Mq > 1){Mq = 1;}
	else if (Mq < -1){Mq = -1;}
	else {Mq = Mq;}
	
	//Generación MABC
	MA = PrimeraABC(Md, Mq, 0);
	MB = SegundaABC(Md, Mq, 0);
	MC = TerceraABC(Md, Mq, 0);
	
	//Inversor
	InvSupA = InversorSup(DcVo * MA,InvSupAIn1,InvSupAIn2,InvSupAOut1,InvSupAOut2);
	InvSupAOut2 = InvSupAOut1;
	InvSupAIn2=InvSupAIn1;
	InvSupAOut1=InvSupA;
	InvSupAIn1=DcVo * MA;
	InvInfA = InversorInf(IoutInversorA,InvInfAIn1,InvInfAIn2,InvInfAOut1,InvInfAOut2);
	InvInfAOut2 = InvInfAOut1;
	InvInfAIn2=InvInfAIn1;
	InvInfAOut1=InvInfA;
	InvInfAIn1=IoutInversorA;
	InvSupB = InversorSup(DcVo * MB,InvSupBIn1,InvSupBIn2,InvSupBOut1,InvSupBOut2);
	InvSupBOut2 = InvSupBOut1;
	InvSupBIn2=InvSupBIn1;
	InvSupBOut1=InvSupB;
	InvSupBIn1=DcVo * MB;
	InvInfB = InversorInf(IoutInversorB,InvInfBIn1,InvInfBIn2,InvInfBOut1,InvInfBOut2);
	InvInfBOut2 = InvInfBOut1;
	InvInfBIn2=InvInfBIn1;
	InvInfBOut1=InvInfB;
	InvInfBIn1=IoutInversorB;
	InvSupC = InversorSup(DcVo * MC,InvSupCIn1,InvSupCIn2,InvSupCOut1,InvSupCOut2);
	InvSupCOut2 = InvSupCOut1;
	InvSupCIn2=InvSupCIn1;
	InvSupCOut1=InvSupC;
	InvSupCIn1=DcVo * MC;
	InvInfC = InversorInf(IoutInversorC,InvInfCIn1,InvInfCIn2,InvInfCOut1,InvInfCOut2);
	InvInfCOut2 = InvInfCOut1;
	InvInfCIn2=InvInfCIn1;
	InvInfCOut1=InvInfC;
	InvInfCIn1=IoutInversorC;
	
	//Tensión y corriente Inversor trifasica
	InversorA = InvSupA + InvInfA;
	InversorB = InvSupB + InvInfB;
	InversorC = InvSupC + InvInfC;
	IoutInversorA = InversorA/50.0;//Valor leido del archivo
	IoutInversorB = InversorB/50.0;
	IoutInversorC = InversorC/50.0;
	
	
	//Tensión y corriente Inversor DQ
	Vod = PrimeraD(InversorA, InversorB, InversorC);
	Vo0 = Tercera0(InversorA, InversorB, InversorC);
	Voq = SegundaQ(InversorA, InversorB, InversorC);
	
	Iinvd = PrimeraD(IoutInversorA, IoutInversorB, IoutInversorC);
	Iinvq = SegundaQ(IoutInversorA, IoutInversorB, IoutInversorC);
	Iinv0 = Tercera0(IoutInversorA, IoutInversorB, IoutInversorC);
	
	//Potencia trifasica
	Pot3Phase = (InversorA*IoutInversorA+InversorB*IoutInversorB+InversorC*IoutInversorC);
	if (Pot3Phase>4000) {Pot3Phase=4000;}
}
    
   	//Comuniacion Serial-- envio datos arduino
	//bits=(int) ((Dq*4095)/70);
	//memset(buffer,0,sizeof(buffer));
	//sprintf(buffer,"v%07d\n",bits);
	//serialPuts(fd,buffer);
	//serialFlush(fd);
	//tcflush(fd,TCIOFLUSH);

    //Validacion
    //printf("\n Promedio: %f",Prom[6]);
    //printf("\n Tgn: %f",Tgn);
    //printf("\n Fg: %f",Fg); //Funciona
    //printf("\n fi: %f",fi); //Funciona
    //printf("\n Fi_c: %f",Fi_c);
    //printf("\n Cc: %f",Cc);
    //printf("\n Fi_e: %f",Fi_e);
    //printf("\n Ce: %f",Ce);
    //printf("\n Xc: %f",Xc);
    //printf("\n Xe: %f",Xe);
    //printf("\n tc: %f",tc);
    //printf("\n te: %f",te);
    //printf("\n tcR: %f",PtcR);
    //printf("\n teR: %f",PteR);
    //printf("\n tq: %f",tq);
    //printf("\n tf: %f",tf);
    //printf("\n Xg: %f",Xg); //Funciona
    //printf("\n Alfa: %f",Alfa);
    //printf("\n cont: %d",cont);
    printf("\n Vel: %f",Vel);
    //printf("\n VelTot: %f",VelTot);
    printf("\n VelMax: %f",VelMax);
    //printf("\n VelMin: %f",VelMin);
    //printf("\n W: %f",W);
    //printf("\n teta: %f",teta);
    //printf("\n Acilc: %f",Acilc);
    //printf("\n Tg1ant: %f",Tg1ant);
    //printf("\n T1: %f",T1);
    //printf("\n Tcld: %f",Tcld);
    //printf("\n T11: %f",T11);
    //printf("\n T111: %f",T111);
    //printf("\n Acile: %f",Acile);
    //printf("\n Tg2ant: %f",Tg2ant);
    //printf("\n T2: %f",T2);
    //printf("\n Tht: %f",Tht);
    //printf("\n T22: %f",T22);
    //printf("\n T222: %f",T222);
    //printf("\n pcs: %f",pcs);
    //printf("\n ant: %f",ant);
    //printf("\n pcs2: %f",pcs2);
    //printf("\n Pg: %f",TG);
    //printf("\n Pg: %f",Pg);
    //printf("\n Vg: %f",Vg);
    //printf("\n tmec: %f",tmec);
    //printf("\n Pmec: %f",Pmec);
    //printf("\n vol: %f",vol);
	//printf("\n TENSION: %f",Vout);
	//printf("\n CORRIENTE: %f",Iout);//Corriente Dc
	//printf("\n VoContrEntrada: %f",VoContrEntrada);
	//printf("\n CONVERSORSUP: %f",DcVoSup);
	//printf("\n CONVERSORINF: %f",DcVoInf);
	printf("\n CONVERSOR: %f",DcVo);
	//printf("\n SUMVD: %f",SumaVD);
	//printf("\n SUMVD: %f",SumaVQ);
	//printf("\n ContrVod: %f",ContrVod);
	//printf("\n ContrVoq: %f",ContrVoq);
	//printf("\n DesDinVD: %f",DesDinVD);
	//printf("\n DesDinVQ: %f",DesDinVQ);
	//printf("\n SUMVD: %f",SumaID);
	//printf("\n SUMVQ: %f",SumaIQ);
	//printf("\n ContrIod: %f",ContrIod);
	//printf("\n ContrIoq: %f",ContrIoq);
	//printf("\n DesDinID: %f",DesDinID);
	//printf("\n DesDinIQ: %f",DesDinIQ);
	//printf("\n Md: %f",Md);
	//printf("\n Mq: %f",Mq);
	//printf("\n MA: %f",MA);
	//printf("\n MB: %f",MB);
	//printf("\n MC: %f",MC);
	//printf("\n InversorA: %f",InversorA);
	//printf("\n InversorB: %f",InversorB);
	//printf("\n InversorC: %f",InversorC);
	//printf("\n IoutInversorA: %f",IoutInversorA);
	//printf("\n IoutInversorB: %f",IoutInversorB);
	//printf("\n IoutInversorC: %f",IoutInversorC);
	printf("\n Vod: %f",Vod);
	//printf("\n Voq: %f",Voq);
	//printf("\n VoRef: %f",VoRef);
	//printf("\n Iinvd: %f",Iinvd);
	//printf("\n Iinvq: %f",Iinvq);
	//printf("\n Pot3Phase: %f",Pot3Phase);
    printf("\n cont: %f",cont);
    printf("\n ------------------------");


    //Txt
	  sprintf(text1,"%5.6f",pcs);
	  strcat(text1,"\t");
	  sprintf(text2,"%5.6f",pcs2);
	  strcat(text2,"\t");
	  sprintf(text3,"%5.6f",Vod);
	  strcat(text3,"\t");
	  sprintf(text4,"%5.6f",Voq);
	  strcat(text4,"\t");
	  sprintf(text5,"%5.6f",VoRef);
	  strcat(text5,"\t");
	  sprintf(text6,"%5.6f",InversorA);
	  strcat(text6,"\t");
	  sprintf(text7,"%5.6f",InversorB);
	  strcat(text7,"\t");
	  sprintf(text8,"%5.6f",InversorC);
	  strcat(text8,"\t");
	  sprintf(text9,"%5.6f",Tgn);
	  strcat(text9,"\t");
	  sprintf(text10,"%5.6f",te);
	  strcat(text10,"\t");
	  sprintf(text11,"%5.6f",tc);
	  strcat(text11,"\t");
	  sprintf(text12,"%5.6f",VelMax*9.549);
	  strcat(text12,"\t");
	  sprintf(text13,"%5.6f",Vel*9.549);
	  strcat(text13,"\t");
	  sprintf(text14,"%5.6f",tmec);
	  strcat(text14,"\t");
	  sprintf(text15,"%5.6f",Pmec*0.5);
	  strcat(text15,"\t");
	  sprintf(text16,"%5.6f",Vout);
	  strcat(text16,"\t");
	  sprintf(text17,"%5.6f",DcVo);
	  strcat(text17,"\t");
	  sprintf(text18,"%5.6f",TG);
	  strcat(text18,"\t");
	  sprintf(text19,"%5.6f",Pg);
	  strcat(text19,"\t");
	  sprintf(text20,"%5.6f",Vg);
	  strcat(text20,"\t");	
	  sprintf(text21,"%5.6f",cont);
	  strcat(text21,"\n");

    ////Guardar txt
	  fputs(text1,fichero);
	  fputs(text2,fichero);
	  fputs(text3,fichero);
	  fputs(text4,fichero);
	  fputs(text5,fichero);
	  fputs(text6,fichero);
	  fputs(text7,fichero);
	  fputs(text8,fichero);
	  fputs(text9,fichero);
	  fputs(text10,fichero);
	  fputs(text11,fichero);
	  fputs(text12,fichero);
	  fputs(text13,fichero);
	  fputs(text14,fichero);
	  fputs(text15,fichero);
	  fputs(text16,fichero);
	  fputs(text17,fichero);
	  fputs(text18,fichero);
	  fputs(text19,fichero);
	  fputs(text20,fichero);
	  fputs(text21,fichero);

//Muestra el vector
/*for(int i=0;i<N;i++){
  for(int j=0;j<jmax;j++){
    printf("%4.0f",u[i][j]);
  }
  printf("\n");
}*/
	return 0;

}

int main(int argc,char** argv) {
	//Time
	struct timespec t;
	struct sched_param param;
	int interval=4000; //en nanoseg--4s--1000000000-->-2 ceros para 10ms--agregar 1 cero para que funcione el inversor
	
	
	if(argc>=2 && atoi(argv[1])>0)
	{
		printf("using realtime, priority: %d\n",atoi(argv[1]));
		param.sched_priority = atoi(argv[1]);
		/* enable realtime fifo scheduling */
		if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
		perror("sched_setscheduler failed");
		exit(-1);
		}
	}
	if(argc>=3)
      interval=atoi(argv[2]);
      
	if (wiringPiSetup () == -1)
    return 1 ;
	pinMode (2, OUTPUT);  
    
	//Comunicacion serial
	//fd=serialOpen("/dev/ttyACM0",115200);
	//serialClose(fd);
	//fd=serialOpen("/dev/ttyACM0",115200);
	//sleep(1);
	
	
	
	alf=ku/(cp*rho);
	hi = L/(N-1);
	dt = (0.5*(pow(hi,2)))/alf;
	jmax = round(ttt/dt);
	
	//Creacion de matrix
	/*u = (double**) malloc(N * sizeof(double*));
	if (u==NULL){
	printf("\n Error, Arreglo vacio N");
	}else{
	for(int i=0;i<N;i++){
		u[i]=(double*) malloc(jmax * sizeof(double*));
		if (u[i]==NULL){
		printf("\n Error, Arreglo vacio Jmax");
		}
    
	}
	}*/
	
	
	/* get current time */
 	clock_gettime(0,&t);

	/* start after one second */
	t.tv_sec++;
  //for (int i = 0;i<=1000;i++)
	while(1)
	{
		/* wait untill next shot */
      clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
      /* do the stuff */
      
	if(estado==0){
		estado=1;
		}
	else{
		estado=0; 
	}
	digitalWrite (2, estado) ; 	//pin 13 gpio readall
	
	
	fichero =fopen("graficoB.txt","a");
	out();
	fclose(fichero);
	
	
	
	
	/* calculate next shot */
      t.tv_nsec+=interval;
      tsnorm(&t);
	//printf("\n Vg: %d",cont);
	if (cont>300){exit(-1);}
	if (cont<400){cont =cont+0.0001;}//Si no se pone el tiempo correcto no funcionan los controles
	}
	//}

	return 0;
}
