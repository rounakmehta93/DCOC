global disturb;
global counter;
disturb = 1;
counter = 0;
global K;
model_statespace(0,[0,0,0,0]',true);
t = 0:0.1:20;
x0 = [0.1,0,0,0]';
K = lqr_solution();
[t,x] = ode45('model_statespace',t, x0);
subplot(2,1,1)
plot(t,x)
legend('1','2','3','4')
subplot(2,1,2)
plot(t,-x*K') % x is 21x4 . (-K*x')'

% How to integrate the stochastic model with existing matlab solvers. Ode45
% takes much longer to compute. lqr doesn't consider this to be a soln.
%Effectively a non linear model due to constant added.

%Create a non linear model. ode45 cant work with discrete disturbance.
%Discrete time modeling. use for loop. lqrd for discrete time.
%Choose delta t with soln closest to continuous time.
%Value iteration - slow in matlab. (try on simplependulum 1st)- 2008 paper.