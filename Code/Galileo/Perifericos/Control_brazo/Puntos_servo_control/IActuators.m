function ADC=IActuators(q2,q3,q4)
AB=0.086798938;
BC=0.310166928260983;
DE=0.00644639756491228;
EB=0.305247157105962;
FM=0;
FC=0.32;
CK=0.112519298309976;
CN=0.18885;

DB=sqrt(DE^2+EB^2);
NO=0.283;
CM=sqrt(FM^2+FC^2);

l1=EB+0.08;
l2=BC;
l3=CN;
l4=NO;
m2=1.266; %Masas de los centros de masa [kg]
m3=1;
m4=3.663;
psi2=17.51;
psi3=6.54;
psi4=3.5983;
rCm2=0.121892; %Ubicacion de los centros de masa
rCm3=0.0357;
rCm4=0.127866;
g=9.81; %gravedad [m^2/s]
hmin= 0.75; %altura mínima  la que se queire llegar con la muñeca del brazo desde el chasis
Lbag=0.35; %distancia desde el centro del brazo hasta el punto de comienzo del almacenamiento (es decir, bag)
f=-0.38; %Distancia del piso a la parte superior del chasis

%%%% Actuadores

dmin1=0.245; %distancia de extremo a extremo del actuador 1 cerrado
dmax1=dmin1+0.13; %Recorrido máximo actuador 1
Fmax1=750; % [N] Fuerza máxima que puede desarrollar el actuador 1.

dmin2=10.3*0.0254; %distancia de extremo a extremo del actuador 2 cerrado 0.26162
dmax2=dmin2+6*0.0254; %Recorrido máximo actuador 2 0.1524 MAX= 0.41402     373.875
Fmax2=400; % [N] Fuerza máxima que puede desarrollar el actuador 2

ang1=atan2d(DE,EB);
ang2=atan2d(FM,FC);
theta=180-ang2-acosd((CK^2+CM^2-dmin2^2)/(2*CK*CM));

alpha=90+q2-ang1;
d1=sqrt(DB^2+AB^2-(2*DB*AB*cosd(alpha)));
senobeta=(DB*sind(alpha))/d1;


gamma=180-theta-ang2+q3;
d2=sqrt(CM^2+CK^2-(2*CM*CK*cosd(gamma)));
senodelta=(CM*sind(gamma))/d2;

M2=-m2*g*(rCm2*cosd(q2+psi2));
M3=-m3*g*(l2*cosd(q2)+rCm3*cosd(q2+q3+psi3));
M4=-m4*g*(l2*cosd(q2)+l3*cosd(q2+q3)+rCm4*cosd(q2+q3+q4+psi4));

TA1=-(M2+M3+M4);
TA2=-(M3+M4);

X=l2*cosd(q2)+l3*cosd(q2+q3)+l4*cosd(q2+q3+q4)
Y=l1+l2*sind(q2)+l3*sind(q2+q3)+l4*sind(q2+q3+q4)

FA1=TA1/(senobeta*AB)
FA2=TA2/(senodelta*CK);

I(1)=0.00514512*FA1+0.7
%I(2)=1*FA2;

ADC=(I+0.071626777)./0.0470096018;



end

