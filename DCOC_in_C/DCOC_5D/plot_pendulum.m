close all; %closes figures
states = tdfread('XandU.txt',' ');
subplot(6,1,1)
stairs(states.t,states.x0)
title('x')



subplot(6,1,2)
stairs(states.t,states.x1)
title('x dot')

subplot(6,1,3)
stairs(states.t,states.x2)
title('th ')

subplot(6,1,4)
stairs(states.t,states.x3)
title('th dot')

subplot(6,1,5)
stairs(states.t,states.u)
title('u')

subplot(6,1,6)
stairs(states.t,states.w)
title('w')