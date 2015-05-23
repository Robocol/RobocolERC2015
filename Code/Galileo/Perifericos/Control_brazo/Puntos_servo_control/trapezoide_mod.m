function [p,X] = trapezoide_mod()

%Vector de tiempo
deltat=1/100;
t=0:deltat:1;

%Velocidad máxima, posiciones de cambio, aceleración máxima y jerk máximo
cj=32*(pi^2)/(2+pi)
ca=8*pi/(2+pi)
cv=2
d=1/(2+pi)

%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];
x3=[];

%Primer cicloide
[p1,X1]=cicloide(0,0.25);
X1(1,:) = X1(1,:)*0.5;
X1(5,:) = X1(5,:)*d/(0.5^3);
X1(4,:) = X1(4,:)*d/(0.5^2);
X1(3,:) = X1(3,:)*d/(0.5);
X1(2,:) = X1(2,:)*d;


%Primer segmento de aceleración constante
X2(1,:) = 0.125:0.01:0.375;
X2(5,:) = 0;
X2(4,:) = ca;
X2(3,:) = 8*pi*d*(X2(1,:)-0.125)+2*d;
X2(2,:) = 4*pi*d*((X2(1,:)-0.125).^2)+2*d*(X2(1,:)-0.125)+(1/4-1/(2*pi))*d;

%Segundo cicloide
[p3,X3]=cicloide(0.25,0.75);
X3(1,:) = 0.5*(X3(1,:)-0.25)+0.375;
X3(5,:) = X3(5,:)*d/(0.5^3);
X3(4,:) = X3(4,:)*d/(0.5^2);
X3(3,:) = X3(3,:)*d/(0.5)-2*d+(2*pi+2)*d;
X3(2,:) = X3(2,:)*d+(1/4*pi+3/4-1/(2*pi))*d+(-2*d+(2*pi+2)*d)*(X3(1,:)-0.375)-(1/4-1/(2*pi))*d;

%segundo segmento de aceleración constante
X4(1,:) = 0.625:0.01:0.875;
X4(5,:) = 0;
X4(4,:) = -ca;
X4(3,:) = -8*pi*d*(X4(1,:)-0.625)+(2*pi+2)*d;
X4(2,:) = -4*pi*d*((X4(1,:)-0.625).^2)+(2*pi+2)*d*(X4(1,:)-0.625)+(3*pi/4+5/4-1/(2*pi))*d+d/pi;

%Tercer cicloide
[p3,X5]=cicloide(0.75,1);
X5(1,:) = 0.5*(X5(1,:)-0.75)+0.875;
X5(5,:) = X5(5,:)*d/(0.5^3);
X5(4,:) = X5(4,:)*d/(0.5^2);
X5(3,:) = X5(3,:)*d/(0.5);
X5(2,:) = X5(2,:)*d+(5/4*pi+7/4-1/(2*pi))*d+(-pi/4+1/pi)*d-(3/4+1/(2*pi))*d;

p=[d,cv,ca,cj]
X=[X1,X2,X3,X4,X5];
