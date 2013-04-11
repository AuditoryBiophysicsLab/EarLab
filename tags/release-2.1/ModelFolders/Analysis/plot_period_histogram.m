function plot_period_histogram
global SDT

phase_bins =0:2*pi/SDT.period_binsize:2*pi;

i = get(SDT.GUI(1),'value');
SDT.analysis_frequency = str2num(get(SDT.GUI(6),'string'));

if SDT.analysis_frequency < 1 
    SDT.analysis_frequency = 1; 
    set(SDT.GUI(6),'string',num2str(1));
elseif SDT.analysis_frequency > 10000
    SDT.analysis_frequency = 10000; 
    set(SDT.GUI(6),'string',num2str(10000));   
end

period_hist = 2*pi*mod(SDT.Fs*SDT.Spike_data(i,1:SDT.spike_count(i)),SDT.Fs*(1/SDT.analysis_frequency))/(SDT.Fs/SDT.analysis_frequency);
Nspikes = length(period_hist);
[N,x]=hist(period_hist,phase_bins);


axes(SDT.ax(3))
bar(180*x/pi,N)
[peak,index]=max(N);
hold on
plot(180*phase_bins/pi,peak*(cos(phase_bins-phase_bins(index))+1)/2,'r:')
set(gca,'xlim',[min(180*phase_bins/pi) max(180*phase_bins/pi)],'ylim',[0 peak*1.1],'xtick',[0 90 180 270],'fontname','times new roman','fontsize',[8],'fontweight','bold')
xlabel('Phase degrees','fontname','times new roman','fontsize',[8],'fontweight','bold')
hold off

sync_value = sqrt((sum(cos(period_hist))/Nspikes).^2 + (sum(sin(period_hist))/Nspikes).^2);

H = text(0,1.05*peak,strcat('Sync =',num2str(fix(100*sync_value)/100)));
set(H,'fontname','times new roman','fontsize',[9],'fontweight','bold','color','r')