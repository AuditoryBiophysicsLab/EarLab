t = [.01:.01:(size(DRNL, 1) / 100)];
Freq = ([0:size(DRNL, 1) - 1]/size(DRNL, 1)) * 100000;
FFTLen = floor(size(DRNL, 1) / 2);
FreqResp_Lin = 2 * (abs(fft(DRNL_Lin)) / size(Freq, 2));
FreqResp_Nonlin = 2 * (abs(fft(DRNL_Nonlin)) / size(Freq, 2));
FreqResp = 2 * (abs(fft(DRNL)) / size(Freq, 2));
loglog(Freq(1:FFTLen),FreqResp_Lin(1:FFTLen), 'r', Freq(1:FFTLen),FreqResp_Nonlin(1:FFTLen), 'g', Freq(1:FFTLen),FreqResp(1:FFTLen), 'b');
title('DRNL Freq Resp');
legend('Linear', 'Nonlinear', 'DRNL');

