function [p,X] = cicloide(ti,tf)

%Vector de tiempo
deltat=1/100;
t=ti:deltat:tf;

%Velocidad máxima y aceleraciones
cj=4*(pi^2)
ca=2*(pi)
cv=2


%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];

for i=1:length(t)
	x3(i)=4*(pi^2)*cos(2*pi*t(i));		
	x2(i)=2*(pi)*sin(2*pi*t(i));
	x1(i)=1-cos(2*pi*t(i));
	x0(i)=t(i)-sin(2*pi*t(i))/(2*pi);
end
p=[cv,ca,cj];
X=[t;x0;x1;x2;x3];
