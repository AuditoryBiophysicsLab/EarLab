function bin_resize_histogram
global SDT

period_flag = get(SDT.GUI(2),'value');
ISI_flag = get(SDT.GUI(3),'value');

scale = get(gcbo,'value');

if period_flag ==1 
    SDT.period_binsize=(1-scale)*90 +15;
    plot_period_histogram
elseif ISI_flag ==1
    SDT.ISI_binsize=(scale)*.005 + 1/SDT.Fs;
    plot_ISI_histogram
end











