% modelo matematico de generacion por turbina de rio %
den=1000; % densidad kg/m^3%
% cp=0.59; %coeficiente de potenci%
D=1; %diametro de la turbina %
A=pi()*((D/2)^2);
r=D/2;
B=0;
T_d=0;
J= 16.1919e-4; 
beta = 0;

% % Calculo motor dc - no
Ri=0.973;
Ji=1.72;
Kp=5.6627;
Kb=6.2;       %contra electromotriz (V*s)
Bi=0.1;


%conversor dc-dc
Lcc=2.109e-3;
Ccc=83.3e-6;
Dcc=0.625;
Rcc=225; 

%*****
%Variables Comunes
%*****
Fr = 60;
Fc = 5000;
Tc = 1/Fc;
Ts =Tc/100;
Vg = 150;
D = 1;

%*****
%Variables Inversor de Voltaje
%*****

% Parametros de la carga y de la planta
Rinv  = 1;
Linv  = 23.45e-3; 
Cinv  = 3e-6; 


Rr = 40 ;
Lr = 0;

% Parametros de los controladores
K2Ic = 5;
K1Ic = 100;
K2Vo = 0.5;
K1Vo = 50;