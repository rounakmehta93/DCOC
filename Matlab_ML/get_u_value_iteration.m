function [u,V] = get_u_value_iteration(alpha,X,x,w)
%% other parameters
global del_t;
global prob_matrix;


%% nx1 vectors where n = no. of states
global x_min;
global x_max;
global step_size;
global sigma;
global u_energy;
global u_energy_multiplier;

%% inputs and disturbance vectors
global u_space; %inputs
global w_space; %disturbances

%% init
%V = zeros([(x_max - x_min)./step_size + ones(2,1)]');
global x1_space;
global x2_space;

%[X1,X2,W] = ndgrid(x1_space, x2_space,w_space);
temp_u = zeros(size(u_space));
temp_w = zeros(size(w_space));

%% Value iteration
global g;



    if(sum(x>x_max)>0 || sum(x<x_min) > 0)  %possible bug fix
        u = NaN;
        V = 0;
        return;
    end
    if(u_energy < 0) 
        u = NaN;
        V = 0;
        disp('U energy < 0');
        return;
    end
        for c = 1:size(u_space,2)
            x_new = pendulum_nonlinearmodel_ss([x(1);x(2)],u_space(c),w,del_t);
            for d = 1:size(w_space,2)
                K_ = exp(-dist2(X,[x_new; w_space(d)]')/(2*sigma^2));
                if (sum(x_new>x_max)>0 || sum(x_new<x_min) > 0)
                    temp_w(d) = 0;
                else
                temp_w(d) =alpha * K_;
                %interpn(X1,X2,W,V,x_new(1),x_new(2),w_space(d));
                end
            end
            temp_u(c) = sum(temp_w.*prob_matrix(find(w_space==w),:)) + g;
        end
        [V,u_index] = max(temp_u);
        u_energy = u_energy - u_energy_multiplier * abs(u_space(u_index));
u = u_space(u_index);


end
