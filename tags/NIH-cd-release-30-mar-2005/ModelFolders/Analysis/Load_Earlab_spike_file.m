function Load_Earlab_spike_file
global SDT

%%%%%% Load raw data files %%%%%%%%%%%%%%%%%%%%%%%%% 
SDT.data_name = deblank(strrep(SDT.filename,'.out',''));
Data =load(strcat(SDT.path_name,SDT.filename));
input_waveform = load(strcat(SDT.input_path,SDT.input_filename));
input_waveform = input_waveform/max(abs(input_waveform));

SDT.Fs = 100000;
SDT.analysis_frequency=500;

t =(1:length(input_waveform))/SDT.Fs;
DIMS = size(Data);
Num_fibers =max(Data(:,1))+1;
SDT.spike_count = zeros(1,Num_fibers);

SDT.period_binsize=64;
SDT.ISI_binsize=.0005;

%%%%%%% Set up fiber plotting menu %%%%%%%%%%%%%%%
    for i = 1:Num_fibers
        if i == 1
            fiber_list = strcat('Fiber..1');
        else
            fiber_list = strvcat(fiber_list,strcat('Fiber..',num2str(i)));
        end
    end
    
   set(SDT.GUI(1),'string',fiber_list);
%%%%%%%%%% Sort Spikes by fiber %%%%%%%%%%%%%%%%%%%%

for i = 1:DIMS(1)
   SDT.spike_count(Data(i,1)+1)=SDT.spike_count(Data(i,1)+1)+1;
   SDT.Spike_data(Data(i,1)+1,SDT.spike_count(Data(i,1)+1))=Data(i,2); 
end


%%%%%%%%%%%%% Plot Data that has been loaded %%%%%%%%%%

axes(SDT.ax(1));
SDT.SPIKEPLOT = plot(SDT.Spike_data(1:Num_fibers,:),1:Num_fibers,'k.');
set(SDT.SPIKEPLOT,'markersize',[.5])
set(SDT.ax(1),'xlim',[min(t) max(t)],'ylim',[1 Num_fibers],'fontname','times new roman','fontsize',[10],'fontweight','bold')
xlabel('Time [seconds]','fontname','times new roman','fontsize',[10],'fontweight','bold')
ylabel('Fiber Number','fontname','times new roman','fontsize',[10],'fontweight','bold')

axes(SDT.ax(2));
plot(t,input_waveform)
set(SDT.ax(2),'xlim',[min(t) max(t)],'ylim',[min(input_waveform) max(input_waveform)],'xtick',[-1],'ytick',[-10000000])