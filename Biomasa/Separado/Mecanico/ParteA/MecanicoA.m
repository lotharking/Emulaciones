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
  double teta = 1;
  double Pg = 1;
  double W = 1;
  //Salida
  double Xg = 0;
  //variables
  double Patm = 101325; //presion atmosferica (pa)
  double Ap = 0.0007068; //area del piston (m^2)
  double Fg = 0;
  double r = 0.02; //radio del cigueñal (m)
  double l = 0.06; //longitud de la biela (m)
  double PHI = PI/2; //angulo de desfasamiento (rad)
  double rp = 0.015; //radio del piston (m) OJO LO MULTIPLIQUE X 2 PARA VER QUE PASA CON LA POTENCIA
  double fi = 0;
  double Fi_e = 0;
  double Ce = 0;
  double Cc = 0;
  double Fi_c = 0;
  double Xe = 0;
  double Xc = 0;
  double te = 0; //Torque de expansión
  double tc = 0; //Torque de compresión
  double b = 0.0000000001; //Coeficiente de fricción
  double c = 0.012; //coeficiente de carga !!OJO CAMBIO!!
  double tf = 0;
  double lwr = 0.06; //Longitud del regenerador en metros.

  // Operaciones
  Fg =((Pg*1000)-(Patm*1000))*Ap;
  fi = (90*PI)/180;
  Fi_e = asin((r*sin(teta))/l);
  Ce = (r*cos(teta))+(l*cos(Fi_e));
  Fi_c = asin((r*sin(teta-fi))/l);
  Cc = (r*cos(teta-fi))+(l*cos(Fi_c));
  Xe = r+l-Ce; //Carrera de expansión
  Xc = r+l-Cc; //Carrera de compresión
  te = Fg*tan(Fi_e)*(l-((pow(r,2))/(4*l))+(r*cos(teta))+(pow(r,2))/(4*l)*cos(2*teta));
  tc = Fg*tan(Fi_c)*(l-((pow(r,2))/(4*l))+r*cos(teta-fi)+(pow(r,2))/(4*l)*cos(2*teta-2*fi));
  tf = W*b;
  Xg = ((r*4)+lwr-Xe-Xc)*0.5;
  //validación
  printf("\n Xg: %f",Xg);
  printf("\n te: %f",te);
  printf("\n tc: %f",tc);
  printf("\n tf: %f",tf);
}