/*
    Redondear números en C con round
    @author parzibyte
    parzibyte.me/blog
*/
#include<stdio.h>
#include<math.h>
#include <stdlib.h>

int main(){
  int siMemoria = 1;
  double T1=200;
  double T2=400;
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
  //if (r>0.5){printf("disminuya el intervalo de tiempo dt");}
  double jmax = round(t/dt);
  double **u=NULL;
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
printf("\n %5.4f",TW1);

//Salida2
TW2= u[29][(int) jmax-1];
printf("\n %5.4f",TW2);

//Salida3
TW3= u[19][(int) jmax-1];
printf("\n %5.4f",TW3);

//Salida4
TW4= u[9][(int) jmax-1];//preguntar a kevin
printf("\n %5.4f",TW4);

//Muestra el vector
/*for(int i=0;i<N;i++){
  for(int j=0;j<jmax;j++){
    printf("%4.0f",u[i][j]);
  }
  printf("\n");
}*/

}