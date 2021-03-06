﻿#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Constantes
#define PI 3.14159

//Contadores
int cont = 0;
double retro = 0;
int contVect = 0;

//arreglos
double PteR[650];
double PtcR[650];
double u[45][185];
//Acondicionaamiento Torque
double teR = 0;
double tcR = 0;

//Variables globales
double h =1000; //coeficiente global de transferencia de calor (W/m^2*K)
double Awr = 0.0058; //area de transferencia de calor de la rejilla
double dg = 1.23; //densidad del gas de trabajo(kg/m^3)
double cpg = 1006; //calor especifico del gas de trabajo (J/Kg*k)
double Rg = 0.287; //constante de gas ideal(K/Kg)
double Xwr = 0.04; //Longitud del Regenerador
double lwr = 0.06; //Longitud del regenerador en metros.
double b = 0.0000000001; //Coeficiente de fricción
double c = 0.012; //coeficiente de carga !!OJO CAMBIO!!
double Patm = 101.325; //presion atmosferica (kpa)//HACER PRUEBA CON 101325--->PREGUNTAR A KEVIN
double Ap = 0.0007068; //area del piston (m^2)
double Fg = 0;
double r = 0.02; //radio del cigueñal (m)
double l = 0.06; //longitud de la biela (m)
double PHI = PI/2; //angulo de desfasamiento (rad)
double rp = 0.015; //radio del piston (m) OJO LO MULTIPLIQUE X 2 PARA VER QUE PASA CON LA POTENCIA
double J = 0.004; //momento de inercia (Kg*m^2)
double Tgn = 0;
double Tgw = 0;
double Xg = 0;
double T11 = 0;
double T22 = 0;
double Vg = 1;
double teta = 0;
double Pg = 0;
double W = 0;
double Xe = 0;
double Xc = 0;
//Torques
double tf = 0;
double te = 0; //Torque de expansión
double tc = 0; //Torque de compresión
double tq = 0; //Carga
double tprom = 0;
//
double fi = 0;
double Fi_e = 0;
double Ce = 0;
double Cc = 0;
double Fi_c = 0;
double Alfa = 0;
double Vel = 0;
double ini = 2; //Valor inicial
//Variables termicas y termodinamicos
double Mg = 0.0002;
double Acilc = 0;
double Acile = 0;
double Tg1 = 0;
double Tg1ant = 0;
double Tg2 = 0;
double Tg2ant = 0;
double Tcld = 0;
double Tht = 0;
//Parametros frio
double Tcold = 300; //temperatura de entrada (K)
double F1 = 0.001; //flujo de chaqueta (kg/s)
double V1 = 0.001; //volumen de chaqueta (m^3)
//Parametros caliente
double dv = 14.71; //densidad (kg/m^3)
double cpv = 1873.3; //calor especifico (Kg*K)
double Thot = 600; //temperatura de entrada(K)
double V2 = 0.001; //volumen de chaqueta (m^3)
double F2 = 0.001; //flujo de chaqueta (kg/s)

//Parametros termodinamicos
double pcs = 0;
double TG = 1;
//Parametros integral
double IntIn1 = 1;
double IntOut1 = 1;
double Int2In1 = 1;
double Int2Out1 = 1;
double Int3In1 = 1;
double Int3Out1 = 1;
double Int4In1 = 1;
double Int4Out1 = 1;
double Int5In1 = 1;
double Int5Out1 = 1;
double Int6In1 = 1;
double Int6Out1 = 1;
double Int7In1 = 1;
double Int7Out1 = 1;

//Temperatura
double N = 45;//45
double ku = 15;
double rho = 7830;
double cp = 500;
double L = 0.04;
double alf= 0;
double dt = 0;
double hi = 0;
double t = 20;
double jmax = 0;
double TW1,TW2,TW3,TW4;

//Txt
FILE* fichero;
char text1[20],text2[20],text3[20],text4[20],text5[20],text6[20],text7[20],text8[20];

double integral1(double IntIn1, double IntOut1)
{
	double intg=1;
	intg=(0.0001*IntIn1)+(IntOut1);
	return intg;
}

double integral2(double Int2In1, double Int2Out1)
{
	double intg=1;
	intg=(0.0001*Int2In1)+(Int2Out1);
	return intg;
}

double integral3(double Int3In1, double Int3Out1)
{
	double intg=1;
	intg=(0.0001*Int3In1)+(Int3Out1);
	return intg;
}

double integral4(double Int4In1, double Int4Out1)
{
	double intg=1;
	intg=(0.0001*Int4In1)+(Int4Out1);
	return intg;
}

double integral5(double Int5In1, double Int5Out1)
{
	double intg=1;
	intg=(0.0001*Int5In1)+(Int5Out1);
	return intg;
}

double integral6(double Int6In1, double Int6Out1)
{
	double intg=1;
	intg=(0.0001*Int6In1)+(Int6Out1);
	return intg;
}

double integral7(double Int7In1, double Int7Out1)
{
	double intg=1;
	intg=(0.0001*Int7In1)+(Int7Out1);
	return intg;
}

double Temperatura(double Xg,double T1, double T2){
  //variables inicio
  double temp = 0;
  //int Ni = (int) N;
  double q = 0;
  double Ti = 300;
  double T0 = T2;
  double TL = T1;
  double ra = (ku*dt)/(rho*cp*(pow(hi,2)));

  //Variables selección Regnerador
  double a=0.0337;
  double b=0.040;
  double c=0.0482;
  double d=0.0555;
  double e=0.0639;

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
TW1= u[39][(int) jmax-1];
////printf("\n %5.4f",TW1);

////Salida2
TW2= u[29][(int) jmax-1];
////printf("\n %5.4f",TW2);

////Salida3
TW3= u[19][(int) jmax-1];
////printf("\n %5.4f",TW3);

////Salida4
TW4= u[0][(int) jmax-1];
////printf("\n %5.4f",TW4);

if (a<Xg && Xg<=b){
  temp=TW1;
}else if (b<Xg && Xg<=c){
  temp=TW2;
}else if (c<Xg && Xg<=d){
  temp=TW3;
}else if (d<Xg && Xg<=e){
  temp=TW4;
}
/*printf("\n Xg: %f",Xg);
printf("\n Temp1: %f",TW1);
printf("\n Temp2: %f",TW2);
printf("\n Temp3: %f",TW3);
printf("\n Temp4: %f",TW4);
printf("\n out: %f",temp);*/
return temp;
}

double tempGas(double Tgn, double Vg,double retro){
  double Tg1 = retro;//solo funciona si inicializo igualandola a la Temperatura de entrada (CORREGIR)
  double Sum = 0;
  double Div = 0;
  double Mult = 0;
  double Cte = (h*Awr)/(dg*cpg);
  Div = Cte/Vg;
  Sum = Tgn - Tg1;
  Mult = Sum * Div;
  Tg1 = integral3(Int3In1,Int3Out1);
  Int3In1 = Mult;
  Int3Out1 = Tg1;
  /*printf("\n Vg: %f",Vg);
  printf("\n Div: %f",Div);
  printf("\n Cte: %f",Cte);
  printf("\n Sum: %f",Sum);
  printf("\n Mult: %f",Mult);
  printf("\n Tgn: %f",Tgn);
  printf("\n Tg1: %f",Tg1);*/
  return Tg1;
}

int out(){
	if (cont<3000){cont +=1;}
	else {cont =cont;}
  //for (cont=0;cont<500;cont++){
    Tgn = Temperatura(Xg,T11,T22); //Temperatura actual del gas
    Tgw = tempGas(Tgn, Vg,retro);
    //Parte mecanica
    retro=Tgw;
    Fg =((Pg*1000)-(Patm*1000))*Ap;
    fi = (90*PI)/180;
    Fi_e = asin((r*sin(teta))/l);
    Ce = (r*cos(teta))+(l*cos(Fi_e));
    Fi_c = asin((r*sin(teta-fi))/l);
    Cc = (r*cos(teta-fi))+(l*cos(Fi_c));
    Xe = r+l-Ce; //Carrera de expansión
    Xc = r+l-Cc; //Carrera de compresión
    te = Fg*tan(Fi_e)*(l-((pow(r,2))/(4*l))+(r*(cos(teta))+(pow(r,2))/(4*l)*cos(2*teta)));
    tc = Fg*tan(Fi_c)*(l-((pow(r,2))/(4*l))+r*(cos(teta-fi)+(pow(r,2))/(4*l)*cos(2*teta-2*fi)));
    tf = W*b;
    Xg = ((r*4)+lwr-Xe-Xc)*0.5;

    //acondicionamiento par
    PteR[649]=te;
    PtcR[649]=tc;
    for(int i=0;i<650;i++){
        teR += PteR[i];
        tcR += PtcR[i];
    }
    teR = teR/650;
    tcR = tcR/650;

    if (cont>2){ini=0;} //Recordar añadirle una condición inicial
    //
    if (cont>2000){tq=te + tc;}
    Alfa =  (te + tc+ini-tf-tq)/J; //Sumarle ini
    Vel = integral1(IntIn1,IntOut1);
    IntIn1 = Alfa;
    IntOut1 = Vel;
    teta = integral2(Int2In1, Int2Out1);
    Int2In1 = Vel;
    Int2Out1 = teta;
    //Modelo termico
    Acilc = (rp*2*PI)*Xc;
    Tg1ant = ((Acilc*(h/(dg*cpg)))/Vg) * (T11-Tg1);
    Tg1 = integral4(Int4In1,Int4Out1);
    Int4In1 = Tg1ant;
    Int4Out1 = Tg1;
    Tcld = ((Tcold-T11)* F1/(V1*dv)-h*Acilc/(V1*dv*cpv)*(T11-Tg1));
    T11 += Tcld;
    Acile = (rp*2*PI)*Xe;
    Tg2ant = ((Acile*(h/(dg*cpg)))/Vg) * (T22-Tg2);
    Tg2 = integral6(Int6In1,Int6Out1);
    Int6In1 = Tg2ant;
    Int6Out1 = Tg2;
    Tht = (Thot-T22)* F2/(V2*dv)-h*Acile/(V2*dv*cpv)*(T22-Tg2);
    T22 += Tht;

    //Modelo termodinamico
    pcs = floor(4*((teta/(2*3.1426))-floor(teta/(2*3.1426))))+1;
    if (pcs == 1){TG=Tgw;}
    else if (pcs == 2){TG=T22;}
    else if (pcs == 3){TG=Tgw;}
    else if (pcs == 4){TG=T11;} //que pasa si no es ninguna?
    Pg = (Mg * Rg * TG) / Vg;
    Vg = (Xc + Xe + Xwr) * Ap;
    
    //acondicionamiento del torque mecanico
    for(int i=1;i<650;i++){
        PteR[i-1] = PteR[i];
        PtcR[i-1] = PtcR[i];
    }
    

    //Validacion
    //printf("\n Promedio: %f",Prom[6]);
    //printf("\n Tgn: %f",Tgn);
    //printf("\n Tgw: %f",Tgw);
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
    //printf("\n tq: %f",tq);
    //printf("\n tf: %f",tf);
    //printf("\n Xg: %f",Xg); //Funciona
    //printf("\n Alfa: %f",Alfa);
    //printf("\n cont: %d",cont);
    //printf("\n Vel: %f",Vel);
    //printf("\n teta: %f",teta);
    //printf("\n Acilc: %f",Acilc);
    //printf("\n Tg1ant: %f",Tg1ant);
    //printf("\n Tg1: %f",Tg1);
    //printf("\n Tcld: %f",Tcld);
    //printf("\n T11: %f",T11);
    //printf("\n Acile: %f",Acile);
    //printf("\n Tg2ant: %f",Tg2ant);
    //printf("\n Tg2: %f",Tg2);
    //printf("\n Tht: %f",Tht);
    //printf("\n T22: %f",T22);
    //printf("\n pcs: %f",pcs);
    //printf("\n Pg: %f",TG);
    //printf("\n Pg: %f",Pg);
    //printf("\n Vg: %f",Vg);
    printf("\n ------------------------");


    //Txt
    sprintf(text1,"%5.2f",Alfa);
	  strcat(text1,"\t");
	  sprintf(text2,"%5.2f",Vel);
	  strcat(text2,"\t");
	  sprintf(text3,"%5.2f",T11);
	  strcat(text3,"\t");
	  sprintf(text4,"%5.2f",T22);
	  strcat(text4,"\t");
	  sprintf(text5,"%5.2f",teta);
	  strcat(text5,"\t");
	  sprintf(text6,"%5.2f",tq);
	  strcat(text6,"\t");
	  sprintf(text7,"%f",Vg);
	  strcat(text7,"\t");
	  sprintf(text8,"%5.2f",teR);
	  strcat(text8,"\n");

    ////Guardar txt
	  fputs(text1,fichero);
	  fputs(text2,fichero);
	  fputs(text3,fichero);
	  fputs(text4,fichero);
	  fputs(text5,fichero);
	  fputs(text6,fichero);
	  fputs(text7,fichero);
	  fputs(text8,fichero);

//Muestra el vector
/*for(int i=0;i<N;i++){
  for(int j=0;j<jmax;j++){
    printf("%4.0f",u[i][j]);
  }
  printf("\n");
}*/
	return 0;

}

int main(void) {
	alf=ku/(cp*rho);
	hi = L/(N-1);
	dt = (0.5*(pow(hi,2)))/alf;
	jmax = round(t/dt);
	
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
	fichero =fopen("graficoB.txt","a");
  //for (int i = 0;i<=1000;i++)
	while(1)
	{
	out();
	//printf("\n Vg: %d",cont);
	}
	fclose(fichero);
	//}

	return 0;
}
