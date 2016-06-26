function x_new = pendulum_nonlinearmodel_ss(X,U,F,del_t)

x_new = zeros(2,1);
m = 0.2;
b = 0.001;
I = 0.006;
g = 9.8;
l = 0.3;


th = X(1);
th_ = X(2);
T = U;
x_new(1) = X(1) + del_t* th_;
x_new(2) = X(2) + del_t* (-m*g*l*sin(th) -b*th_ + F*l*cos(th) + T)/I;
