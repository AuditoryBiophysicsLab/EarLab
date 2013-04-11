% Title:    LogRippleNoise.m
% Author:   D.C. Mountain <dcm@bu.edu>
% Date:     9-Jul-07
%
% The purpose of this program is to create noise with a rippled spectral
% envelope. The approach is based on Won, Drennan, and Rubinstein (2007)
% "Spectral-Ripple Resolution Correlates with Speech Reception
% in Noise in Cochlear Implant Users" JARO  

SampleRate = 44100;
NumSamples = 0.5*SampleRate;        % stimuli will be 500 ms in duration
NumFreqs   = 200;                   % the number of frequency components
MinFreq    = 100;                   % lowest frequency component (Hz)
HighFreq   = 5000;                  % highest frequency component (Hz)

RippleDensity = input('What is the ripple density (ripples/octave)? ');                  
K = menu('Choose an envelope type','Sine','Cosine','Flat');

% compute the frequency values with components equally space in log
% frequency
FreqIncr   = log(HighFreq/MinFreq)/(NumFreqs-1);
FreqValues = MinFreq*exp([0:NumFreqs-1]*FreqIncr);
RippleFrequency = log(2)/(RippleDensity*FreqIncr);

switch K
    case (1)
        MagValues  = abs(sin(pi*[0:NumFreqs-1]/RippleFrequency));
    case (2) 
        MagValues  = abs(cos(pi*[0:NumFreqs-1]/RippleFrequency));% spectral magnitude values
    otherwise
        MagValues(:) = 1;
end

Time = [0:NumSamples-1]/SampleRate; % Time vector for plotting

RippledNoise = zeros(1,NumSamples);

for k=1:NumFreqs
    RippledNoise      = MagValues(k)*sin(2*pi*FreqValues(k)*Time + 2*pi*rand) + RippledNoise;
end
RippledNoise = RippledNoise/max(abs(RippledNoise));

Frequency = [1:(NumSamples/2)]*SampleRate/NumSamples;  % Frequency vector for plotting

NoiseSpectrum = abs(fft(RippledNoise));

figure(1)
plot(Time, RippledNoise)
figure(2)
loglog(Frequency, NoiseSpectrum(2:NumSamples/2 +1))
axis([100 10000,1 1000]);

wavplay(RippledNoise,SampleRate)
wavwrite(RippledNoise,SampleRate,'NoiseFile')