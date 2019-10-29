/*
    Redondear números en C con round
    @author parzibyte
    parzibyte.me/blog
*/
#include<stdio.h>
#include<math.h>
#include <stdlib.h>
#define PI 3.14159

int main(){
  //entradas
  double te = 1;
  double tc = 1;
  double ini = 2;
  double tf = 1;
  double tq = 1;
  double J = 0.004; //momento de inercia (Kg*m^2)
  double Alfa = 0;
  double Vel = 0;
  double teta = 0;

  //Operaciones
  for (int i=0;i<10;i++){
    if (i==2){ini=0;}
    Alfa =  (te+tc+ini-tf-tq)/J;
    Vel += Alfa;
    teta += Vel;
    printf("\n Alfa: %f", Alfa);
    printf("\n Vel: %f", Vel);
    printf("\n teta: %f", teta);
  }
}