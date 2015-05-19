function [p,X] = curva_s(s)

%Vector de tiempo
deltat=1/100;
t=0:deltat:1;

%Velocidad máxima y aceleraciones
cj=-8/(s*(s-2))
ca=-4/(s-2)
cv=2


%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];

for i=1:length(t)
	if t(i)< (s/2)
		x3(i)=cj;		
		x2(i)=cj*t(i);
		x1(i)=cj*(t(i)^2)/2;
		x0(i)=cj*(t(i)^3)/6;
	elseif t(i)< (1-s/2)
		x3(i)=0;		
		x2(i)=ca;
		x1(i)=cj*(s^2)/8 + ca*(t(i)-s/2);
		x0(i)=ca*((s^2)/8+(t(i)^2)/2-t(i)*s/2) + cj*((s^2)*t(i)/8-(s^3)/24);
	else
		x3(i)=-cj;		
		x2(i)=ca-cj*(t(i)-1+s/2);
		x1(i)=cj*(-(s^2)/4 + s/2 - (t(i)^2)/2 + t(i) - 1/2);
		x0(i)=cj*(-(s^2)*t(i)/4 - (t(i)^3)/6 + (t(i)^2)/2 - t(i)/2 + t(i)*s/2 + 1/6 - s/4 + (s^2)/8);
	end
end
p=[cv,ca,cj]
X=[t;x0;x1;x2;x3];
