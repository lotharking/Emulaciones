%PARAMETROS PARA MODELO DE REGENERADOR%
h=1000; % coeficiente global de transferencia de calor (W/m^2*K)
kwr=15; %condutividad termica de las rejillas (W/m*k)
cpwr=500; %calor especifico de las rejillas (J/Kg*k)
Awr=0.0058; %area de transferencia de calor de la rejilla%
dwr=7830; %densidad de rejilla (kg/m^3)
dg=1.23; %densidad del gas de trabajo(kg/m^3)
cpg=1006; %calor especifico del gas de trabajo (J/Kg*k)
lwr = 0.06; %Longitud del regenerador en metros. 
Xwr = 0.04; % Longitud del Regenerador


 %PARAMETROS PARA EL MODELO MECANICO%
 r=0.02;% radio del cigueñal (m)%
 l=0.06; % longitud de la biela (m)%
 PHI=pi/2; % angulo de desfasamiento (rad)%
 rp=0.015; %radio del piston (m) OJO LO MULTIPLIQUE X 2 PARA VER QUE PASA CON LA POTENCIA
 Ap=0.0007068; % area del piston (m^2)
 Patm=101.325; %presion atmosferica (kpa)%
 J=0.004; %momento de inercia (Kg*m^2)%
 b=0.0000000001; % coeficiente de friccion% OJO CAMBIO 
 c=0.012; %coeficiente de carga% OJO CAMBIO
 

% PARAMETROS PARA MODELO TERMICO%

% fluido de enfriamiento%
F1=0.001; %flujo de chaqueta (kg/s)%
V1=0.001; % volumen de chaqueta (m^3)%
Tcold=300; % temperatura de entrada (K)%
dw=1000; %densidad (kg/m^3)%
cpw=4227; % calor especifico(Kg*K)

% fluido de calentamiento%
F2=0.001; %flujo de chaqueta (kg/s)%
V2=0.001; % volumen de chaqueta (m^3)%
Thot=600; % temperatura de entrada(K)
dv=14.71; %densidad (kg/m^3)
cpv=1873.3; %calor especifico (Kg*K)%


%PARAMETROS PARA EL MODELO TERMODINAMICO%
Xreg=0.04; %longitud del regenerador%
Rg=0.287; %constante de gas ideal(K/Kg)%
Ap = 0.0007068; 

fve=60/(2*pi());

%%parametro del generador electrico 800w%%
Rs=1.649;             % (ohm) Resistencia de armadura 
L_q = 9.03e-3;        % Inductancias 
L_d = 38e-3;          %
Lambda = 0.48;        % (T) Flujo magnnetico
P = 1;                %  Numero de pares de polos 
Rl= 1;               % Carga resistiva
Xl=0;                 % Carga reactiva

% Calculo motor dc
Ri=0.973;
Ji=1.72;
Kp=5.6627;
Kb=6.2;       %contra electromotriz (V*s)
Bi=0.1;

%CARGA
CargaR = 1000;