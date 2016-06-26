function K = lqr_solution()
M = .5;
m = 0.2;
b1 = 0.1;
b2 = 0.01;
I = 0.006;
g = 9.8;
l = 0.3;
u = 0.0;
p = I*(M+m)+M*m*l^2; %denominator for the A and B matrices

A = [0      1              0           0;
     0 (-(I+m*l^2)*(b1+b2) + m*l^2*b2)/p  (m^2*g*l^2)/p   0;
     0      0              0           1;
     0 -(m*l*b1)/p + M*b2*l/p      m*g*l*(M+m)/p  0];
B = [     0;
     (I+m*l^2)/p;
          0;
        m*l/p];
%{
E = [0;
    (-(I+m*l^2)*(b1+b2) + m*l^2*b2)/p;
     0;
      (M*b2*l + m*b1*l)/p]* v;  
   %}
%x_dot = A*x + B*u +E;
C = [1 0 0 0;
     0 0 1 0];
        
D = [0;
     0];
%y = C*x + D;

states = {'x' 'x_dot' 'phi' 'phi_dot'};
inputs = {'u'};
outputs = {'x'; 'phi'};
sys_ss = ss(A,B,C,D,'statename',states,'inputname',inputs,'outputname',outputs);
poles = eig(A)
co = ctrb(sys_ss);
controllability = rank(co)
Q = [1 0;0 10];
R = 1;
%N = zeros(4,1);
K = lqr(A,B,Q,R);
end