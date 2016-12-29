clear 
global prob_matrix;
global del_t;
prob_matrix = [0.5 0.1 0.4; 0.1 0.2 0.7; 0.3 0.3 0.4];
del_t = 0.01;


global u_space; %inputs
global w_space; %disturbances

u_space = 0.03*[-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1]; %inputs
w_space = 0.1*[1 2 3]; %disturbances

global x_min;
global x_max;
global step_size;
x_min = [0; -1];
x_max = [pi/8; 1];
step_size = [pi/20; 0.1];

global x1_space;
global x2_space;
x1_space = x_min(1):step_size(1):x_max(1);
x2_space = x_min(2):step_size(2):x_max(2);

global g;
g =1;
