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
#include <wiringPiSPI.h>

const int minutes = 60;
const int seconds = 1;


#define PI 3.14159265
#define LOW_SPIN 2
#define MID_SPIN 5
#define NSEC_PER_SEC    1000000000//1000000000
int estado=0;

//Prueba
double Planda1=0,Planda2=0,Planda3=0,Planda4=0,Planda5=0,Plandai1,Plandai2,Plandai3,Plandai4,Plandai5;
double acumVeloc1=1,acumVeloc2=1,acumVeloc3=1,acumVeloc4=1,acumVeloc5=1,Pcp1,Pcp2,Pcp3,Pcp4,Pcp5;
double Ppot1,Ppot2,Ppot3,Ppot4,Ppot5,acel1,acel2,acel3,acel4,acel5,PTmec1,PTmec2,PTmec3,PTmec4,PTmec5;
double Pin1=1,Pin2=1,Pin3=1,Pin4=1,Pin5=1,Pout1=1,Pout2=1,Pout3=1,Pout4=1,Pout5=1;
double Vout1,Vout2,Vout3,Vout4,Vout5,NVel,NTmec;


//Entradas
double vrio=2.8;
double Rcarga=50;

//Parametros mecanicos
double D=1;
double radio=0.5;//D/2
double beta=0;
double Tfric=0;
double iner = 0.0465;//0.00161919; //50;//acomodar aqui(aun no se que pasa)-->efectos de modelado
double B=0;
double Planda=0;
double Plandai;
double Pcp;
double Tvis;
double PTmec;
double Ppot;
double acel;

//Parametros Generador DC
double Ri=2.581;
double Ji=0.0465;
double Kp=5.6627;
double Kb=1.25;
double Bi=0.002953;
double Li=0.0281;

double Vout;
double Iout=0;
double PTelec=0;
double Pelec=0;

//Conversor
double ConvInSup1=0, ConvInSup2=0, ConvOutSup1=0,ConvOutSup2=0;
double ConvInInf1=0, ConvInInf2=0, ConvOutInf1=0,ConvOutInf2=0;
double DcVo=0, DcVoSup = 0, DcVoInf = 0, Duty = 0, Duty1 = 0, DCDiv = 0, DCDiv2 = 0;
double ContIIn = 0, ContIIn1 = 0, ContIIn2 = 0, ContIOut1 = 0, ContIOut2 = 0;
double VoContrEntrada = 0,ContrDcVo = 0;
double SalidaVDCDeseada = 250;


//Conversor corrriente
double DI1=1, DI2=1, I1=1,I2=1;


//Inversor
int cont = 0;
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
double Vg = 150;
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
double ConT = 0,ConT2 = 0, CruceXCero = 0, InvAnterior = 0, PLL = 0;
double FilIn1D = 0, FilIn2D = 0, FilOut1D = 0, FilOut2D = 0, VodPrueba = 0;
double FilIn1Q = 0, FilIn2Q = 0, FilOut1Q = 0, FilOut2Q = 0, VoqPrueba = 0;
double VodAR[16], VoqAR[16];
double Pot3Phase = 0;

//Control inversor
//double InCont1=0,OutCont1=0, Ref=1, Suma=1;

//otras variables
double acumVeloc=1;
int bits= 0,bits2= 0,bits3= 0;
FILE* fichero;
char text1[20],text2[20],text3[20],text4[20],text5[20],text6[20],text7[20],text8[20],text9[20],text10[20],text11[20],text12[20],text13[20];
char text14[20],text15[20],text16[20],text17[20],text18[20],text19[20],text20[20],text21[20],text22[20],text23[20],text24[20],text25[20];
char text26[20],text27[20],text28[20],text29[20];
char buffer[9],read_buffer[125],buffer2[9],buffer3[9],buffer4[16],buffer5[24],inputCharArray[125],inChar,delimitador[] = ",";
int bytes_read = 0;
int fd, i, j;
int Vrio_char;
double PSend;
int dato1,dato2,dato3,dato4;
bool stringComplete,conti;

//Parametros integral
double IntIn1 = 1;
double IntOut1 = 1;


extern int clock_nanosleep(clockid_t __clock_id, int __flags,
      __const struct timespec *__req,
      struct timespec *__rem);


static inline void tsnorm(struct timespec *ts)
{
   while (ts->tv_nsec >= NSEC_PER_SEC) {
      ts->tv_nsec -= NSEC_PER_SEC;
      ts->tv_sec++;
   }
}


double integral1(double IntIn1, double IntOut1){
	double intg=1;
	intg=(0.0001*IntIn1)+(IntOut1);
	return intg;
}

double landa(double wangular, double vrio, double r){
	double x=wangular*r;
	double y=x/(vrio+0.01);
	return y;
}

double landai(double landa, double beta){
	double x,y;
	x= (1/(0.01 + landa + (0.08*beta)))-(0.035/((pow(beta,3))+1));
	y= 1/(x+0.01);
	return y;
}
double cp(double beta, double landa, double landai){
	double w=0.5176;
	double w1=116/(landai+0.001);
	double w2=0.4*beta;
	double w3=exp(-21/(landai+0.001));
	double x=(w*(w1-w2-5)*w3)+(0.0068*landa);
	return x;
}
double potencia (double cp,double rad, double vrio){
	double Ar;
	double Po;

	Ar = ((PI*(pow(rad,2))));
	Po = (rad*1000*Ar*cp*(pow(vrio,3)));

	return Po;
}
double Tmec(double pot, double w){
	double x=pot/(w+0.01);
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
	double w=Kb*vel*2.4;
	double w1=Ri*Iout;
	double v=w+w1;
	return v;
}
//Conversor DC-DC
double conversorSup(double ConvIn, double ConvInS1,double ConvInS2,double ConvOutS1,double ConvOutS2){
	double Vo=1;
	//Vo=(0.014*ConvIn)+(0.02801*ConvInS1)+(0.014*ConvInS2)+(1.94*ConvOutS1)-(0.9961*ConvOutS2);//Buck
	//Vo=(0.01684*ConvIn)+(0.03367*ConvInS1)+(0.01684*ConvInS2)+(1.93*ConvOutS1)-(0.9976*ConvOutS2);//Boost
	Vo=(0.01684*ConvIn)+(0.03367*ConvInS1)+(0.01684*ConvInS2)+(1.93*ConvOutS1)-(0.9976*ConvOutS2);
	return Vo;
}

double conversorInf(double ConvIn1,double ConvInI2,double ConvOutI1,double ConvOutI2){
	double Vo=1;
	Vo=(0.001233*ConvIn1)-(0.001233*ConvInI2)+(1.998*ConvOutI1)-(0.9975*ConvOutI2);//Boost
	return Vo;
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
	double V1 = sin(w*ConT);
	double V2 = cos(w*ConT);
	V = (Vd * V1) + (Vq * V2) + (Vo);
	return V;
}

//Segunda linea
double SegundaABC(double Vd, double Vq, double Vo){
	double V=1;
	double w = 2*PI*60;
	double V1 = -(sin(w*ConT)* 0.5)-(cos(w*ConT) * (sqrt(3)/2));
	double V2 = -(cos(w*ConT) * 0.5)+(sin(w*ConT) * (sqrt(3)/2));
	V = (Vd * V1)+ (Vq * V2) + (Vo);
	return V;
}

//Tercera linea
double TerceraABC(double Vd, double Vq, double Vo){
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
	double V1 = sin(w*ConT);	
	double V2 = -0.5*sin(w*ConT)-sqrt(3)/2.0*cos(w*ConT);
	double V3 = -V2-sin(w*ConT);
	V = (2/3.0)*(VA*V1+VB*V2+VC*V3);
	return V;
}

//Segunda linea
double SegundaQ(double VA, double VB, double VC)
{
	double V = 1;
	double w = 2*PI*60;
	double V1 = cos(w*ConT);	
	double V2 = sqrt(3)/2.0*sin(w*ConT)-0.5*cos(w*ConT);
	double V3 = -V2-cos(w*ConT);
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
	return Vo;
}

double Filtro(double FilIn, double FilIn1,double FilIn2,double FilOut1,double FilOut2){
	double Vo=1;
	Vo=(0.002743*FilIn1)+(0.002743*FilIn2)+(1.893*FilOut1)-(0.8989*FilOut2);
	return Vo;
}

//metodo

void out(){

	//Lectura Vrio serial-- aqui lee la velocidad del rio
	
	//stringComplete = false;
    //conti = true;
    //serialFlush(fd);
    
    //memset(inputCharArray, 0, sizeof(inputCharArray));
	//while (conti) {
        ////printf("Buscando new line\n");
      //inChar = serialGetchar(fd);
      //if (inChar == 'f') {
        ////printf("Primer new line\n");
        //j = 0;
        //while (!stringComplete) {
          //while (serialDataAvail (fd) > 0  && conti) {
            //inChar = serialGetchar(fd);
            //if (inChar == 'e') {
              //stringComplete = true;
              //conti = false;
            //} else {
              //inputCharArray[j] = inChar;
              //j++;
            //}
          //}
        //}
      //}
     //}
    //serialFlush(fd);
    
    //i=0;
    //char *token = strtok(inputCharArray, delimitador);
    //dato1=atoi(token);
    //while(token != NULL){
            //token = strtok(NULL, delimitador);
            //if(i==0){dato2=atoi(token);}
            //if(i==1){dato3=atoi(token);}
            //if(i==2){dato4=atoi(token);}
            //i++;
        //}
    //printf("\n dato1: %d",dato1);
    //printf("\n dato2: %d",dato2);
    //printf("\n dato3: %d",dato3);
    //printf("\n dato4: %d",dato4);
	
	////TURBINA DE RIO	
	Planda=landa(acumVeloc,vrio,radio);
	Plandai=landai(Planda,beta);
	Pcp=cp(beta,Planda,Plandai);		
	Ppot=potencia(Pcp,radio,vrio);
	if (Ppot>5000){Ppot=5000;}	
	if (Ppot<0){Ppot=0;}	
	Tvis= acumVeloc*B;
	PTmec= Tmec(Ppot,acumVeloc);
	if (PTmec<0){PTmec=0;}
	
	acel = aceleracion(PTmec,PTelec,Tfric,iner, Tvis);
	acumVeloc = integral1(IntIn1,IntOut1);
	IntIn1 = acel;
	IntOut1 = acumVeloc;
	if (acumVeloc<0){acumVeloc=0;}
	Vout= tension(Ri,Kb, Iout,acumVeloc);
	
	NTmec=PTmec*0.98*(1/2.4);
	NVel=acumVeloc*2.4;
	
	
	//printf("\n %f",Planda);
	//printf("\n %f",Plandai);
	//printf("\n %f",Pcp);
	
	
	
	//Par electrico
	if(ConT>0.1){
	PTelec= Telec(Kp,Iout);//No olvidar el signo
} else {PTelec= 0;}//Solo funciona para las pruebas
	
	if (PTelec<0){PTelec=0;}
	
	//CONVERSOR DC-DC
	DcVo = DcVoSup + 0.001;
	if (DcVo>400) {DcVo=400;}
	if (DcVo<0) {DcVo=0;}
	Duty=(Vout/SalidaVDCDeseada);
	if (Duty>0.9){Duty=0.9;}
	DCDiv = Vout/(Duty);
	
	
	Duty1=(Vout/DcVo);
	DCDiv2 = Vout/(Duty1);
			
	//Entrada conversor
	ContrDcVo=DcVo/Vout;
	
	//Control
	VoContrEntrada = ControlDcVo(SalidaVDCDeseada - DcVo,ContIIn1,ContIOut1);
	ContIOut1 = VoContrEntrada;
	ContIIn1 = SalidaVDCDeseada - DcVo;
	
	//Conversor
	DcVoSup = Vout/(1 - VoContrEntrada);
	//DcVoSup =  conversorSup(VoContrEntrada,ConvInSup1,ConvInSup2,ConvOutSup1,ConvOutSup2);
	//ConvOutSup2 = ConvOutSup1;
	//ConvInSup2 = ConvInSup1;
	//ConvOutSup1 = DcVoSup;
	//ConvInSup1 = VoContrEntrada;
	
	
	
	//DcVoInf = conversorInf(ConvInInf1,ConvInInf2,ConvOutInf1,ConvOutInf2);
	//ConvOutInf2 = ConvOutInf1;
	//ConvInInf2 = ConvInInf1;
	//ConvOutInf1 = DcVoInf;
	//ConvInInf1 = Iout;
	//if (DcVoInf>5){DcVoInf=5;}
	
	
	
			
	Iout=Pot3Phase/(DcVo);//Pot3Phase/DcVo;
	if (Iout>15){Iout=15;}
	if (Iout<0){Iout=0;}
	//if (acumVeloc<0){Iout=0;}
	
	
	
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
	Md = (DesDinID + Vod) * (2/Vg);
	Mq = (DesDinIQ + Voq) * (2/Vg);
	
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
	IoutInversorA = InversorA/Rcarga;//Valor leido del archivo
	IoutInversorB = InversorB/Rcarga;
	IoutInversorC = InversorC/Rcarga;
	
	
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
	if (Pot3Phase<0) {Pot3Phase=0;}
	
	
	//Comuniacion Serial-- envio datos arduino
	
	//bits =(int) (((Md+1)*1000));//4095 para DAC
	//bits2=(int) (((Mq+1)*1000));//4095 para DAC
	//bits3=(int) (((MC+2)*1000));//4095 para DAC
	
	//memset(buffer5,0,sizeof(buffer5));
	
	//sprintf(buffer5,"v%07dw%07dz%07d\n",bits,bits2,bits3);
	//serialPuts(fd,buffer5);
	//serialFlush(fd);
	//tcflush(fd,TCIOFLUSH);

	//impresion
	//printf("\n Variable 2: %d",bits2);
	//printf("\n PSend 3: %f",PSend);
	//printf("\n Variable 0: %c",buffer5[0]);
	printf("\n VELOCIDAD: %f",vrio);
	printf("\n Rcarga: %f",Rcarga);
	//printf("\n LANDA: %f",PTmec1);
	//printf("\n LANDAI: %f",Ppot1);
	//printf("\n CP : %f",acumVeloc1);
	//printf("\n POTENCIA: %f",Ppot);
	//printf("\n PARE: %f",PTelec);
	//printf("\n PARM: %f",PTmec);
	//printf("\n ACEL: %f",acel);
	printf("\n VANGULAR: %f",acumVeloc);
	//printf("\n TENSION: %f",Vout);
	printf("\n CORRIENTE: %f",Iout);//Corriente Dc
	//printf("\n VoContrEntrada: %f",VoContrEntrada);
	//printf("\n CONVERSORSUP: %f",DcVoSup);
	//printf("\n CONVERSORINF: %f",DcVoInf);
	//printf("\n CONVERSOR: %f",DcVo);
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
	printf("\n Pot3Phase: %f",Pot3Phase);
	printf("\n ConT: %f",ConT);
	//printf("\n ConT2: %f",ConT2);
    printf("\n ------------------------");
    
    
	
	//acondicionar variables para txt
	sprintf(text1,"%5.4f",NVel);
	strcat(text1,"\t");
	sprintf(text2,"%5.4f",acumVeloc);
	strcat(text2,"\t");
	sprintf(text3,"%5.4f",Vout);
	strcat(text3,"\t");
	sprintf(text4,"%5.4f",Iout);
	strcat(text4,"\t");
	sprintf(text5,"%5.4f",DcVo);
	strcat(text5,"\t");
	sprintf(text6,"%5.4f",PTmec5);
	strcat(text6,"\t");
	sprintf(text7,"%5.4f",acumVeloc);
	strcat(text7,"\t");
	sprintf(text8,"%5.4f",acumVeloc1);
	strcat(text8,"\t");
	sprintf(text9,"%5.4f",acumVeloc2);
	strcat(text9,"\t");
	sprintf(text10,"%5.4f",Pcp);
	strcat(text10,"\t");
	sprintf(text11,"%5.4f",Md);
	strcat(text11,"\t");
	sprintf(text12,"%5.4f",Mq);
	strcat(text12,"\t");
	sprintf(text13,"%5.4f",InversorA);
	strcat(text13,"\t");
	sprintf(text14,"%5.4f",Vout);
	strcat(text14,"\t");
	sprintf(text15,"%5.4f",Vod);
	strcat(text15,"\t");
	sprintf(text16,"%5.4f",Voq);
	strcat(text16,"\t");
	sprintf(text17,"%5.4f",Iinvd);
	strcat(text17,"\t");
	sprintf(text18,"%5.4f",Iinvq);
	strcat(text18,"\t");
	sprintf(text19,"%5.4f",Pot3Phase);
	strcat(text19,"\t");
	sprintf(text20,"%5.4f",IoutInversorA);
	strcat(text20,"\t");
	//sprintf(text21,"%5.4f",IoutInversorA);
	//strcat(text21,"\t");
	//sprintf(text22,"%5.4f",IoutInversorB);
	//strcat(text22,"\t");
	//sprintf(text23,"%5.4f",IoutInversorC);
	//strcat(text23,"\t");
	//sprintf(text24,"%5.4f",VoRef);
	//strcat(text24,"\t");
	//sprintf(text25,"%5.4f",VoRef);
	//strcat(text25,"\t");
	//sprintf(text26,"%5.4f",VoRef);
	//strcat(text26,"\t");
	//sprintf(text27,"%5.4f",VoRef);
	//strcat(text27,"\t");
	//sprintf(text28,"%5.4f",VoRef);
	//strcat(text28,"\t");
	sprintf(text29,"%5.4f",ConT);//Aqui contador
	strcat(text29,"\n");

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
	//fputs(text21,fichero);
	//fputs(text22,fichero);
	//fputs(text23,fichero);
	//fputs(text24,fichero);
	//fputs(text25,fichero);
	//fputs(text26,fichero);
	//fputs(text27,fichero);
	//fputs(text28,fichero);
	fputs(text29,fichero);

	
	//contador
	//printf("\n se va a contar");
	if (ConT<10){ConT+=(0.1/1000.0);}//ajustar con el tiempo real
	else {ConT=0.0;}
	//printf("\n se ha contado");
	if (ConT2<0.01){ConT2+=0.1/1000.0;}//ajustar con el tiempo real
	else {ConT2=0.0;}
}
int main(int argc,char** argv) {
	//Time
	struct timespec t;
	struct sched_param param;
	int interval=10000000; //en nanoseg--4s--1000000000-->-->10 ms
	
	FILE * fp;
	int aaa,bbb;
	long int fsize;
	
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
   
   

	
	//Activar para la comunicacion serial
	//fd=serialOpen("/dev/ttyACM0",115200);
	//serialClose(fd);
	//fd=serialOpen("/dev/ttyACM0",115200);
	//sleep(1);
	
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
	
	
   
	fp = fopen ("Entrada.txt", "r");
	fseek(fp, 0, SEEK_END);//Pone el puntero al final del archivo
	fsize = ftell(fp);//Determina la posicion del puntero
	fseek(fp, fsize-5, SEEK_SET);//Setea el puntero en la posicion de los ultimos datos
	fscanf(fp,"%i,%i",&aaa,&bbb);//Asigna a las variables los valores
	fclose(fp);
	vrio=((double)aaa)/10.0;
	Rcarga=(double)bbb;
	
	//Evita valores indeseados
	//if (vrio<1){vrio=1;}	
	//if (vrio>2.8){vrio=2.8;}
	//if (Rcarga<15){Rcarga=15;}	
	//if (Rcarga>50){Rcarga=50;}

	
	fichero =fopen("graficoH.txt","a");
	out();
	fclose(fichero);
	//Rcarga=Rcarga-0.0035;
	//vrio=vrio-0.0002;
	//if (ConT>1){exit(-1);}
	//if (ConT>0.5){Rcarga=30;}
	
	
	/* calculate next shot */
      t.tv_nsec+=interval;
      tsnorm(&t);
	}
	//}

	return 0;
}
