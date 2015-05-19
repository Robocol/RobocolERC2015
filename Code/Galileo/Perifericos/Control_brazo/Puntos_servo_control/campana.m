function [X1,X2,X3] = campana(p2,p3,s1,s3,ti,tf,x0i,x0f,vi)

%Primer segmento
[p1,X1] = curva_s(s1);
tf1=(((tf-ti)*p2)+ti);
cy2=-p2/(p2-p3-1);
x0f1=cy2*(x0f-x0i)+x0i;
X1=escalar2(X1,ti,tf1,x0i,x0f1,vi);

%Segundo segmento
ti2=X1(1,length(X1(1,:)));
tf2=(((tf-ti)*p3)+ti);
x0i2=X1(2,length(X1(2,:)));
cy3=(p2-2*p3)/(p2-p3-1);
x0f1=cy3*(x0f-x0i)+x0i;
x1i2=X1(3,length(X1(3,:)));

t2=(0:0.01:1)*(tf2-ti2)+ti2;

for i=1:length(t2)
	x32(i)=0;	
	x22(i)=0;
	x12(i)=x1i2;
	x02(i)=x1i2*(t2(i)-ti2)+x0i2;
end

X2=[t2;x02;x12;x22;x32];

%Tercer segmento
tf2
tf
x0i3=X2(2,length(X2(2,:)))
x0f
x1i3=X2(3,length(X2(3,:)))

[p3,X3] = curva_s(s3);
X3=escalar_inv(X3,tf2,tf,x0i3,x0f,x1i3);


