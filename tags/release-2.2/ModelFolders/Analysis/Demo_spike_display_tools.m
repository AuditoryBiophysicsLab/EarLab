function Demo_spike_display_tools
global SDT
global SPIKE_DEMO

SPIKE_DEMO.demo_window = figure('position',[40 300 405 425],'name','DEMO: Spike Display Tool','numbertitle','off');
SPIKE_DEMO.task_number = 1;

text_window1 =                      '         Welcome to Spike Display Tool Demo';
text_window1 = strvcat(text_window1,'                                                      ');
text_window1 = strvcat(text_window1,' This tool is intended to instruct Earlab users in the');
text_window1 = strvcat(text_window1,'usage of the Matlab Spike display tools included in this');
text_window1 = strvcat(text_window1,'release. This demo will lead users through a step by step');
text_window1 = strvcat(text_window1,'display of a sample spike data set. After each feature is');
text_window1 = strvcat(text_window1,'demonstrated click the next button to move to the next');
text_window1 = strvcat(text_window1,'feature.');
text_window1 = strvcat(text_window1,'                                      ');
text_window1 = strvcat(text_window1,'                                      ');
text_window1 = strvcat(text_window1,'To launch the Spike Display Tool click the next button');

SPIKE_DEMO.gui(1)=uicontrol('parent',SPIKE_DEMO.demo_window,...
    'units','normalized',...
    'position',[.05 .125 .9 .85],...
    'backgroundcolor',[.8 .85 .95],...
    'style','text',...
    'horizontalalignment','left',...
    'string',text_window1,...
    'Fontsize',[10],...
    'Fontname','Times New Roman',...
    'Fontweight','bold');

SPIKE_DEMO.gui(2)=uicontrol('parent',SPIKE_DEMO.demo_window,...
    'units','normalized',...
    'backgroundcolor',[1 .8 .8],...
    'position',[.35 .025 .3 .05],...
    'style','pushbutton',...
    'horizontalalignment','left',...
    'string','NEXT   >>',...
    'Fontsize',[12],...
    'Fontname','Times New Roman',...
    'callback','global SPIKE_DEMO;demo_launch_tools(SPIKE_DEMO.task_number);SPIKE_DEMO.task_number=SPIKE_DEMO.task_number+1;',...
    'Fontweight','bold');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%























