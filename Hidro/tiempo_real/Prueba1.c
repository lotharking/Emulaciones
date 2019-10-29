//gcc -o Prueba1 Prueba1.c -lrt -Wall -lm -lwiringPi


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
double iner = 50;//acomodar aqui(aun no se que pasa)-->efectos de modelado
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
double Iout=1;
double Rout=25;
double PTelec=0;
double Pelec=0;

//Conversor
double DcDuty=1;
double D1=1, D2=1, V1=1,V2=1;
double DcVo=1;

//Conversor corrriente
double DI1=1, DI2=1, I1=1,I2=1;


//Inversor
double sen[62]=  {0,0.1,0.2,0.3,0.39,0.48,0.56,0.64,0.72,0.78,0.84,0.89,0.93,0.96,1,1,0.99,0.97,0.95,0.91,0.86,0.81,0.75,0.68,0.6,0.52,0.43,0.33,0.24,0.14,0.04,-0.06,-0.16,-0.26,-0.35,-0.44,-0.53,-0.61,-0.69,-0.76,-0.82,-0.87,-0.92,-0.95,-0.98,-0.99,-1,-1,-0.98,-0.96,-0.93,-0.88,-0.83,-0.77,-0.71,-0.63,-0.55,-0.46,-0.37,-0.28,-0.18,-0.08};
double cose[62]= {1,1,0.99,0.97,0.95,0.91,0.86,0.81,0.75,0.68,0.6,0.52,0.43,0.33,0.24,0.14,0.04,-0.06,-0.16,-0.26,-0.35,-0.44,-0.53,-0.61,-0.69,-0.76,-0.82,-0.87,-0.92,-0.95,-0.98,-0.99,-1,-1,-0.98,-0.96,-0.93,-0.88,-0.83,-0.77,-0.71,-0.63,-0.55,-0.46,-0.37,-0.28,-0.18,-0.08,0,0.1,0.2,0.3,0.39,0.48,0.56,0.64,0.72,0.78,0.84,0.89,0.93,0.96};
double TinvSen=1, TinvCos=1;
double InDuty=1;
double InvSenVin1=1, InvSenVin2=1, InvSenVout1=1, InvSenVout2=1, InvCosVin1=1, InvCosVin2=1, InvCosVout1=1, InvCosVout2=1;
int cont=0;
double Mult1=1,Mult2=1;
double Dq=1;
double Pot=1;
double corrient=1;

//Control inversor
double InCont1=1,OutCont1=1, Ref=1, Suma=1;

//otras variables	
double acumVeloc=1;
int bits= 0;
FILE* fichero;
char text1[20],text2[20],text3[20],text4[20],text5[20],text6[20],text7[20];
char buffer[8],read_buffer[9];
int bytes_read = 0;
int fd;
int Vrio_char;

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
    

double landa(double wangular, double vrio, double r)
{
	double x=wangular*r;
	double y=x/(vrio+0.001);
	return y;
}

double landai(double landa, double beta)
{
	double x,y;
	x= (1/(landa + (0.08*beta)))-(0.035/((pow(beta,3))+1));
	y= 1/x;
	return y;
}
double cp(double beta, double landa, double landai)
{
	double w=0.5176;
	double w1=116/landai;
	double w2=0.4*beta;
	double w3=exp(-21/landai);
	double x=(w*(w1-w2-5)*w3)+(0.0068*landa);
	return x;
}
double potencia (double cp,double rad, double vrio)
{
	double Ar;
	double Po;
		
	Ar = ((PI*(pow(1,2)))/4);
	Po = (rad*1000*Ar*cp*(pow(vrio,3)));
	
	return Po;
}
double Tmec(double pot, double w)
{
	double x=pot/w;
	return x;
}
double Telec(double kp, double iout)
{
	double x= kp*iout;
	return x;
}
double aceleracion(double Tmec,double Telect, double Tfric, double iner, double Tvis)
{
	double x = Tmec - Telect - Tfric - Tvis;
	double y = x/iner;
	return y;
}
double tension(double Ri,double Kb,double Iout,double vel)
{
	double w=Kb*vel;
	double w1=Ri*Iout;
	double v=w+w1;
	return v;
}

double corrienteDc(double Vin,double DI1,double DI2,double I1,double I2)
{
	double Io=1;
	double Duty=150/Vin;
	if (Duty>1){Duty=1;}
	Io=(0.04697*DI1*Duty)-(0.04672*DI2*Duty)+(1.938*I1)-(0.9947*I2);
	return Io;
}

double conversor(double Vin, double DcVo,double D1,double D2,double V1,double V2)
{
	double Vo=1;
	double Duty=150/Vin;
	if (Duty>1){Duty=1;}
	Vo=(0.02828*D1*Duty)+(0.02823*D2*Duty)+(1.938*V1)-(0.9947*V2);
	return Vo;
}

double inversor(double InvVin1, double InvVin2, double InvVout1, double InvVout2)
{
	double Vo=1;
	Vo=(0.05413*InvVin1)+(0.04095*InvVin2)+(1.335*InvVout1)-(0.4327*InvVout2);
	return Vo;
}

double controlInv(double Suma, double InCont1,double OutCont1)
{
	double Duty=1;
	Duty=(0.5*Suma)-(0.495*InCont1)+(OutCont1);
	return Duty;
}

//metodo

void out()
{	
	fichero =fopen("grafico.txt","a");
	
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
	
	
	Planda=landa(acumVeloc,vrio,radio);
	Plandai=landai(Planda,beta);
	Pcp=cp(beta,Planda,Plandai);
	Ppot=potencia(Pcp,radio,vrio);
	Tvis= acumVeloc*B;
	PTmec= Tmec(Ppot,acumVeloc);
	acel = aceleracion(PTmec,PTelec,Tfric,iner, Tvis);
	acumVeloc=acumVeloc+acel;
	Vout= tension(Ri,Kb, Iout,acumVeloc);
	DcVo= conversor(Vout,DcDuty,D1,D2,V1,V2);
	V2 = V1;
	D2=D1;
	V1=DcVo;
	D1=Vout;
	Iout=Vout/225;
	//Iout= corrienteDc(Vout,DI1,DI2,I1,I2);
	//I2=I1;
	//DI2=DI1;
	//I1=Iout;
	//DI1=Vout;
	PTelec= Telec(Kp,Iout);
	Pelec=DcVo*Iout;
	Mult1= InDuty*DcVo*(sen[cont]);
	Mult2= InDuty*DcVo*(cose[cont]);
	TinvSen= inversor(InvSenVin1, InvSenVin2, InvSenVout1, InvSenVout2);
	InvSenVin2=InvSenVin1;
	InvSenVout2=InvSenVout1;
	InvSenVin1=Mult1;
	InvSenVout1=TinvSen;
	TinvCos= inversor(InvCosVin1, InvCosVin2, InvCosVout1, InvCosVout2);
	InvCosVin2=InvCosVin1;
	InvCosVout2=InvCosVout1;
	InvCosVin1=Mult2;
	InvCosVout1=TinvCos;
	Dq=sqrt(((TinvSen*TinvSen))+((TinvCos*TinvCos)));
	Suma=Ref-((1/39.2)*Dq);
	InDuty=controlInv(Suma,InCont1,OutCont1);
	InCont1=Suma;
	OutCont1=InDuty;
	corrient=Dq/40;
	Pot=corrient*Dq;
	//Iout=Dq/40;
	
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
	printf("\n VANGULAR: %f",acumVeloc);
	//printf("\n TENSION: %f",Vout);
	//printf("\n CORRIENTE: %f",Iout);//Corriente Dc
	//printf("\n CONVERSOR: %f",DcVo);//Tension en DC--->se pueden  graficar
	//printf("\n Dq: %f",Dq);//Tension en AC-->ideal tension que se desea ver
	//printf("\n POTENCIA-AC: %f",Pot);//Potencia Ac-->variable importante
	
	//acondicionar variables para txt
	sprintf(text1,"%5.2f",Planda);
	strcat(text1,"\t");
	sprintf(text2,"%5.2f",Pcp);
	strcat(text2,"\t");
	sprintf(text3,"%5.2f",InDuty);
	strcat(text3,"\t");
	sprintf(text4,"%5.2f",DcVo);
	strcat(text4,"\t");
	sprintf(text5,"%5.2f",Iout);
	strcat(text5,"\t");
	sprintf(text6,"%5.2f",Dq);
	strcat(text6,"\t");
	sprintf(text7,"%5.2f",Pot);
	strcat(text7,"\n");
	if (cont<61){cont+=1;}
	else {cont=0;}
	
	//Guardar txt
	fputs(text1,fichero);
	fputs(text2,fichero);
	fputs(text3,fichero);
	fputs(text4,fichero);
	fputs(text5,fichero);
	fputs(text6,fichero);
	fputs(text7,fichero);
	fclose(fichero);
}


int main(int argc,char** argv)
{
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
	out();
      
      /* calculate next shot */
      t.tv_nsec+=interval;
      tsnorm(&t);
	}	
	//}
	
	return 0;
}

