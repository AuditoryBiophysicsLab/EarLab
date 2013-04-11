function demo_launch_tools(task_number)
global SPIKE_DEMO 
global SDT

if task_number == 1
    Spike_display_tool;
    
    text_window2 =                      '         Opening the Spike Display Tool';
    text_window2 = strvcat(text_window2,'                                                      ');
    text_window2 = strvcat(text_window2,' The tool is opened by either opening Matlab and');
    text_window2 = strvcat(text_window2,'changing directories to the Analysis sub directory of');
    text_window2 = strvcat(text_window2,'the Earlab folder and typing: Spike_display_tool or');
    text_window2 = strvcat(text_window2,'double clicking the Spike_display_tool.m file ');
    text_window2 = strvcat(text_window2,'        [Only for certain operating systems]');
    text_window2 = strvcat(text_window2,'                                               ');
    text_window2 = strvcat(text_window2,'NOTE: The Demo has only been validated for Matlab'); 
    text_window2 = strvcat(text_window2,'      version Release 13                         ');
    text_window2 = strvcat(text_window2,'                                               ');
    text_window2 = strvcat(text_window2,'The Spike display tool has been launched automatically');
    text_window2 = strvcat(text_window2,'the tool is running in a window labeled:        ');
    text_window2 = strvcat(text_window2,'               Spike Display Tool        ');
    text_window2 = strvcat(text_window2,'                                               ');
    text_window2 = strvcat(text_window2,' The Demo window will be kept in the foreground     ');
    text_window2 = strvcat(text_window2,'Simply click on the different windows to toggle ');
    text_window2 = strvcat(text_window2,'between the two display windows.                  ');
    text_window2 = strvcat(text_window2,'                                               ');
    text_window2 = strvcat(text_window2,'To load sample spike data click the next button');
    
    set(SPIKE_DEMO.gui(1),'string',text_window2)
    
    figure(SPIKE_DEMO.demo_window)
    
elseif task_number == 2
    
    text_window3 =                      '         Loading a Spike File';
    text_window3 = strvcat(text_window3,'                                                      ');
    text_window3 = strvcat(text_window3,' Go to the Spike Display window and double click on ');
    text_window3 = strvcat(text_window3,'the file: ''CNSpike''  in the listbox at the upper left');
    text_window3 = strvcat(text_window3,'of the display window. This will load the sample');
    text_window3 = strvcat(text_window3,'data set and display both the acoustic input file');
    text_window3 = strvcat(text_window3,'[top graph] and a dot raster of the generated spikes');
    text_window3 = strvcat(text_window3,'[lower graph]. There are several tools to manipulate');
    text_window3 = strvcat(text_window3,'the data and perform analysis of the spike trains.');
    text_window3 = strvcat(text_window3,'                                                  ');
    text_window3 = strvcat(text_window3,'The sample data is the response of 64 individual ');
    text_window3 = strvcat(text_window3,'model Bushy cells to a 500 Hz acoustic tones ');
    text_window3 = strvcat(text_window3,'presented to a full peripheral auditory model.     ');
    text_window3 = strvcat(text_window3,'                         ');
    text_window3 = strvcat(text_window3,' Once the Spike data is loaded click the next button');
    text_window3 = strvcat(text_window3,' to see simple data manipulation               ');
    text_window3 = strvcat(text_window3,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window3)
    
    figure(SPIKE_DEMO.demo_window)
    
elseif task_number == 3
    
    text_window4 =                      '         Manipulating Spike Display: Zooming';
    text_window4 = strvcat(text_window4,'                                                      ');
    text_window4 = strvcat(text_window4,' Go to the Spike Display window and click the button ');
    text_window4 = strvcat(text_window4,'marked ''Zoom'' in the lower left of the display window.');
    text_window4 = strvcat(text_window4,'The button will turn red and the the text will change');
    text_window4 = strvcat(text_window4,'to ''Zooming''. Move the cursor over the dot raster ');
    text_window4 = strvcat(text_window4,'plot, click over the area of interest and hold');
    text_window4 = strvcat(text_window4,'a drag box will appear to select a subset of the graph.');
    text_window4 = strvcat(text_window4,'Release the mouse button and the graph will resize both');
    text_window4 = strvcat(text_window4,'the dot raster plot and the input waveform plot.');                                              
    text_window4 = strvcat(text_window4,'To continue zooming click the zoom button again, to zoom out');
    text_window4 = strvcat(text_window4,'click the zoom buton and right click over the dot raster');
    text_window4 = strvcat(text_window4,'plot, the graph will zoom out by a factor of two.');
    text_window4 = strvcat(text_window4,'NOTE: All GUI tools have tooltip strings, simply place '); 
    text_window4 = strvcat(text_window4,'the cursor over any gui object to see text help     '); 
    text_window4 = strvcat(text_window4,'                         ');
    text_window4 = strvcat(text_window4,'                                ');
    text_window4 = strvcat(text_window4,' To see markersize manipulations click the next button ');
    text_window4 = strvcat(text_window4,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window4)
    
    figure(SPIKE_DEMO.demo_window)
    
elseif task_number == 4
    
    text_window5 =                      '     Manipulating Spike Display: Changing Marker size';
    text_window5 = strvcat(text_window5,'                                                      ');
    text_window5 = strvcat(text_window5,' Go to the Spike Display window and select the slider ');
    text_window5 = strvcat(text_window5,'marked ''Change Marker Size'' at the lower left of the');
    text_window5 = strvcat(text_window5,'of the dot raster plot. By moving the slider to the');
    text_window5 = strvcat(text_window5,'right the marker used to respresent a spike will ');
    text_window5 = strvcat(text_window5,'increase in size, moving to the left will deacrease it.');
    text_window5 = strvcat(text_window5,'Changing marker size is particularly useful when ');
    text_window5 = strvcat(text_window5,'zooming or loading files with various quantities of ');
    text_window5 = strvcat(text_window5,'spikes. When zooming in or loading files with few spikes');                                              
    text_window5 = strvcat(text_window5,'it is helpful to increase marker size. Using the zoom');
    text_window5 = strvcat(text_window5,'buton select a small section of the dot raster plot');
    text_window5 = strvcat(text_window5,'the markers will be small and difficult to see, slide');
    text_window5 = strvcat(text_window5,'the slider button to the left and increase the marker '); 
    text_window5 = strvcat(text_window5,'size. Zoom out and notice how crowded the graph appears.'); 
    text_window5 = strvcat(text_window5,'Move the slider to the left to resize the markers.      ');
    text_window5 = strvcat(text_window5,'                                ');
    text_window5 = strvcat(text_window5,' To see simple analysis tools click the next button ');
    text_window5 = strvcat(text_window5,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window5)
    
    figure(SPIKE_DEMO.demo_window)
    
 elseif task_number == 5
    
    text_window6 =                      '     Single Fiber Analysis Tools: Selecting a Fiber';
    text_window6 = strvcat(text_window6,'                                                      ');
    text_window6 = strvcat(text_window6,' Once a spike data file is loaded, some simple analysis');
    text_window6 = strvcat(text_window6,'can be performed using the tools provided. To select a');
    text_window6 = strvcat(text_window6,'a single fiber for an analysis go to the pull down menu');
    text_window6 = strvcat(text_window6,'to the left of the dot raster plot marked: ');
    text_window6 = strvcat(text_window6,'''Fiber to Analyze'', select a fiber number by using the');
    text_window6 = strvcat(text_window6,'pull down menu and associated vertical scroll button,  ');
    text_window6 = strvcat(text_window6,'fibers between 16 and 25 are stimulated by the acoustic');
    text_window6 = strvcat(text_window6,'input signal in the sample data, fibers below and above');                                              
    text_window6 = strvcat(text_window6,'demonstrate primarily spontaneous rate. Once a fiber is');
    text_window6 = strvcat(text_window6,'selected two analysis tools are available to analyze the');
    text_window6 = strvcat(text_window6,'single fiber responses, a period histogram and a');
    text_window6 = strvcat(text_window6,'inter spike interval (ISI) histogram. '); 
    text_window6 = strvcat(text_window6,'                                                    '); 
    text_window6 = strvcat(text_window6,'                                  ');
    text_window6 = strvcat(text_window6,' To see example of a period histogram click the next button');
    text_window6 = strvcat(text_window6,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window6)
    
    figure(SPIKE_DEMO.demo_window)   
    
 elseif task_number == 6
    
    text_window7 =                      '     Single Fiber Analysis Tools: Period Histogram';
    text_window7 = strvcat(text_window7,'                                                      ');
    text_window7 = strvcat(text_window7,' After a single fiber is selected, select an analysis ');
    text_window7 = strvcat(text_window7,'frequency by entering a frequency in Hertz in the');
    text_window7 = strvcat(text_window7,'textbox below the fiber select menu. The default ');
    text_window7 = strvcat(text_window7,'frequency of 500 Hz appropriate for the sample data.');    
    text_window7 = strvcat(text_window7,'Once an analysis frequncy is selected, click on the');  
    text_window7 = strvcat(text_window7,'circular radio button marked: '' Period Histogram'' ');
    text_window7 = strvcat(text_window7,'below the frequency analysis textbox. Once the button');
    text_window7 = strvcat(text_window7,'is clicked a small graph will appear below the radio');                                              
    text_window7 = strvcat(text_window7,'buttons. The graph shows a bar graph representing the');
    text_window7 = strvcat(text_window7,'period histogram based on the analysis frequency, at');
    text_window7 = strvcat(text_window7,'the top of the histogram in red will appear the text');
    text_window7 = strvcat(text_window7,'''Sync='' and a number representing the Synchronization'); 
    text_window7 = strvcat(text_window7,'Coefficient as defined by Goldberg and Brown (1969).'); 
    text_window7 = strvcat(text_window7,'There will also be a thin dotted red line over the graph');
    text_window7 = strvcat(text_window7,'showing a single cycle of the analysis frequency.   ');
    text_window7 = strvcat(text_window7,'                                  '); 
    text_window7 = strvcat(text_window7,' To see example of an ISI histogram click the next button');
    text_window7 = strvcat(text_window7,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window7)
    
    figure(SPIKE_DEMO.demo_window)     
    
 elseif task_number == 7
    
   text_window8 =                      '     Single Fiber Analysis Tools: ISI Histogram';
   text_window8 = strvcat(text_window8,'                                                      ');
   text_window8 = strvcat(text_window8,' After a single fiber is selected, click on the circular');
   text_window8 = strvcat(text_window8,'radio button marked: ''ISI Histogram''. Once the button');
   text_window8 = strvcat(text_window8,'is clicked, the small graph below the radio buttons will');
   text_window8 = strvcat(text_window8,'change to show an Inter spike interval (ISI) histogram.');    
   text_window8 = strvcat(text_window8,'At the top of the histogram in red will appear the text');  
   text_window8 = strvcat(text_window8,'''Bin Size ='' and a number in milliseconds representing');
   text_window8 = strvcat(text_window8,'the bin size used to generate the histogram. ');
   text_window8 = strvcat(text_window8,'');                                              
   text_window8 = strvcat(text_window8,'                                  '); 
   text_window8 = strvcat(text_window8,' To see how to adjust bin size click the next button');
   text_window8 = strvcat(text_window8,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window8)
    
    figure(SPIKE_DEMO.demo_window)     
 
 elseif task_number == 8
    
   text_window9 =                      '     Single Fiber Analysis: Adjusting Bin Size';
   text_window9 = strvcat(text_window9,'                                                      ');
   text_window9 = strvcat(text_window9,' After an analysis optionis selected a small slider  ');
   text_window9 = strvcat(text_window9,'will appear below the small analysis graph marked as');
   text_window9 = strvcat(text_window9,'''Change Bin Size''. This tool is used to adjust the');
   text_window9 = strvcat(text_window9,'bin size used to generate both the Period and ISI');    
   text_window9 = strvcat(text_window9,'histograms. Moving the slider to the left will increase');  
   text_window9 = strvcat(text_window9,'the bin size, to the right will decrease the bin size.');
   text_window9 = strvcat(text_window9,'This tool can be useful in matching simulation results');
   text_window9 = strvcat(text_window9,'with experimental data that may have been binned at');                                              
   text_window9 = strvcat(text_window9,'different rates and examining the effects of binning');
   text_window9 = strvcat(text_window9,'on analysis. NOTE: for fine adjust of bin size the small');
   text_window9 = strvcat(text_window9,'rectangles at either end of the slider can be used to'); 
   text_window9 = strvcat(text_window9,'step through small changes in bin size.              ');
   text_window9 = strvcat(text_window9,'                                  ');
   text_window9 = strvcat(text_window9,' For general spike viewing tips click the next button');
   text_window9 = strvcat(text_window9,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window9)
    
    figure(SPIKE_DEMO.demo_window)   
    
   elseif task_number == 9
    
         set(SPIKE_DEMO.gui(2),'string','CLOSE','backgroundcolor',[.7 .15 .15],'foregroundcolor',[1 1 1])  
       
   text_window10 =                      '                     Spike Viewing Tips';
   text_window10 = strvcat(text_window10,'                                                      ');
   text_window10 = strvcat(text_window10,'  The spike display window in Matlab can be resized for');
   text_window10 = strvcat(text_window10,'different screen resolutions, the buttons and tools ');
   text_window10 = strvcat(text_window10,'will also scale. Take note, making windows smaller may');
   text_window10 = strvcat(text_window10,'cause some GUI tools to disappear or not function. ');    
   text_window10 = strvcat(text_window10,'Expanding windows generally does not effect gui tools.');  
   text_window10 = strvcat(text_window10,'When using sliders, grasp the slider button for gross ');
   text_window10 = strvcat(text_window10,'changes, use the slider edge buttons for finer ');
   text_window10 = strvcat(text_window10,'adjustments. The analysis tools will maintain previous');                                              
   text_window10 = strvcat(text_window10,'state, once a binsize and analysis frequency is selected');
   text_window10 = strvcat(text_window10,'the view can be switched from fiber to fiber without');
   text_window10 = strvcat(text_window10,'readjusting analysis parameters. When using Matlab'); 
   text_window10 = strvcat(text_window10,'where the are many windows open and large numbers of');
   text_window10 = strvcat(text_window10,'variables in the workspace, it is possible the graphics');
   text_window10 = strvcat(text_window10,'will enter odd state, if this happens close the application');
   text_window10 = strvcat(text_window10,'and reopen it to alleviate the problem.                    ');
   text_window10 = strvcat(text_window10,'                                  ');
   text_window10 = strvcat(text_window10,' To finish the demo and leave the Viewer open');
    text_window10 = strvcat(text_window10,'               click the Close button                ');  
   text_window10 = strvcat(text_window10,'                                  ');
    
    set(SPIKE_DEMO.gui(1),'string',text_window10)
    
    figure(SPIKE_DEMO.demo_window) 
elseif task_number == 10
    close(SPIKE_DEMO.demo_window)
    figure(SDT.main)
end