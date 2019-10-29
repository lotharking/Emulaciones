#include<stdio.h>
#include<math.h>
#include <stdlib.h>

//Variables globales
double Tgn = 0;
double Tgw = 0;
double Xg = 0;
double T11 = 0;
double T22 = 0;
double Vg = 0;



double Temperatura(double Xg,double T1, double T2){
  //variables inicio
  double temp = 0;
  int siMemoria = 1;
  double N = 45;//45
  int Ni = (int) N;
  double ku = 15;
  double rho = 7830;
  double cp = 500;
  double L = 0.04;
  double q = 0;
  double h = L/(N-1);
  double alfa=ku/(cp*rho);
  double dt = (0.5*(pow(h,2)))/alfa;
  double t = 20;
  double Ti = 300;
  double T0 = T2;
  double TL = T1;
  double r = (ku*dt)/(rho*cp*(pow(h,2)));
  double TW1,TW2,TW3,TW4;
  double jmax = round(t/dt);
  double **u=NULL;

  //Variables selección
  double a=0.0337;
  double b=0.040;
  double c=0.0482;
  double d=0.0555;
  double e=0.0639;

  if (r>0.5){printf("disminuya el intervalo de tiempo dt");}
  //Creacion de matrix
  u = (double**) calloc(N,sizeof(double*));
  if (u==NULL){
  //mensaje de error
}else{
  for(int i=0;i<N;i++){
    u[i]=(double*) calloc(jmax,sizeof(double*));
    if (u[i]==NULL){
      //mensaje de error
      siMemoria=0;
    }
  }
}
//hace el u(:,1)
if(siMemoria==1){
for(int i=0;i<N;i++){
  for(int j=0;j<1;j++){
    u[i][j]=Ti;
  }
}
}
//hace el u(1,:)
if(siMemoria==1){
for(int i=0;i<1;i++){
  for(int j=0;j<jmax;j++){
    u[i][j]=T0;
  }
}
}

//hace el u(N,:)
if(siMemoria==1){
  for(int j=0;j<jmax;j++){
    u[Ni-1][j]=TL;
  }
}

for(int j=0;j<jmax-1;j++){
  for(int i=1;i<N-1;i++){
    u[i][j+1]= ((r*(u[i-1][j]))+(1-(2*r))*(u[i][j])+(r*(u[i+1][j]))) + (q*dt);
  }
}
//Salidas del ciclo
//Salida1
TW1= u[39][(int) jmax-1];
//printf("\n %5.4f",TW1);

//Salida2
TW2= u[29][(int) jmax-1];
//printf("\n %5.4f",TW2);

//Salida3
TW3= u[19][(int) jmax-1];
//printf("\n %5.4f",TW3);

//Salida4
TW4= u[9][(int) jmax-1];//preguntar a kevin
//printf("\n %5.4f",TW4);

if (a<Xg && Xg<=b){
    temp=TW1;
  }else if (b<Xg && Xg<=c){
    temp=TW2;
  }else if (c<Xg && Xg<=d){
    temp=TW3;
  }else if (d<Xg && Xg<=e){
    temp=TW4;
  }
return temp;
}

double tempGas(double Tgn, double Vg){
  double h =1000; //coeficiente global de transferencia de calor (W/m^2*K)
  double Awr = 0.0058; //area de transferencia de calor de la rejilla
  double dg = 1.23; //densidad del gas de trabajo(kg/m^3)
  double cpg = 1006; //calor especifico del gas de trabajo (J/Kg*k)
  double Tg1 = 0;//solo funciona si inicializo igualandola a la Temperatura de entrada
  double Sum = 0;
  double Div = 0;
  double Mult = 0;
  double Cte = (h*Awr)/(dg*cpg);
  Div = Cte/Vg;
  Sum = Tgn - Tg1;
  Mult = Sum * Div;
  Tg1 += Mult;
  return Tg1;
}


int main(){
  //Variables medio

  //Temperatura actual del gas
  Tgn = Temperatura(Xg,T11,T22);
  Tgw = tempGas(Tgn, Vg);
  



//Muestra el vector
/*for(int i=0;i<N;i++){
  for(int j=0;j<jmax;j++){
    printf("%4.0f",u[i][j]);
  }
  printf("\n");
}*/

}