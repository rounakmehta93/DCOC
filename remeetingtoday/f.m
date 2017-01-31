function xdot = f(t,x)
global K_g K_m r_mp m_p B_eq_nominal B_p_nominal l g F_const L M J_eq I
x1 = x(1); x2 = x(2); x3 = x(3); x4=x(4);
B_eq = B_eq_nominal;
B_p = B_p_nominal;
V = sin(t*0.5*2*pi)*3;
F = F_const * (V*r_mp-K_m*K_g*x2);
term = L*g*sin(x3)*cos(x3) - B_p*L*cos(x3)*x4/m_p/l - m_p*l*sin(x3)*x4*x4 + F - B_eq*x2;
xdot(1,1) = x2;
xdot(2,1) = 1/(J_eq + m_p - L*cos(x3)*cos(x3))*term;
xdot(3,1) = x4;
xdot(4,1) = L*g/m_p/l*sin(x3) - B_p/(I+m_p*l*l)*x4 + (L/m_p/l*cos(x3))/(J_eq+m_p-L*cos(x3)*cos(x3))*term;
end

