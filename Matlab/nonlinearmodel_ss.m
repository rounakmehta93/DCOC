function x_new = nonlinearmodel_ss(X,U,del_t)
x_new = zeros(4,1);
M = .5;
m = 0.2;
b1 = 0.1;
b2 = 0.01;
I = 0.006;
g = 9.8;
l = 0.3;

x = X(1);
x_ = X(2);
ph = X(3);
ph_ = X(4);
F = U;




global v;
%v = markov_model(v);
v=0;

x_new(1) = X(1) + del_t*x_;

x_new(2) = X(2) + del_t*(-(I*b1*v - F*I + I*b2*v + I*b1*x_ + I*b2*x_ - F*l^2*m + b1*l^2*m*v + b2*l^2*m*v + b1*l^2*m*x_ + b2*l^2*m*x_ - (g*l^2*m^2*sin(2*ph))/2 + l^3*m^2*ph_^2*sin(ph) + I*l*m*ph_^2*sin(ph) - b2*l^2*m*v*cos(ph) - b2*l^2*m*x_*cos(ph))/(I*m + I*M + l^2*m^2 - l^2*m^2*cos(ph)^2 + M*l^2*m));
 
x_new(3) = X(3) + del_t*ph_;

x_new(4) = X(4) + del_t*((l*(2*g*m^2*sin(ph) + 2*M*b2*v + 2*M*b2*x_ + 2*b2*m*v + 2*b2*m*x_ + 2*F*m*cos(ph) + 2*M*g*m*sin(ph) - 2*b1*m*v*cos(ph) - 2*b2*m*v*cos(ph) - 2*b1*m*x_*cos(ph) - 2*b2*m*x_*cos(ph) - l*m^2*ph_^2*sin(2*ph)))/(2*(I*m + I*M + l^2*m^2 - l^2*m^2*cos(ph)^2 + M*l^2*m)));

x_new(3) = minimize_angle(x_new(3));