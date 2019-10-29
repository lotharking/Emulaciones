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
  double tetha = 1;
  double Mg = 1;
  double Xc = 1;
  double Xe = 1;
  //Variables
  double Acilc = 0;
  double Acile = 0;
  double h = 1000; //coeficiente global de transferencia de calor (W/m^2*K)
  double dg = 1.23; //densidad del gas de trabajo(kg/m^3)
  double cpg = 1006; //calor especifico del gas de trabajo (J/Kg*k)
  double Tg1 = 0;
  double Tg1ant = 0;
  double Tg2 = 0;
  double Tg2ant = 0;
  double Vg = 1;
  double rp=0.015; //radio del piston (m) OJO LO MULTIPLIQUE X 2 PARA VER QUE PASA CON LA POTENCIA
  double T11 = 0;
  double Tcld = 0;
  double T22 = 0;
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
  double Tgw = 1;
  double Rg = 0.287; //constante de gas ideal(K/Kg)
  double Pg = 0;
  double Xwr = 0.04; //Longitud del Regenerador
  double Ap = 0.0007068; //area del piston (m^2)

  //operaciones Termico
  for (int i=0;i<10;i++){
  Acilc = (rp*2*PI)*Xc;
  Tg1ant = ((Acilc*(h/(dg*cpg)))/Vg) * (T11-Tg1);
  Tg1 += Tg1ant;
  Tcld = ((Tcold-T11)* F1/(V1*dv)-h*Acilc/(V1*dv*cpv)*(T11-Tg1));
  T11 += Tcld;
  Acile = (rp*2*PI)*Xe;
  Tg2ant = ((Acile*(h/(dg*cpg)))/Vg) * (T22-Tg2);
  Tg2 += Tg2ant;
  Tht = (Thot-T22)* F2/(V2*dv)-h*Acile/(V2*dv*cpv)*(T22-Tg2);
  T22 += Tht;

  //Operaciones Termodinamico
  pcs = floor(4*((tetha/(2*3.1426))-floor(tetha/(2*3.1426))))+1;
  if (pcs == 1){TG=Tgw;}
  else if (pcs == 2){TG=T22;}
  else if (pcs == 3){TG=Tgw;}
  else if (pcs == 4){TG=T11;} //que pasa si no es ninguna?
  Pg = (Mg * Rg * TG) / Vg;
  Vg = (Xc + Xe + Xwr) * Ap;

  //Visualización
  printf("\n Tg1: %f",Tg1);
  printf("\n T11: %f",T11);
  printf("\n Tg2: %f",Tg2);
  printf("\n T22: %f",T22);
  }
}