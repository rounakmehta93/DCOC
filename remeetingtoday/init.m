global K_g K_m r_mp m_p B_eq_nominal B_p_nominal l g F_const L M J_eq I
eta_g = 0.9;
eta_m = 0.69;
K_g = 3.71;
K_t = 7.68*10^(-3);
K_m = 7.68*10^(-3);
r_mp = 6.35*10^(-3);
R_m = 2.6;
m_c = 0.38;
m_p = 0.23;
I = 7.88*10^(-3);
B_eq_nominal = 4.3;
B_p_nominal = 0.0024;
l = 0.3302;
g = 9.81;
J_m = 3.9*10^(-7);

L = (m_p*l)^2/(I+m_p*l^2);
M = m_c + m_p;
J_eq = m_c +eta_g*K_g^2*J_m/r_mp/r_mp;

if 1==1
    time = 0:0.002:10;
    x0 = [0;0;-pi;0];
    load('output_u.mat')
    out(4,:) = out(4,:)-pi;
    F_const = eta_g*K_g*eta_m*K_t/(r_mp^2*R_m);
else
    time = 0:0.002:12.52;
    x0 = [0;0;1.557-pi;0];
    load('output.mat')
    out(4,:) = out(4,:)-pi;
    F_const = 0;
end