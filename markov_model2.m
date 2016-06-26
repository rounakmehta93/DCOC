function disturb_new = markov_model2(disturb,disturbances)
%disturbances = [-1,0,1];
disturb_index = find(disturbances==disturb);
assert(size(disturb_index,2)==1)%if error thrown here repetition in disturbances or disturb not present in disturbances.
global prob_matrix
prob_matrix = [0.5 0.1 0.4; 0.1 0.2 0.7; 0.3 0.3 0.4]; 
%row: input column: probabilities of output
prob_matrix_cdf = cumsum(prob_matrix,2);
disturb_index_new = find(prob_matrix_cdf(disturb_index,:)>=rand,1); %returns index of element where CDF<rand

disturb_new = disturbances(disturb_index_new);
%disp(disturb_new);
end