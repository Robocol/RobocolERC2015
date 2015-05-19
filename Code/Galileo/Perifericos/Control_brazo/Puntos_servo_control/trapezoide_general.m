function [p,X] = trapezoide_general(p2,p3)

%Vector de tiempo
deltat=1/100;
t=0:deltat:1;

%Velocidad máxima y aceleraciones
ca1=2/((p3-p2+1)*p2)
ca3=2/((p3-p2+1)*(p3-1))
cv=2*p2/((p3-p2+1)*p2)


%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];

for i=1:length(t)
	if t(i)<p2
		x2(i)=ca1;
		x1(i)=ca1*t(i);
		x0(i)=ca1*(t(i)^2)/2;
	elseif t(i)<p3
		x2(i)=0;
		x1(i)=cv;
		x0(i)=ca1*(p2^2)/2 + ca1*p2*(t(i)-p2);
	else
		x2(i)=ca3;
		x1(i)=ca1*p2 + ca3*(t(i)-p3);
		x0(i)=ca1*(p2^2)/2 + ca1*p2*(p3-p2) + ca1*p2*(t(i)-p3) + ca3*(t(i)-p3)^2/2;
	end
end
p=[cv,ca1,ca3]
X=[t;x0;x1;x2];
