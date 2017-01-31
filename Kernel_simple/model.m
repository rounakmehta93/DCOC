function X_new = model(X,U)
global n_bar;
X_new = zeros(n_bar,2);



X_new(:,1) = X(:,1) - U + 1;
X_new(:,2) = X(:,2) - 0.1*U;