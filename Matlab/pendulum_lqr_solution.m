function K = pendulum_lqr_solution(del_t)
x_new = zeros(2,1);
m = 0.2;
b = 0.01;
I = 0.006;
g = 9.8;
l = 0.3;


A = [0 1;
     -m*g*l/I 1/I];
B = [0;l/I];
Q = [0 0; 0 0];
R = 1;
%N = zeros(4,1);
K = lqrd(A,B,Q,R,del_t);
end