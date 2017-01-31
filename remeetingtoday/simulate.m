clear;
init;
options = odeset('AbsTol',1e-7,'RelTol',1e-7);
[T,X] = ode45(@f,time,x0,options);

close all
figure(1)
subplot(411)
plot(T,out(2,:),'k');hold on;
plot(T,X(:,1),'r--')
legend('measured','model','Location','Northeast')
ylabel('x_c (m)')
subplot(412)
plot(T,out(3,:),'k');hold on;
plot(T,X(:,2),'r--')
ylabel('dx_c/dt (m/sec)')
subplot(413)
plot(T,rad2deg(out(4,:)),'k');hold on;
plot(T,rad2deg(X(:,3)),'r--')
ylabel('\theta (deg)')
subplot(414)
plot(T,rad2deg(out(5,:)),'k');hold on;
plot(T,rad2deg(X(:,4)),'r--')
ylabel('d\theta/dt (deg/sec)')
xlabel('time (sec)')