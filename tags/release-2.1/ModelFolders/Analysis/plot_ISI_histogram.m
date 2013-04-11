function plot_ISI_histogram
global SDT


i = get(SDT.GUI(1),'value');

ISI_bins = .0005:SDT.ISI_binsize:.05;
ISI = diff(SDT.Spike_data');
[SDT.ISI,SDT.ISI_bins] = hist(ISI,ISI_bins);

SDT.ISI(1,:) =0;
axes(SDT.ax(3))
bar(1000*SDT.ISI_bins,SDT.ISI(:,i))
set(gca,'xlim',[min(1000*SDT.ISI_bins) max(1000*SDT.ISI_bins)],'ylim',[0 1.1*max(SDT.ISI(:,i))],'fontname','times new roman','fontsize',[8],'fontweight','bold')
xlabel('ISI interval [ms]','fontname','times new roman','fontsize',[8],'fontweight','bold')

H = text(25,1.0*max(SDT.ISI(:,i)),strcat('Bin Size =',num2str(fix(100000*SDT.ISI_binsize)/100),' ms'));
set(H,'fontname','times new roman','fontsize',[9],'fontweight','bold','color','r')