% Title:    RippleNoise.m
% Author:   D.C. Mountain <dcm@bu.edu>
% Date:     9-Jul-07
%
% The purpose of this program is to create noise with a rippled spectral
% envelope. The approach is based on Henry, BA and Turner, CW (2003) JASA
% 113:2861-2873

SampleRate = 44100;
NumSamples = 0.5*SampleRate;        % stimuli will be 500 ms in duration
RippleSpacing = 1000;               % spacing between ripple peaks in Hertz
DelaySamples  = round(SampleRate/RippleSpacing);

Time = [0:NumSamples-1]/SampleRate; % Time vector for plotting
Frequency = [0:(NumSamples/2)-1]*SampleRate/NumSamples;

WhiteNoise = randn(NumSamples+DelaySamples,1);
RippledNoise = WhiteNoise(1:NumSamples)+ WhiteNoise(DelaySamples:DelaySamples+NumSamples-1);

NoiseSpectrum = abs(fft(RippledNoise));

figure(1)
plot(Time,RippledNoise)
figure(2)
plot(Frequency, NoiseSpectrum(1:NumSamples/2))