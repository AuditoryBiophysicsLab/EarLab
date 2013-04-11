function [syncindex,two_n_Rsq] = synccalc(spike_times_sec,period_sec)

% function [syncindex,two_n_Rsq] = synccalc(spike_times_sec,period_sec)
%
% The "sync" or "synchronization coefficient" is sometimes also referred
% to as the "vector strength". It is a dimensionless number that varies 
% between 0 and 1, and provides an indication of how "phase locked" 
% the unit response is to the stimulus waveform.
%
% period_sec is the stimulus period n seconds
% spike_times_sec is a vector of spike times in seconds
% (as long as period and spike times are in same units
% everything will be fine.)
%
% syncindex is the synchronization index
% Rayleigh Criterion:
% two_n_Rsq is 2*number_of_spikes*(syncindex^2)
% two_n_Rsq must be greater than 13.8 
% for the probability to be less than 0.001 
% that the implicit period histogram 
% occurred from uniform distribution (no phase-locking)




theta = 2*pi*mod(spike_times_sec,period_sec)/period_sec;
N = length(spike_times_sec);
syncindex = sqrt((sum(cos(theta)))^2 + (sum(sin(theta)))^2)/N;

two_n_Rsq = 2*N*syncindex;