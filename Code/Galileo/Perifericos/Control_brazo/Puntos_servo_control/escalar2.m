function X = escalar2(Xs,ti,tf,x0i,x0f,vi)

t=Xs(1,:)*(tf-ti)+ti;
x0=Xs(2,:)*(x0f-x0i)+x0i+vi*t;
x1=Xs(3,:)*(x0f-x0i)/(tf-ti)+vi;
x2=Xs(4,:)*(x0f-x0i)/((tf-ti)^2);
x3=Xs(5,:)*(x0f-x0i)/((tf-ti)^3);

X=[t;x0;x1;x2;x3];
