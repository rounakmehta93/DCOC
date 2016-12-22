function X_new = pendulum_nonlinearmodel_vectorized(X,U_ENERGY,U,F,del_t)
global n_bar;
X_new = zeros(n_bar,2);
m = 0.2;
b = 0.001;
I = 0.006;
g = 9.8;
l = 0.3;


th = X(:,1);
th_ = X(:,2);
T = U;
X_new(:,1) = X(:,1) + del_t* th_;
X_new(:,2) = X(:,2) + del_t* (-m*g*l*sin(th) -b*th_ + F.*l.*cos(th) + T)/I;
