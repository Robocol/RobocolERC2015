function [p,X] = armonico(ti,tf)

%Vector de tiempo
deltat=1/100;
t=ti:deltat:tf;

%Velocidad máxima y aceleraciones
pj=1/2*(pi^3)
ca=1/2*(pi^2)
cv=1/2*pi


%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];

for i=1:length(t)
	x3(i)=-1/2*(pi^3)*sin(pi*t(i));		
	x2(i)=1/2*(pi^2)*cos(pi*t(i));
	x1(i)=1/2*(pi)*sin(pi*t(i));
	x0(i)=1/2*(1-cos(pi*t(i)));
end
p=[cv,ca,pj];
X=[t;x0;x1;x2;x3];
