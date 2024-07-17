%%
Ventrada = 150;
ConvInSup1 = 0;
ConvInSup2 = 0;
ConvOutSup1 = 0;
ConvOutSup2 = 0;
Iout = 0;
ConvInInf1 = 0;
ConvInInf2 = 0;
ConvOutInf1 = 0;
ConvOutInf2 = 0;

while(1)
DcVoSup=(0.02833*ConvInSup1)+(0.02833*ConvInSup2)+(1.943*ConvOutSup1)-(ConvOutSup2);
ConvOutSup2 = ConvOutSup1;
ConvInSup2=ConvInSup1;
ConvOutSup1=DcVoSup;
ConvInSup1=Ventrada;

DcVoInf=(-1.189*ConvInInf1)+(1.189*ConvInInf2)+(1.943*ConvOutInf1)-(ConvOutInf2);
ConvOutInf2 = ConvOutInf1;
ConvInInf2=ConvInInf1;
ConvOutInf1=DcVoInf;
ConvInInf1=Iout;

DcVo = DcVoSup + DcVoInf
Iout = DcVo / 225;
end