#include<stdio.h>
#include<math.h>
#include <stdlib.h>

int main(){
  double a=0.0337;
  double b=0.040;
  double c=0.0482;
  double d=0.0555;
  double e=0.0639;
  //Variables de entrada
  double Xg=0;
  double TW1 = 5; //Cambiar por las entradas
  double TW2 = 10;
  double TW3 = 15;
  double TW4 = 20;
  //Salida
  double Tgw=0;
  //Metodo
  if (a<Xg && Xg<=b){
    Tgw=TW1;
  }else if (b<Xg && Xg<=c){
    Tgw=TW2;
  }else if (c<Xg && Xg<=d){
    Tgw=TW3;
  }else if (d<Xg && Xg<=e){
    Tgw=TW4;
  }
  printf("%f",Tgw);
}