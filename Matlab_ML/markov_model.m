function disturb_new = markov_model(disturb)
prob_matrix = [0.5 0.1 0.4; 0.1 0.2 0.7; 0.3 0.3 0.4]; 
%row: input column: probabilities of output
prob_matrix_cdf = cumsum(prob_matrix,2);
disturb_new = find(prob_matrix_cdf(disturb,:)>=rand,1); %returns index of element where CDF<rand

%disp(disturb_new);
end