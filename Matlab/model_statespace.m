function x_dot = model_statespace(t,x,flag)
if(nargin < 3)
    flag = false;
end
M = .5;
m = 0.2;
b1 = 0.1;
b2 = 0.01;
I = 0.006;
g = 9.8;
l = 0.3;

p = I*(M+m)+M*m*l^2; %denominator for the A and B matrices
%{
global counter;
counter = counter + 1;
disp(counter)
%}
global K;

u = -K*x;
global disturb;
disturb = markov_model(disturb);
v = disturb;
A = [0      1              0           0;
     0 (-(I+m*l^2)*(b1+b2) + m*l^2*b2)/p  (m^2*g*l^2)/p   0;
     0      0              0           1;
     0 -(m*l*b1)/p + M*b2*l/p      m*g*l*(M+m)/p  0];
B = [     0;
     (I+m*l^2)/p;
          0;
        m*l/p];
E = [0;
    (-(I+m*l^2)*(b1+b2) + m*l^2*b2)/p;
     0;
      (M*b2*l + m*b1*l)/p]* v;  
x_dot = A*x + B*u; %+ E;
C = [1 0 0 0;
     0 0 1 0];
D = [0;
     0];

end