clear
global v;
v = 1; %initialization
global K;

u = 0;
del_t = 0.01;
t_max = 10;
x = [0,0,pi/4,0]';
K = lqr_solution();
%x_array = zeros(4,10/del_t);
t_array = 0:del_t:t_max;
i = 1;
x_array(:,i) = x;

for i = 2:size(t_array,2)
    u = 0;%-K*x;
    x = nonlinearmodel_ss(x,u,del_t);
    
    
    x_array(:,i) = x;
    u_array(:,i) = u;
    
    
    
end
subplot(5,1,1)
plot(t_array,x_array(1,:))
title('x')
subplot(5,1,2)

plot(t_array,x_array(3,:))
title('phi')
%legend('1','2','3','4')
subplot(5,1,3)
plot(t_array,x_array(4,:))
title('phi dot')

subplot(5,1,4)
plot(t_array,x_array(4,:))
title('x dot')

subplot(5,1,5)
plot(t_array,u_array(1,:))
title('u')

%doubt: choice of del_t affects freq of change of stochastic quantity. This
%would affect freq of measurement but not the real model right?



%Value iteration - slow in matlab. (try on simplependulum 1st)- 2008 paper.