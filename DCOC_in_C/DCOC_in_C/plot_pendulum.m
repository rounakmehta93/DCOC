close all; %closes figures
states = tdfread('XandU.txt',' ');
subplot(3,1,1)
stairs(states.t,states.x1)
title('th')



subplot(3,1,2)
stairs(states.t,states.x2)
title('th dot')



subplot(3,1,3)
stairs(states.t,states.u)
title('u')