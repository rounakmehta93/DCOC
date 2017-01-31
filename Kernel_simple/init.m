clear 
global del_t;
del_t = 0.01;

global u_space; %inputs
global u_energy_multiplier;
global u_energy;
global u_energy_max;
u_energy_max = 90;
u_energy = u_energy_max;
u_space = 0.03*[-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1]; %inputs
u_energy_multiplier = 10;

global x_min;
global x_max;
global num_states;

num_states = 2;
x_min = [-3; 0];
x_max = [3; 1];


global g;
g =1;

global sigma;
sigma=30;