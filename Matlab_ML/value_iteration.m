%% other parameters
global del_t;
global prob_matrix;
%play with sigma and lambda
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
n_bar = 100 % total no. of data points to use
X = rand(2,n_bar);
X = repmat(x_min,[1,n_bar]) + X.* repmat((x_max - x_min),[1,n_bar]);
%[X1,X2,W] = ndgrid(x1_space, x2_space,w_space);
W_indices = randi(size(w_space,2),1,n_bar);
X = [X;w_space(W_indices)];
%V = zeros(size(X1));
%V_old = inf(size(X1));
V = zeros(1,size(X,2));
V_old = inf(1,size(X,2));
K = zeros(n_bar,n_bar);
K_ = zeros(n_bar,1);
temp_u = zeros(size(u_space));
temp_w = zeros(size(w_space));

%% Value iteration
global g;
global sigma;
sigma=15; % for gaussian kernel
lambda=0.00000001;%0.003;
iteration = 0;
K =exp(-dist2(X',X')/(2*sigma^2));
while abs(max(V(:)-V_old(:))) > stopping_criterion
    V_old = V;
    
   
    %K =exp(-dist2(X',X')/(2*sigma^2));
   
    for i = 1:n_bar
     
        for c = 1:size(u_space,2)
            x_new = pendulum_nonlinearmodel_ss(X(1:2,i),u_space(c),X(3,i),del_t);
            
            for d = 1:size(w_space,2)
                K_ = exp(-dist2(X',[x_new; w_space(d)]')/(2*sigma^2));
                if ((sum(x_new>x_max)>0 || sum(x_new<x_min) > 0) || u_energy < 0)
                    temp_w(d) = 0;
                else
                temp_w(d) = V * inv(K + n_bar*lambda*eye(n_bar) ) * K_;
                %interpn(X1,X2,W,V,x_new(1),x_new(2),w_space(d));
                end
            end
            temp_u(c) = sum(temp_w.*prob_matrix(find(w_space==X(3,i)),:)) + g;
        end
        
        V(i) = max(temp_u);
        
     
    end

iteration = iteration + 1
abs(max(V(:)-V_old(:)))

end
save('V.mat','V');
alpha = V * inv(K + n_bar*lambda*eye(n_bar) );
save('alpha.mat','alpha','X');
%save('alpha.mat','X');
