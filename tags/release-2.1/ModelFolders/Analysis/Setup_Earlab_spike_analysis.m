function Setup_Earlab_spike_analysis
global SDT

 ! set > echo

 FID =fopen('echo','r');
 
 line_env = '';
 while (~feof(FID))
   line_env = strvcat(line_env, fgetl(FID));
 end
 
 for i = 1:length(line_env)
    start = findstr(line_env(i,:),'USERNAME=');
    if start >= 1
        username = strrep(line_env(i,:), 'USERNAME=', '');
        break;
    end
 end
 
 present_path = pwd;
 
%  cd ..
% %  start_path = pwd;
% %  pathset = genpath(start_path);
% %  addpath(pathset);
% %  cd(start_path);

 fclose(FID);
 
 SDT.path_name = strcat('C:\Documents and Settings\',username,'\My Documents\EarLab\Output\CN output\');
 SDT.input_path =strcat('C:\Documents and Settings\',username,'\My Documents\EarLab\Signals\Tones\');
 %%%%%%%%%%%%%%% Set basic path information %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 SDT.filename ='CNSpike.out';
 SDT.input_filename = '500_Hz.txt';
 
%  SDT.path_name = which(SDT.filename);
%  SDT.path_name = strrep(SDT.path_name,SDT.filename,'');
%  SDT.path_name =deblank(SDT.path_name);
 
%  SDT.input_path =which(SDT.input_filename);
%  SDT.input_path = strrep(SDT.input_path,SDT.input_filename,'');
% SDT.input_path =deblank(SDT.input_path);


