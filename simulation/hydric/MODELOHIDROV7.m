

% domdelo matematico de generacion por turbina de rio %
% modelo polinomial de grado FIVE%
% CONSTANTES DEL POLINOMIO %

%k=1;
%z=2;
%m=5;

% modelo 2%

den=1000; % densidad kg/m^3%
% cp=0.59; %coeficiente de potenci%
D=1; %diametro de la turbina %
A=pi()*((D/2)^2);
%FACTOR=1/(2*pi())*60;
r=D/2;
B=0;
T_d=0;
J= 16.1919e-4; %inecia maquina(afecta el tiempo de estabilizacion)

% calculo de landa 
%parametros
%grados=0;
beta = 0;

% % Calculo motor dc - no
Ri=0.973;
Ji=1.72;
Kp=5.6627;
Kb=6.2;       %contra electromotriz (V*s)
Bi=0.1;
% Calculo motor dc - funcionan
% Ri=1;
% Ji=0.0465;
% Kp=0.027;
% Kb=0.027;         %contra electromotriz
% Bi=0.005;

%%parametro del generador electrico 800w%%
Rs=1.649;             % (ohm) Resistencia de armadura 
L_q = 9.03e-3;        % Inductancias 
L_d = 38e-3;          %
Lambda = 0.48;        % (T) Flujo magnnetico
P = 1;                %  Numero de pares de polos 
Rl= 40;               % Carga resistiva
Xl=0;                 % Carga reactiva

Rr=2;

%conversor dc-dc
Lcc=2.109e-3;
Ccc=83.3e-6;
Dcc=0.625; %0.625 es el calculado y el espacio estado funciona bien, el otro es tanteo y el implementados funciona mejor
Rcc=225; %total 225

% Lcc=70.3e-6;
% Ccc=2.5e-3;
% Dcc=0.625; %0.625 es el calculado y el espacio estado funciona bien, el otro es tanteo y el implementados funciona mejor
% Rcc=7.5; %7.5
Ac=[0 (-1/Lcc);(1/Ccc) -(1/(Rcc*Ccc))];
Bc=[(Dcc/Lcc);0];
Cc=[0 1];
Dc=0;

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
Linv  = 25e-3; %80e-3
Cinv  = 10e-6; %22e-6

Rr = 5 ;
Lr = 0;
% Lr = 50e-3;

% Parametros de los controladores
K2Ic = 5;
K1Ic = 100;
K2Vo = 0.5;
K1Vo = 50;