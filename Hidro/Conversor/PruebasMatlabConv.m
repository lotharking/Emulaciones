%%
Ventrada = 2;
ConvInSup1 = 1;
ConvInSup2 = 1;
ConvOutSup1 = 1;
ConvOutSup2 = 1;

while(1)
DcVoSup=(0.0001423*Ventrada)+(0.0002846*ConvInSup1)+(0.0001423*ConvInSup2)+(1.999*ConvOutSup1)-(ConvOutSup2)
ConvOutSup2 = ConvOutSup1;
ConvInSup2=ConvInSup1;
ConvOutSup1=DcVoSup;
ConvInSup1=Ventrada;
end

%%
Vent = 1;
ConvInInf1 = 1;
ConvInInf2 = 1;
ConvOutInf1 = 1;
ConvOutInf2 = 1;

while(1)
DcVoInf=(-0.06002*Vent)+(0.06002*ConvInInf2)+(1.999*ConvOutInf1)-(ConvOutInf2)
ConvOutInf2 = ConvOutInf1;
ConvInInf2=ConvInInf1;
ConvOutInf1=DcVoInf;
ConvInInf1=Vent;
end