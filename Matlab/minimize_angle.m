function b = minimize_angle(a)
if a >= 2*pi || a<=0 
    b = rem(a,2*pi);
else
    b = a;
end
