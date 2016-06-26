syms F M m b1 b2 I g l x x_ x__ ph ph_ ph__ Rx Ry N v;
eqn1 = M*x__ == F - b1*(x_+v) - Rx;
eqn2 = Rx - b2*(x_+v) == m*(x__ - ph__*l*cos(ph) + ph_^2*l*sin(ph)); %needs to change for drag to account for horizontal velocity of beam wrt cart
%eqn3 = - m*g*cos(ph) + Ry*cos(ph) - Rx*sin(ph) == m*(- ph_^2*l - x__*sin(ph));
eqn4 = - m*g*sin(ph) + Ry*sin(ph) + Rx*cos(ph) - b2*(x_+v)== m*(- ph__*l  + x__*cos(ph));
eqn5 = I*ph__ == Ry*sin(ph)*l + Rx*cos(ph)*l; %torque due to drag about center of mass = 0

%eqn4_linear = subs(eqn4,[cos(ph),sin(ph)],[1,ph]);
Rx_sol = solve(eqn2,Rx); 
EQN1 = subs(eqn1,Rx,Rx_sol); %eliminate Rx. need to eliminate ph__
EQN1 = x__ == solve(EQN1,x__);


Ry_sol = solve(eqn4,Ry);
EQN2 = subs(eqn5,Ry,Ry_sol);
EQN2 = subs(EQN2,Rx,Rx_sol);
EQN2 = ph__ == solve(EQN2, ph__);

%%linearize
%EQN1 = subs(EQN1,[cos(ph),sin(ph),ph_],[1,ph,0]);
%EQN2 = subs(EQN2,[cos(ph),sin(ph),ph_],[1,ph,0]);

ph__sol = solve(EQN2,ph__);
EQN3 = subs(EQN1,ph__,ph__sol);
x__sol = solve(EQN3, x__);
EQN4 = subs(EQN2, x__,x__sol);
ph__sol = solve(EQN4,ph__);

x__sol = simplify(x__sol)
ph__sol = simplify(ph__sol)