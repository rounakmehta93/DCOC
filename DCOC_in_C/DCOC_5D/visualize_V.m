close all
V_temp = csvread('V_out_neg.txt');
w_len = 1;
x0_len = 5;
x1_len = 5;
x2_len = 5;
x3_len = 5;
V_temp_counter = 1;
V= zeros(x0_len, x1_len, x2_len, x3_len, w_len);
for i0 = 1:x0_len
    for i1 = 1:x1_len
        for i2 = 1:x2_len
            for i3 = 1:x3_len
                for i4 = 1:w_len
                    V(i0,i1,i2,i3,i4) = V_temp(V_temp_counter);
                    V_temp_counter = V_temp_counter + 1;
                end
            end
        end
    end
end
for i=1:5
    for j = 1:5
        figure((i-1)*5 + j)
        subplot(1,2,1)
        surf(V(:,:,i,j,1))
    end   
end

V_temp = csvread('V_out_pos.txt');
w_len = 1;
x0_len = 5;
x1_len = 5;
x2_len = 5;
x3_len = 5;
V_temp_counter = 1;
V= zeros(x0_len, x1_len, x2_len, x3_len, w_len);
for i0 = 1:x0_len
    for i1 = 1:x1_len
        for i2 = 1:x2_len
            for i3 = 1:x3_len
                for i4 = 1:w_len
                    V(i0,i1,i2,i3,i4) = V_temp(V_temp_counter);
                    V_temp_counter = V_temp_counter + 1;
                end
            end
        end
    end
end
for i=1:5
    for j = 1:5
        figure((i-1)*5 + j)
        subplot(1,2,2)
        surf(V(:,:,i,j,1))
    end   
end
%{    
figure(1)
subplot(1,2,2)
surf(V(:,:,1,2,1))
figure(2)
subplot(1,2,2)
surf(V(:,:,1,3,1))
figure(3)
subplot(1,2,2)
surf(V(:,:,2,1,1))
figure(4)
subplot(1,2,2)
surf(V(:,:,2,3,1))
figure(5)
subplot(1,2,2)
surf(V(:,:,3,1,1))
        %}