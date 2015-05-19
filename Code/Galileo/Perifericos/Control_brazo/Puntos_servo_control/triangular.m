function X = triangular(ti,tf,x0i,x0f,x1i)

%Vector de tiempo
deltat=(tf-ti)/100;
t=ti:deltat:tf;

%Velocidad máxima y aceleración
vmax=2*(x0f-x0i)/(tf-ti)
a=2*vmax/(tf-ti)

%Vectores de movimiento: xn=n-esima derivada
x0=[];
x1=[];
x2=[];

for i=1:length(t)
	if t(i)<((tf-ti)/2)
		x2(i)=a;
		x1(i)=a*(t(i)-ti)+x1i;
		x0(i)=a*((t(i))^2)/2 + (x1i-a*ti)*t(i) + a*(ti^2)/2 - x1i*ti + x0i;
		tmid=t(i);
		xmid=x0(i);
	else
		x2(i)=-a;
		x1(i)=-a*(t(i)-tmid)+vmax;
		x0(i)=-a*((t(i))^2)/2 + (vmax+a*tmid)*t(i) - a*(tmid^2)/2 - vmax*tmid + xmid;
	end
end



X=[t;x0;x1;x2];
