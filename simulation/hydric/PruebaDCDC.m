% Lcc=70.3e-6;
% Ccc=2.5e-3;
% Dcc=0.625;

% Lcc=127.5e-6;
% Ccc=2.5e-3;
% Dcc=0.32;
% Rcc=7.5;

Lcc=2.109e-3;
Ccc=83.3e-6;
Dcc=0.625;
Rcc=225; 

A=[0 (-1/Lcc);(1/Ccc) -(1/(Rcc*Ccc))];
B=[(1/Lcc);0];
C=[1 0;0 1];
D=0;
sys_mimo = ss(A,B,C,D);
tf(sys_mimo)

% A=[-(1/(Rcc*Ccc)) (-1/Lcc);(1/Ccc) 0];
% B=[0;(Dcc/Lcc)];
% C=[1 0];
% D=0;