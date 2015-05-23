function X = escalar(Xs,ti,tf,x0i,x0f)

t=Xs(1,:)*(tf-ti)+ti;
x0=Xs(2,:)*(x0f-x0i)+x0i;
x1=Xs(3,:)*(x0f-x0i)/(tf-ti);
x2=Xs(4,:)*(x0f-x0i)/((tf-ti)^2);

X=[t;x0;x1;x2];
