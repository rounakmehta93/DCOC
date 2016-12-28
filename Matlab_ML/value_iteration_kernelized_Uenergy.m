%%TODOs
%bug: ability to change n_bar
%bug: diverges for w_space > 1
%u_energy as another state.
%get loss: distance of training points with functions.

%% other parameters
global del_t;
global prob_matrix;
%play with sigma and lambda
stopping_criterion = 0.01;


%% nx1 vectors where n = no. of states
global x_min;
global x_max;
global num_states;


%% inputs and disturbance vectors
global u_space; %inputs
global w_space; %disturbances

%% init
global n_bar;
n_bar = 1000; % total no. of data points to use
n_bar_edge = 20; %per edge
assert(n_bar > n_bar_edge * (num_states+1) *2)

%generate random numbers in the desired range
X = rand(n_bar, num_states);
X = repmat(x_min',[n_bar,1]) + X.* repmat((x_max - x_min)',[n_bar,1]);

%generate random numbers on the edge
% for each state min/max choose n_bar_edge values

for i = 1:num_states
    X(n_bar_edge*(i-1)+1 : n_bar_edge*i,i) = x_min(i);
    
end
for i = 1:num_states
    X(n_bar_edge*num_states + n_bar_edge*(i-1)+1 : n_bar_edge*num_states + n_bar_edge*i,i) = x_max(i);
    
end

%append u_energy random values.
X_energy = rand(n_bar,1) * u_energy_max + 0;
X = [X X_energy];

X(2*n_bar_edge*num_states + 1 : 2*n_bar_edge*num_states + 2*n_bar_edge, num_states+1) = 0;

%append random w_values
W_indices = randi(size(w_space,2),n_bar,1);
X = [X w_space(:,W_indices)'];






%initialize Value function estimate at these points
V = zeros(size(X,1),1);

V_old = inf(size(X,1),1);

K = zeros(n_bar,n_bar);
K_vector = zeros(n_bar,n_bar*size(u_space,2)*size(w_space,2));
%temp_u = zeros(size(u_space));
%temp_w = zeros(size(w_space));

%% Value iteration
global g;
global sigma;
sigma=15; % for gaussian kernel
lambda=0.000001;%0.003;
iteration = 0;
K =exp(-dist2(X,X)/(2*sigma^2));
X_new = zeros(n_bar,num_states+1,size(u_space,2));

while abs(max(V(:)-V_old(:))) > stopping_criterion
    V_old = V;
   
   
    %stack the array as such [X,u1,w1 X,u2,w1, X,u1,w2 X,u2,w2]' 
    for c = 1:size(u_space,2)
        X_new(:,:,c) = pendulum_nonlinearmodel_vectorized(X(:,1:2),X(:,3),u_space(c),X(:,4),del_t);
        
        % we know what the current disturbance is. 
    end
    %get the [X,u1 ; X,u2] from [X,u1 X,u2]
    Xtemp1 = reshape(X_new, [n_bar*size(u_space,2) num_states+1]);
    %using max instead of an OR condition to work on a matrix. 0 or 1
    %anyways. 1 when it does go out of bounds
    exit_indices_matrix = max(Xtemp1 > repmat([x_max; inf]',[size(Xtemp1,1),1]) ,...
                              Xtemp1 < repmat([x_min; 0]',[size(Xtemp1,1),1]));
    %1 where the value should be 0. logical matrix
    exit_indices = sum(exit_indices_matrix,2) > 0;
    
    %get the [w1 w2 w3; w1 w2 w3 ...] vector
    Wtemp0 = repmat(w_space,[size(Xtemp1,1) 1]);
    %{
    for d = 1:size(w_space,2)
        Wtemp0(:,d) = repmat(w_space(d),[size(Xtemp1,1) 1]);
    end
    %}
    %get the [w1; w1; ... w2; w2;...; w3; w3...] vector
    Wtemp = reshape(Wtemp0,[size(Wtemp0,1)*size(Wtemp0,2), 1]);
    %get the [X,u1,w1; X,u2,w1; X,u1,w2; X,u2,w2] vector
    X_temp2 = [repmat(Xtemp1,[size(w_space,2), 1]) Wtemp];
    
    K_ = exp(-dist2(X,X_temp2)/(2*sigma^2));
    
    V_temp = ( V' * inv(K + n_bar*lambda*eye(n_bar) ) * K_ )';
    %convert the stacked array to a matrix. [n_bar*uspace*wspace.
    % need to test when w space is >1. Diverging
    V_temp2 = reshape(V_temp, [n_bar*size(u_space,2), size(w_space,2)]); 
    V_temp2(exit_indices,:) = 0;
    V_expectation = sum(V_temp2,2);
    %at whatever index x exits the bounds we can set V to 0 vector
    V_expectation_reshape = reshape(V_expectation, [n_bar, size(u_space,2)]);
    V = max(V_expectation_reshape,[],2) + g;
    
        %{
            for d = 1:size(w_space,2)
                K_ = exp(-dist2(X,[x_new, w_space(d)])/(2*sigma^2));
                if ((sum(x_new>x_max)>0 || sum(x_new<x_min) > 0))
                    temp_w(d) = 0;
                else
                temp_w(d) = V * inv(K + n_bar*lambda*eye(n_bar) ) * K_;
                %interpn(X1,X2,W,V,x_new(1),x_new(2),w_space(d));
                end
            end
            temp_u(c) = sum(temp_w.*prob_matrix(find(w_space==X(3,i)),:)) + g;
        end
        
        V(i) = max(temp_u);
      %}  
     %for each point 
   

iteration = iteration + 1
abs(max(V(:)-V_old(:)))

end
save('V.mat','V');
alpha = V' * inv(K + n_bar*lambda*eye(n_bar) );
save('alpha.mat','alpha','X');
%save('alpha.mat','X');
