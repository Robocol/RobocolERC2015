q2=-50:10:50;
for i=1:length(q2)
	I(i)=IActuators(q2(i),0,0);
end

plot(q2,I)
