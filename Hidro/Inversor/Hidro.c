//gcc -o Prueba Hidro.c -lrt -Wall -lm -lwiringPi


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


#define PI 3.14159265
#define LOW_SPIN 2
#define MID_SPIN 5
#define NSEC_PER_SEC    1000000000//1000000000
int estado=0;

//Parametros mecanicos
double D=1;
double radio=0.5;
double beta=0;
double Tfric=0;
double iner = 0.00161919; //50;//acomodar aqui(aun no se que pasa)-->efectos de modelado
double vrio=2.8;
double B=0;
double Planda=0;
double Plandai;
double Pcp;
double Tvis;
double PTmec;
double Ppot;
double acel;

//Parametros motor DC
double Ri=0.973;
double Ji=1.72;
double Kp=5.6627;
double Kb=6.2;
double Bi=0.1;
double Vout;
double Iout=0;
double Rout=25;
double PTelec=0;
double Pelec=0;

//Conversor
double ConvInSup1=0, ConvInSup2=0, ConvOutSup1=0,ConvOutSup2=0;
double ConvInInf1=0, ConvInInf2=0, ConvOutInf1=0,ConvOutInf2=0;
double DcVo=0, DcVoSup = 0, DcVoInf = 0, Duty = 1;

//Conversor corrriente
double DI1=1, DI2=1, I1=1,I2=1;


//Inversor
double senFa[101]=  {0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1,1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8,0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48,-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96,-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48,-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0};
double coseFa[101]= {1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8,0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48,-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96,-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48,-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0,0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1};
double senFb[101]=  {0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48,-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96,-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48,-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0,0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1,1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8};
double coseFb[101]= {-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96,-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48,-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0,0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1,1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8,0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48};
double senFc[101]=  {-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48,-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0,0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1,1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8,0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48,-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96};
double coseFc[101]= {-0.42,-0.36,-0.3,-0.24,-0.18,-0.12,-0.06,0,0,0.06,0.12,0.18,0.24,0.3,0.36,0.42,0.48,0.54,0.58,0.64,0.68,0.72,0.78,0.8,0.84,0.88,0.9,0.92,0.96,0.96,0.98,1,1,1,1,1,0.98,0.96,0.96,0.92,0.9,0.88,0.84,0.8,0.78,0.72,0.68,0.64,0.58,0.54,0.48,0.42,0.36,0.3,0.24,0.18,0.12,0.06,0,-0.06,-0.12,-0.18,-0.24,-0.3,-0.36,-0.42,-0.48,-0.54,-0.58,-0.64,-0.68,-0.72,-0.78,-0.8,-0.84,-0.88,-0.9,-0.92,-0.96,-0.96,-0.98,-1,-1,-1,-1,-1,-0.98,-0.96,-0.96,-0.92,-0.9,-0.88,-0.84,-0.8,-0.78,-0.72,-0.68,-0.64,-0.58,-0.54,-0.48};
int cont = 0;
double Ws= 2* PI * 60;
double SumaVD = 0;
double InContrVD1 = 1;
double OutContrVD1 = 1;
double SumaVQ = 1;
double InContrVQ1 = 1;
double OutContrVQ1 = 1;
double VoRef = 220;
double Vod = 220;
double Voq = 0;
double Vo0 = 0;
double Iinvd = 0;
double Iinvq = 0;
double Iinv0 = 0;
double Cinv = 0.00001;
double Linv = 0.025;
double Vg = 150;
double ContrVod = 1;
double ContrVoq = 1;
double DesDinVD = 1;
double DesDinVQ = 1;
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
//double PruebaA = 0, PruebaB = 0, PruebaC = 0;
double ConT = 0,ConT2 = 0, CruceXCero = 0, InvAnterior = 1;

//Control inversor
double InCont1=1,OutCont1=1, Ref=1, Suma=1;

//otras variables
double acumVeloc=1;
int bits= 0;
FILE* fichero;
char text1[20],text2[20],text3[20],text4[20],text5[20],text6[20],text7[20],text8[20],text9[20];
char buffer[8],read_buffer[9];
int bytes_read = 0;
int fd;
int Vrio_char;

//Parametros integral
double IntIn1 = 1;
double IntOut1 = 1;

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

double landa(double wangular, double vrio, double r){
	double x=wangular*r;
	double y=x/(vrio+0.001);
	return y;
}

double landai(double landa, double beta){
	double x,y;
	x= (1/(landa + (0.08*beta)))-(0.035/((pow(beta,3))+1));
	y= 1/x;
	return y;
}
double cp(double beta, double landa, double landai){
	double w=0.5176;
	double w1=116/landai;
	double w2=0.4*beta;
	double w3=exp(-21/landai);
	double x=(w*(w1-w2-5)*w3)+(0.0068*landa);
	return x;
}
double potencia (double cp,double rad, double vrio){
	double Ar;
	double Po;

	Ar = ((PI*(pow(1,2)))/4);
	Po = (rad*1000*Ar*cp*(pow(vrio,3)));

	return Po;
}
double Tmec(double pot, double w){
	double x=pot/w;
	return x;
}
double Telec(double kp, double iout){
	double x= kp*iout;
	return x;
}
double aceleracion(double Tmec,double Telect, double Tfric, double iner, double Tvis){
	double x = Tmec - Telect - Tfric - Tvis;
	double y = x/iner;
	return y;
}
//Generador
double tension(double Ri,double Kb,double Iout,double vel){
	double w=Kb*vel;
	double w1=Ri*Iout;
	double v=w+w1;
	return v;
}
//Conversor DC-DC
double conversorSup(double ConvInS1,double ConvInS2,double ConvOutS1,double ConvOutS2){
	double Vo=1;
	Vo=(0.02833*ConvInS1)+(0.02833*ConvInS2)+(1.943*ConvOutS1)-(ConvOutS2);
	return Vo;
}   

double conversorInf(double ConvInI1,double ConvInI2,double ConvOutI1,double ConvOutI2){
	double Vo=1;
	Vo=(-1.189*ConvInI1)+(1.189*ConvInI2)+(1.943*ConvOutI1)-(ConvOutI2);
	return Vo;
}

//INVERSOR
//Control de tensión
double ControlVDQ(double SVDQ, double InCtrVDQ1, double OutCtrVDQ1)
{
	double Vo=1;
	Vo=(SVDQ * 0.5) - (0.495 * InCtrVDQ1) + (OutCtrVDQ1);
	return Vo;
} 

//Control de corriente
double ControlIDQ(double SIDQ, double InCtrIDQ1, double OutCtrIDQ1)
{
	double Vo=1;
	Vo=(SIDQ * 0.5) - (0.495 * InCtrIDQ1) + (OutCtrIDQ1);
	return Vo;
}

//DQ a ABC
//Primera linea
double PrimeraABC(double Vd, double Vq, double Vo)
{
	double V=1;
	double w = 2*PI*60;
	double V1 = sin(w*ConT);
	double V2 = cos(w*ConT);
	V = (Vd * V1) + (Vq * V2) + (Vo);
	return V;
}

//Segunda linea
double SegundaABC(double Vd, double Vq, double Vo)
{
	double V=1;
	double w = 2*PI*60;
	double V1 = -(sin(w*ConT)* 0.5)-(cos(w*ConT) * (sqrt(3)/2));
	double V2 = -(cos(w*ConT) * 0.5)+(sin(w*ConT) * (sqrt(3)/2));
	V = (Vd * V1)+ (Vq * V2) + (Vo);
	return V;
}

//Tercera linea
double TerceraABC(double Vd, double Vq, double Vo)
{
	double V=1;
	double w = 2*PI*60;
	double V1 = -(-(sin(w*ConT)* 0.5)-(cos(w*ConT) * (sqrt(3)/2)))-(sin(w*ConT));
	double V2 = -(-(cos(w*ConT) * 0.5)+(sin(w*ConT) * (sqrt(3)/2)))-(cos(w*ConT));
	V = (Vd * V1) + (Vq * V2) + (Vo);
	return V;
}

//ABC a DQ
//Primera linea
double PrimeraD(double VA, double VB, double VC)
{
	double V=1;
	double w = 2*PI*60;
	//double VH = 220*sin(w*ConT);	
	//double VI = 220*sin(w*ConT- 2*PI/3);	
	//double VJ = 220*sin(w*ConT+2*PI/3);	
	double V1 = sin(w*ConT);	
	double V2 = -0.5*sin(w*ConT)-sqrt(3)/2.0*cos(w*ConT);
	double V3 = -V2-sin(w*ConT);
	V = (2/3.0)*(VA*V1+VB*V2+VC*V3);
	//V = (2/3.0)*(VH*V1+VI*V2+VJ*V3);
	return V;
}

//Segunda linea
double SegundaQ(double VA, double VB, double VC)
{
	double V = 1;
	double w = 2*PI*60;
	//double VH = 220*sin(w*ConT);	
	//double VI = 220*sin(w*ConT- 2*PI/3);	
	//double VJ = 220*sin(w*ConT+2*PI/3);	
	double V1 = cos(w*ConT);	
	double V2 = sqrt(3)/2.0*sin(w*ConT)-0.5*cos(w*ConT);
	double V3 = -V2-cos(w*ConT);
	V = (2/3.0)*(VA*V1+VB*V2+VC*V3);
	//V = (2/3.0)*(VH*V1+VI*V2+VJ*V3);
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
	Vo=(0.01993*InvInS1)+(0.01993*InvInS2)+(1.96*InvOutS1)-(InvOutS2);
	//Vo=(0.5*in)+(InvInS1)+(0.5*InvInS2)-(InvOutS2);
	return Vo;
}   

double InversorInf(double inI, double InvInI1,double InvInI2,double InvOutI1,double InvOutI2){
	double Vo=1;
	Vo=(-9.933*InvInI1)+(9.933*InvInI2)+(1.96*InvOutI1)-(InvOutI2);
	return Vo;
}

double Referencia(double Retin, double RetIn1,double RetIn2,double RetOut1,double RetOut2){
	double Vo=1;
	Vo=(0.0198*RetIn1)+(0.9802*RetOut1);
	return Vo;
}

//metodo

void out(){

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
	
	//TURBINA DE RIO	
	Planda=landa(acumVeloc,vrio,radio);
	Plandai=landai(Planda,beta);
	Pcp=cp(beta,Planda,Plandai);
	Ppot=potencia(Pcp,radio,vrio);
	Tvis= acumVeloc*B;
	PTmec= Tmec(Ppot,acumVeloc);
	acel = aceleracion(PTmec,PTelec,Tfric,iner, Tvis);
	acumVeloc = integral1(IntIn1,IntOut1);
	IntIn1 = acel;
	IntOut1 = acumVeloc;

	Vout= tension(Ri,Kb, Iout,acumVeloc);
	
	//CONVERSOR DC-DC
	Duty=(300/Vout);
	if (Duty>1){Duty=1;}
	DcVoSup =  conversorSup(ConvInSup1,ConvInSup2,ConvOutSup1,ConvOutSup2);
	ConvOutSup2 = ConvOutSup1;
	ConvInSup2 = ConvInSup1;
	ConvOutSup1 = DcVoSup;
	ConvInSup1 = Vout*Duty;
	
	DcVoInf = conversorInf(ConvInInf1,ConvInInf2,ConvOutInf1,ConvOutInf2);
	ConvOutInf2 = ConvOutInf1;
	ConvInInf2 = ConvInInf1;
	ConvOutInf1 = DcVoInf;
	ConvInInf1 = Iout;
	
	DcVo = DcVoSup + DcVoInf + 0.01;
	Iout=DcVo/225.0;
	
	Pelec=DcVo*Iout;
	PTelec= Telec(Kp,Iout);
	
	//INVERSOR
	//Referencia
	VoRef = Referencia(220,RefIn1,RefIn2,RefOut1,RefOut2);
	RefIn1 = 220;
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
	DesDinVD = 5.5 - (Cinv * Voq * Ws);
	DesDinVQ = -0.8294 + (Cinv * Vod * Ws);
	//DesDinVD = ContrVod - (Cinv * Voq * Ws);
	//DesDinVQ = ContrVoq + (Cinv * Vod * Ws);
	
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
	Md = (DesDinID + Vod) * (2/Vg);
	Mq = (DesDinIQ + Voq) * (2/Vg);
	
	if (Md > 1){Md = 1;}
	else if (Md < -1){Md = -1;}
	else {Md = Md;}
	
	if (Mq > 1){Mq = 1;}
	else if (Mq < -1){Mq = -1;}
	else {Mq = Mq;}
	
	//Generación MABC
	//MA = PrimeraABC(0.7160, 0.1856, 0);
	//MB = SegundaABC(0.7160, 0.1856, 0);
	//MC = TerceraABC(0.7160, 0.1856, 0);
	MA = PrimeraABC(Md, Mq, 0);
	MB = SegundaABC(Md, Mq, 0);
	MC = TerceraABC(Md, Mq, 0);
	
	//Inversor
	InvSupA = InversorSup(MA * 300,InvSupAIn1,InvSupAIn2,InvSupAOut1,InvSupAOut2);
	InvSupAOut2 = InvSupAOut1;
	InvSupAIn2=InvSupAIn1;
	InvSupAOut1=InvSupA;
	InvSupAIn1=MA * 300;
	InvInfA = InversorInf(IoutInversorA,InvInfAIn1,InvInfAIn2,InvInfAOut1,InvInfAOut2);
	InvInfAOut2 = InvInfAOut1;
	InvInfAIn2=InvInfAIn1;
	InvInfAOut1=InvInfA;
	InvInfAIn1=IoutInversorA;
	InvSupB = InversorSup(MB * 300,InvSupBIn1,InvSupBIn2,InvSupBOut1,InvSupBOut2);
	InvSupBOut2 = InvSupBOut1;
	InvSupBIn2=InvSupBIn1;
	InvSupBOut1=InvSupB;
	InvSupBIn1=MB * 300;
	InvInfB = InversorInf(IoutInversorB,InvInfBIn1,InvInfBIn2,InvInfBOut1,InvInfBOut2);
	InvInfBOut2 = InvInfBOut1;
	InvInfBIn2=InvInfBIn1;
	InvInfBOut1=InvInfB;
	InvInfBIn1=IoutInversorB;
	InvSupC = InversorSup(MC * 300,InvSupCIn1,InvSupCIn2,InvSupCOut1,InvSupCOut2);
	InvSupCOut2 = InvSupCOut1;
	InvSupCIn2=InvSupCIn1;
	InvSupCOut1=InvSupC;
	InvSupCIn1=MC * 300;
	InvInfC = InversorInf(IoutInversorC,InvInfCIn1,InvInfCIn2,InvInfCOut1,InvInfCOut2);
	InvInfCOut2 = InvInfCOut1;
	InvInfCIn2=InvInfCIn1;
	InvInfCOut1=InvInfC;
	InvInfCIn1=IoutInversorC;
	
	//Tensión y corriente Inversor trifasica
	InvAnterior = InversorA;
	InversorA = InvSupA + InvInfA;
	InversorB = InvSupB + InvInfB;
	InversorC = InvSupC + InvInfC;
	IoutInversorA = InversorA/40.0;
	IoutInversorB = InversorB/40.0;
	IoutInversorC = InversorC/40.0;
	
	//Prueba
	if (InversorA>=0 && InvAnterior<0){CruceXCero=1;}
	else {CruceXCero=0;}
	if (CruceXCero == 1){ConT2 = 0.000;}
	//else {ConT += 0.001;}
	
	//Tensión y corriente Inversor DQ
	//Vod = PrimeraD(1, 2, 3);
	//Voq = SegundaQ(1, 2, 3);
	Vod = PrimeraD(InversorA, InversorB, InversorC);
	Voq = SegundaQ(InversorA, InversorB, InversorC);
	Vo0 = Tercera0(InversorA, InversorB, InversorC);
	Iinvd = PrimeraD(IoutInversorA, IoutInversorB, IoutInversorC);
	Iinvq = SegundaQ(IoutInversorA, IoutInversorB, IoutInversorC);
	Iinv0 = Tercera0(IoutInversorA, IoutInversorB, IoutInversorC);
	
	
	//Comuniacion Serial-- envio datos arduino
	//bits=(int) ((Dq*4095)/70);
	//memset(buffer,0,sizeof(buffer));
	//sprintf(buffer,"v%07d\n",bits);
	//serialPuts(fd,buffer);
	//serialFlush(fd);
	//tcflush(fd,TCIOFLUSH);

	//impresion
	//printf("\n Variable 1: %f",Plandai);
	//printf("\n Variable 2: %d",bits);
	//printf("\n Variable 3: %c",buffer[4]);
	//printf("\n Variable 4: %f",Planda);
	//printf("\n Variable: %f",Pcp);

	//printf("\n VELOCIDAD: %f",vrio);
	//printf("\n ANGULAR: %f",acumVeloc);
	//printf("\n Dq: %f",Dq);

	//printf("\n LANDA: %f",Planda);
	//printf("\n LANDAI: %f",Plandai);
	//printf("\n CP : %f",Pcp);
	//printf("\n POTENCIA: %f",Ppot);
	//printf("\n PARE: %f",PTelec);
	//printf("\n PARM: %f",PTmec);
	//printf("\n ACEL: %f",acel);
	//printf("\n VANGULAR: %f",acumVeloc);
	//printf("\n TENSION: %f",Vout);
	//printf("\n CORRIENTE: %f",Iout);//Corriente Dc
	//printf("\n CONVERSORSUP: %f",DcVoSup);
	//printf("\n CONVERSORINF: %f",DcVoInf);
	//printf("\n CONVERSOR: %f",DcVo);//Tension en DCSumaVD
	//printf("\n SUMVD: %f",SumaVD);
	//printf("\n SUMVD: %f",SumaVQ);
	//printf("\n RCURRENT: %f",DesDinVD);
	//printf("\n RCURRENT: %f",DesDinVQ);
	//printf("\n SUMVD: %f",SumaID);
	//printf("\n SUMVQ: %f",SumaIQ);
	//printf("\n RCURRENTD: %f",DesDinID);
	//printf("\n RCURRENTD: %f",DesDinIQ);
	//printf("\n ContrIod: %f",ContrIod);
	//printf("\n ContrIoq: %f",ContrIoq);
	printf("\n Md: %f",Md);
	printf("\n Mq: %f",Mq);
	//printf("\n MA: %f",MA);
	//printf("\n MB: %f",MB);
	//printf("\n MC: %f",MC);
	//printf("\n InversorA: %f",InversorA);
	//printf("\n InversorB: %f",InversorB);
	//printf("\n InversorC: %f",InversorC);
	//printf("\n IoutInversorA: %f",IoutInversorA);
	//printf("\n IoutInversorB: %f",IoutInversorB);
	//printf("\n IoutInversorC: %f",IoutInversorC);
	//printf("\n Vod: %f",Vod);
	//printf("\n Voq: %f",Voq);
	//printf("\n Iinvd: %f",Iinvd);
	//printf("\n Iinvq: %f",Iinvq);
    printf("\n ------------------------");

	//acondicionar variables para txt
	sprintf(text1,"%5.4f",sin(2*PI*60*ConT));
	strcat(text1,"\t");
	sprintf(text2,"%5.4f",InversorA/220.0);
	strcat(text2,"\t");
	sprintf(text3,"%5.4f",(sin(2*PI*60*ConT)*0.5)+(cos(2*PI*60*ConT)*(0.8660254037844386))-sin(2*PI*60*ConT));
	strcat(text3,"\t");
	sprintf(text4,"%5.4f",(2/3.0)*((InversorA*cos(2*PI*60*ConT))+InversorB*(sin(2*PI*60*ConT) * (sqrt(3)/2.0)-(cos(2*PI*60*ConT) * 0.5))+InversorC*(((cos(2*PI*60*ConT) * 0.5)-(sin(2*PI*60*ConT) * (sqrt(3)/2.0)))-(cos(2*PI*60*ConT)))));
	strcat(text4,"\t");
	sprintf(text5,"%5.4f",sin(2*PI*60*ConT) * (sqrt(3)/2)-(cos(2*PI*60*ConT) * 0.5));
	strcat(text5,"\t");
	sprintf(text6,"%5.4f",((cos(2*PI*60*ConT) * 0.5)-(sin(2*PI*60*ConT) * (sqrt(3)/2)))-(cos(2*PI*60*ConT)));
	strcat(text6,"\t");
	sprintf(text7,"%5.4f",InversorA);
	strcat(text7,"\t");
	sprintf(text8,"%5.4f",InversorB);
	strcat(text8,"\t");
	sprintf(text9,"%5.4f",InversorC);
	strcat(text9,"\n");
	if (ConT<0.016){ConT+=(0.001/10.0);}//ajustar con el tiempo real
	else {ConT=0;}
	if (ConT2<0.016){ConT2+=(0.001/10.0);}//ajustar con el tiempo real
	else {ConT2=0;}
	//if (cont<100){cont+=1;}
	//else {cont=0;}

	//Guardar txt
	fputs(text1,fichero);
	fputs(text2,fichero);
	fputs(text3,fichero);
	fputs(text4,fichero);
	fputs(text5,fichero);
	fputs(text6,fichero);
	fputs(text7,fichero);
	fputs(text8,fichero);
	fputs(text9,fichero);
}
int main(int argc,char** argv) {
	//Time
	struct timespec t;
	struct sched_param param;
	int interval=4000000; //en nanoseg--4s--1000000000-->-2 ceros para 10ms
	
	
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
   pinMode (2, OUTPUT) ;   
	
	
	fd=serialOpen("/dev/ttyACM0",115200);
	serialClose(fd);
	fd=serialOpen("/dev/ttyACM0",115200);
	
	sleep(1);
	
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
	
	fichero =fopen("graficoH.txt","a");
	out();
	fclose(fichero);
	/* calculate next shot */
      t.tv_nsec+=interval;
      tsnorm(&t);
	}
	//}

	return 0;
}
