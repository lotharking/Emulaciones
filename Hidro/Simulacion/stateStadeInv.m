L=23.45e-3;
Cc=3e-6;
rl=1;
D=0.625; 
R=40;
w=2*pi*60;
% L  = 25e-3; %80e-3
% Cc  = 10e-6; %22e-6
% Vdc=150;
% A=[(-rl/L) 0 0 (-1/L) 0 0;0 (-rl/L) 0 0 (-1/L) 0;0 0 (-rl/L) 0  0 (-1/L);(1/Cc) 0 0 (-1/(R*Cc)) 0 0;0 (1/Cc) 0 0 (-1/(R*Cc)) 0;0 0 (1/Cc) 0 0 (-1/(R*Cc))];
% B=[(2/(3*L));(-1/(3*L));(-1/(3*L));0;0;0];
% C=[0 0 0 1 0 0;0 0 0 0 1 0;0 0 0 0 0 1];
% D=[0:0:0];

A=[(-rl/L) 0 0 (-1/L) 0 0;0 (-rl/L) 0 0 (-1/L) 0;0 0 (-rl/L) 0  0 (-1/L);(1/Cc) 0 0 0 0 0;0 (1/Cc) 0 0 0 0;0 0 (1/Cc) 0 0 0];
B=[1/L 0 0;0 1/L 0;0 0 1/L;0 0 0;0 0 0;0 0 0];
C=[0 0 0 1 0 0;0 0 0 0 1 0;0 0 0 0 0 1];
D=[0 0 0;0 0 0;0 0 0];
% states = {'beta' 'yaw' 'roll' 'phi'};
% inputs = {'rudder' 'aileron'};
% outputs = {'yaw rate' 'bank angle'};

sys_mimo = ss(A,B,C,D);
tf(sys_mimo)

%%filtro de salida
% A=[0 -(1/L);(1/C) -(1/(C*R))];
% B=[(1/L);0];
% C=[0 1];
% D=0;


K2Ic = 5;
K1Ic = 100;
K2Vo = 0.5;
K1Vo = 50;


%*****
%Variables Comunes
%*****
Fr = 60;
Fc = 5000;
Tc = 1/Fc;