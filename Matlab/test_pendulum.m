
global w_space;


u = 0;
w = w_space(1); %initialization
global del_t;

t_max = 20;
x = [pi/20,0]';
%K = pendulum_lqr_solution(del_t);
x_array = zeros(2,t_max/del_t+1);
w_array = zeros(1,t_max/del_t+1);
t_array = 0:del_t:t_max;
u_array = zeros(1,t_max/del_t+1);
V_array = zeros(1,t_max/del_t+1);
i = 1;
x_array(:,i) = x;
temp = load('V.mat');
V = temp.V;
exit_i = size(t_array,2);
for i = 2:size(t_array,2)
    %u = -K*x;
    [u,V_val] = get_u_value_iteration(V,x,w);
    if isnan(u)
        exit_i = i;
        break;
    end
    w = markov_model2(w,w_space);
    x = pendulum_nonlinearmodel_ss(x,u,w,del_t);
    
    x(1) = minimize_angle(x(1));
    x_array(:,i) = x;
    u_array(:,i) = u;
    w_array(:,i) = w;
    u_array(:,i) = u;
    V_array(:,i) = V_val;

    
    
end
subplot(5,1,1)
stairs(t_array(:,1:exit_i),x_array(1,1:exit_i))
title('th')



subplot(5,1,2)
stairs(t_array(:,1:exit_i),x_array(2,1:exit_i))
title('th dot')



subplot(5,1,3)
stairs(t_array(1,1:exit_i),u_array(1,1:exit_i))
title('u')

subplot(5,1,4)
stairs(t_array(1,1:exit_i),w_array(1,1:exit_i))
title('w')

subplot(5,1,5)
stairs(t_array(1,1:exit_i),V_array(1,1:exit_i))
title('V')

figure(2)
%values vs coordinates of the values.
[X,Y] = meshgrid(x2_space',x1_space');
surf(Y,X,mean(V,3))
zlabel('V')
xlabel('x1')
ylabel('x2')

% play around with parameters
% grid density vs time and results
% implement in C