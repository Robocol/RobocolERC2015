clear
clc
[X1,X2,X3]=campana(0.33,0.66,1,1,0,4,0,12,0);
X=[X1,X2,X3];
plot(X(1,:),X(2,:))
