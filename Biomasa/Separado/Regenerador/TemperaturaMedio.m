/*
    Redondear números en C con round
    @author parzibyte
    parzibyte.me/blog
*/
#include<stdio.h>
#include<math.h>
#include <stdlib.h>

int main(){
  
  double Vg = 2;
  double Cte = 5;
  double TW1 = 3;
  double Tg1 = TW1;//solo funciona si inicializo asi
  double Sum = 0;
  double Div = 0;
  double Mult = 0;
  double h =1000; //coeficiente global de transferencia de calor (W/m^2*K)
  double Awr = 0.0058; //area de transferencia de calor de la rejilla
  double dg = 1.23; //densidad del gas de trabajo(kg/m^3)
  double cpg = 1006; //calor especifico del gas de trabajo (J/Kg*k)

  Cte = (h*Awr)/(dg*cpg);
  //while(1){
  for (int i=0;i<10;i++){
    Div = Cte/Vg;
    Sum = TW1 - Tg1;
    Mult = Sum * Div;
    Tg1 = Tg1 + Mult;
    printf("\n %5.4f",Tg1);
  }
}