%% other parameters
global del_t;
global prob_matrix;

stopping_criterion = 0.01;

%% nx1 vectors where n = no. of states
global x_min;
global x_max;
global step_size;

%% inputs and disturbance vectors
global u_space; %inputs
global w_space; %disturbances

%% init
%V = zeros([(x_max - x_min)./step_size + ones(2,1)]');
global x1_space;
global x2_space;

[X1,X2,W] = ndgrid(x1_space, x2_space,w_space);
V = zeros(size(X1));
V_old = inf(size(X1));
temp_u = zeros(size(u_space));
temp_w = zeros(size(w_space));

%% Value iteration
global g;

iteration = 0;

while abs(max(V(:)-V_old(:))) > stopping_criterion
    V_old = V;
    
for i = 1:size(x1_space,2) 
    for j = 1:size(x2_space,2)
        for k = 1:size(w_space,2)
        for c = 1:size(u_space,2)
            x_new = pendulum_nonlinearmodel_ss([x1_space(i);x2_space(j)],u_space(c),w_space(k),del_t);
            for d = 1:size(w_space,2)
                
                temp_w(d) = interpn(X1,X2,W,V,x_new(1),x_new(2),w_space(d));
                if isnan(temp_w(d))
                    temp_w(d) = 0;
                end
            end
            temp_u(c) = sum(temp_w.*prob_matrix(k,:)) + g;
        end
        
        V(i,j,k) = max(temp_u);
        end
    end
end
iteration = iteration + 1
err = abs(max(V(:)-V_old(:)))
error(iteration) = err;
end
save('V.mat','V');

plot(1:iteration,error);
xlabel('iterations')
ylabel('error')
