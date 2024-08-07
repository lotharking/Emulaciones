

% domdelo matematico de generacion por turbina de rio %
% modelo polinomial de grado FIVE%
% CONSTANTES DEL POLINOMIO %

a=790.51;
b=6900.50;
c=23794;
d=39268;
e=31863;
f=10049;

k=1;
z=2;
m=5;


% modelo 2%

den=1000; % densidad kg/m^3%
cp=0.59; %coeficiente de potenci%
D=1; %diametro de la turbina %
A=pi()*((D/2)^2);
FACTOR=1/(2*pi())*60;
r=D/2;
B=0;
T_d=0;
J= 100; %inecia maquina(afecta el tiempo de estabilizacion)

% calculo de landa 
%parametros
%grados=0;
beta = 0;

% Calculo motor dc
Ri=0.6;
Ji=1;
Kp=0.4;
Kb=1.8;
Bi=0.2287;


%%parametro del generador electrico 800w%%
Rs=1.649;             % (ohm) Resistencia de armadura 
L_q = 9.03e-3;        % Inductancias 
L_d = 38e-3;          %
Lambda = 0.48;        % (T) Flujo magnnetico
P = 1;                %  Numero de pares de polos 
Rl= 40;               % Carga resistiva
Xl=0;                 % Carga reactiva

