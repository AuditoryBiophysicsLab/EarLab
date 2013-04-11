fp = fopen('Left_ANSpike1.binary', 'r');
fseek(fp, 0, 'eof');
flen = ftell(fp);
fseek(fp, 0, 'bof');
spikecount = flen / 12
clear cellid_Left_ANSpike1;
clear time_Left_ANSpike1;
for i = 1:spikecount
    cellid_Left_ANSpike1(i) = fread(fp, 1, 'int');
    time_Left_ANSpike1(i) = fread(fp, 1, 'double');
end
fclose(fp);
% make columns
cellid_Left_ANSpike1 = cellid_Left_ANSpike1';
time_Left_ANSpike1 = time_Left_ANSpike1';
% figure; plot(time_Left_ANSpike1, cellid_Left_ANSpike1, '.');

fp = fopen('Right_ANSpike1.binary', 'r');
fseek(fp, 0, 'eof');
flen = ftell(fp);
fseek(fp, 0, 'bof');
spikecount = flen / 12
clear cellid_Right_ANSpike1;
clear time_Right_ANSpike1;
for i = 1:spikecount
    cellid_Right_ANSpike1(i) = fread(fp, 1, 'int');
    time_Right_ANSpike1(i) = fread(fp, 1, 'double');
end
fclose(fp);
% make columns
cellid_Right_ANSpike1 = cellid_Right_ANSpike1';
time_Right_ANSpike1 = time_Right_ANSpike1';
% figure; plot(time_Right_ANSpike1, cellid_Right_ANSpike1, '.');

% fp = fopen('Left_Bushy1.binary', 'r');
% fseek(fp, 0, 'eof');
% flen = ftell(fp);
% fseek(fp, 0, 'bof');
% spikecount = flen / 12
% clear cellid_Left_Bushy1;
% clear time_Left_Bushy1;
% for i = 1:spikecount
%     cellid_Left_Bushy1(i) = fread(fp, 1, 'int');
%     time_Left_Bushy1(i) = fread(fp, 1, 'double');
% end
% fclose(fp);
% % make columns
% cellid_Left_Bushy1 = cellid_Left_Bushy1';
% time_Left_Bushy1 = time_Left_Bushy1';
% % figure; plot(time_Left_Bushy1, cellid_Left_Bushy1, '.');
% 
% 
% fp = fopen('Right_Bushy1.binary', 'r');
% fseek(fp, 0, 'eof');
% flen = ftell(fp);
% fseek(fp, 0, 'bof');
% spikecount = flen / 12
% clear cellid_Right_Bushy1;
% clear time_Right_Bushy1;
% for i = 1:spikecount
%     cellid_Right_Bushy1(i) = fread(fp, 1, 'int');
%     time_Right_Bushy1(i) = fread(fp, 1, 'double');
% end
% fclose(fp);
% % make columns
% cellid_Right_Bushy1 = cellid_Right_Bushy1';
% time_Right_Bushy1 = time_Right_Bushy1';
% % figure; plot(time_Right_Bushy1, cellid_Right_Bushy1, '.');



fp = fopen('Left_MSO_MSOAN1.binary', 'r');
fseek(fp, 0, 'eof');
flen = ftell(fp);
fseek(fp, 0, 'bof');
spikecount = flen / 12
clear cellid_Left_MSO_MSOAN1;
clear time_Left_MSO_MSOAN1;
for i = 1:spikecount
    cellid_Left_MSO_MSOAN1(i) = fread(fp, 1, 'int');
    time_Left_MSO_MSOAN1(i) = fread(fp, 1, 'double');
end
fclose(fp);
cellid_Left_MSO_MSOAN1 = cellid_Left_MSO_MSOAN1';
time_Left_MSO_MSOAN1 = time_Left_MSO_MSOAN1';
% figure; plot(time_Left_MSO_MSOAN1, cellid_Left_MSO_MSOAN1, '.');


fp = fopen('Right_MSO_MSOAN1.binary', 'r');
fseek(fp, 0, 'eof');
flen = ftell(fp);
fseek(fp, 0, 'bof');
spikecount = flen / 12
clear cellid_Right_MSO_MSOAN1;
clear time_Right_MSO_MSOAN1;
for i = 1:spikecount
    cellid_Right_MSO_MSOAN1(i) = fread(fp, 1, 'int');
    time_Right_MSO_MSOAN1(i) = fread(fp, 1, 'double');
end
fclose(fp);
cellid_Right_MSO_MSOAN1 = cellid_Right_MSO_MSOAN1';
time_Right_MSO_MSOAN1 = time_Right_MSO_MSOAN1';
% figure; plot(time_Right_MSO_MSOAN1, cellid_Right_MSO_MSOAN1, '.');



[timesorted_LeftMSO,timesortIdx_LeftMSO] = sort(time_Left_MSO_MSOAN1);
cellid_sortedbytime_LeftMSO = cellid_Left_MSO_MSOAN1(timesortIdx_LeftMSO);


cellid0idc = find(cellid_sortedbytime_LeftMSO==0);
times_cellid0 = timesorted_LeftMSO(cellid0idc);
cellid01idc = find(cellid_sortedbytime_LeftMSO==1);
times_cellid01 = timesorted_LeftMSO(cellid01idc);
cellid02idc = find(cellid_sortedbytime_LeftMSO==2);
times_cellid02 = timesorted_LeftMSO(cellid02idc);
cellid03idc = find(cellid_sortedbytime_LeftMSO==3);
times_cellid03 = timesorted_LeftMSO(cellid03idc);
cellid04idc = find(cellid_sortedbytime_LeftMSO==4);
times_cellid04 = timesorted_LeftMSO(cellid04idc);
cellid05idc = find(cellid_sortedbytime_LeftMSO==5);
times_cellid05 = timesorted_LeftMSO(cellid05idc);
cellid06idc = find(cellid_sortedbytime_LeftMSO==6);
times_cellid06 = timesorted_LeftMSO(cellid06idc);
cellid07idc = find(cellid_sortedbytime_LeftMSO==7);
times_cellid07 = timesorted_LeftMSO(cellid07idc);
cellid08idc = find(cellid_sortedbytime_LeftMSO==8);
times_cellid08 = timesorted_LeftMSO(cellid08idc);
cellid09idc = find(cellid_sortedbytime_LeftMSO==9);
times_cellid09 = timesorted_LeftMSO(cellid09idc);
cellid10idc = find(cellid_sortedbytime_LeftMSO==10);
times_cellid10 = timesorted_LeftMSO(cellid10idc);
cellid11idc = find(cellid_sortedbytime_LeftMSO==11);
times_cellid11 = timesorted_LeftMSO(cellid11idc);
cellid12idc = find(cellid_sortedbytime_LeftMSO==12);
times_cellid12 = timesorted_LeftMSO(cellid12idc);
cellid13idc = find(cellid_sortedbytime_LeftMSO==13);
times_cellid13 = timesorted_LeftMSO(cellid13idc);
cellid14idc = find(cellid_sortedbytime_LeftMSO==14);
times_cellid14 = timesorted_LeftMSO(cellid14idc);
cellid15idc = find(cellid_sortedbytime_LeftMSO==15);
times_cellid15 = timesorted_LeftMSO(cellid15idc);
cellid16idc = find(cellid_sortedbytime_LeftMSO==16);
times_cellid16 = timesorted_LeftMSO(cellid16idc);
cellid17idc = find(cellid_sortedbytime_LeftMSO==17);
times_cellid17 = timesorted_LeftMSO(cellid17idc);
cellid18idc = find(cellid_sortedbytime_LeftMSO==18);
times_cellid18 = timesorted_LeftMSO(cellid18idc);
cellid19idc = find(cellid_sortedbytime_LeftMSO==19);
times_cellid19 = timesorted_LeftMSO(cellid19idc);
cellid20idc = find(cellid_sortedbytime_LeftMSO==20);
times_cellid20 = timesorted_LeftMSO(cellid20idc);
% cellid21idc = find(cellid_sortedbytime_LeftMSO==21);
% times_cellid21 = timesorted_LeftMSO(cellid21idc);


[timesorted_RightMSO,timesortIdx_RightMSO] = sort(time_Right_MSO_MSOAN1);
cellid_sortedbytime_RightMSO = cellid_Right_MSO_MSOAN1(timesortIdx_RightMSO);

rcellid0idc = find(cellid_sortedbytime_RightMSO==0);
times_rcellid0 = timesorted_RightMSO(rcellid0idc);
rcellid01idc = find(cellid_sortedbytime_RightMSO==1);
times_rcellid01 = timesorted_RightMSO(rcellid01idc);
rcellid02idc = find(cellid_sortedbytime_RightMSO==2);
times_rcellid02 = timesorted_RightMSO(rcellid02idc);
rcellid03idc = find(cellid_sortedbytime_RightMSO==3);
times_rcellid03 = timesorted_RightMSO(rcellid03idc);
rcellid04idc = find(cellid_sortedbytime_RightMSO==4);
times_rcellid04 = timesorted_RightMSO(rcellid04idc);
rcellid05idc = find(cellid_sortedbytime_RightMSO==5);
times_rcellid05 = timesorted_RightMSO(rcellid05idc);
rcellid06idc = find(cellid_sortedbytime_RightMSO==6);
times_rcellid06 = timesorted_RightMSO(rcellid06idc);
rcellid07idc = find(cellid_sortedbytime_RightMSO==7);
times_rcellid07 = timesorted_RightMSO(rcellid07idc);
rcellid08idc = find(cellid_sortedbytime_RightMSO==8);
times_rcellid08 = timesorted_RightMSO(rcellid08idc);
rcellid09idc = find(cellid_sortedbytime_RightMSO==9);
times_rcellid09 = timesorted_RightMSO(rcellid09idc);
rcellid10idc = find(cellid_sortedbytime_RightMSO==10);
times_rcellid10 = timesorted_RightMSO(rcellid10idc);
rcellid11idc = find(cellid_sortedbytime_RightMSO==11);
times_rcellid11 = timesorted_RightMSO(rcellid11idc);
rcellid12idc = find(cellid_sortedbytime_RightMSO==12);
times_rcellid12 = timesorted_RightMSO(rcellid12idc);
rcellid13idc = find(cellid_sortedbytime_RightMSO==13);
times_rcellid13 = timesorted_RightMSO(rcellid13idc);
rcellid14idc = find(cellid_sortedbytime_RightMSO==14);
times_rcellid14 = timesorted_RightMSO(rcellid14idc);
rcellid15idc = find(cellid_sortedbytime_RightMSO==15);
times_rcellid15 = timesorted_RightMSO(rcellid15idc);
rcellid16idc = find(cellid_sortedbytime_RightMSO==16);
times_rcellid16 = timesorted_RightMSO(rcellid16idc);
rcellid17idc = find(cellid_sortedbytime_RightMSO==17);
times_rcellid17 = timesorted_RightMSO(rcellid17idc);
rcellid18idc = find(cellid_sortedbytime_RightMSO==18);
times_rcellid18 = timesorted_RightMSO(rcellid18idc);
rcellid19idc = find(cellid_sortedbytime_RightMSO==19);
times_rcellid19 = timesorted_RightMSO(rcellid19idc);
rcellid20idc = find(cellid_sortedbytime_RightMSO==20);
times_rcellid20 = timesorted_RightMSO(rcellid20idc);
% rcellid21idc = find(cellid_sortedbytime_RightMSO==21);
% times_rcellid21 = timesorted_RightMSO(rcellid21idc);

nlags = 21;
% generate rate-ITD, sync-ITD curves
rate_curve_left1 = zeros(nlags,1);
sync_curve_left1 = zeros(nlags,2);
rate_curve_right1 = zeros(nlags,1);
sync_curve_right1 = zeros(nlags,2);
duration = 3;
f = 528.72;
T = 1/f;
delay_axis = 1000*(-T:T/(nlags-1):T)';
% delay_axis
% delay_axis_right

for m = 0:(nlags-1)
    if (m<10 & m>0)
        a = '0';
    else
        a = '';
    end
    rate_curve_left1(m+1) = eval(['length(times_cellid' a num2str(m) ')'])/duration;
    [sync_curve_left1(m+1,1),sync_curve_left1(m+1,2)] = eval(['synccalc(times_cellid' a num2str(m) ',1/f)']);  
    rate_curve_right1(m+1) = eval(['length(times_rcellid' a num2str(m) ')'])/duration;
    [sync_curve_right1(m+1,1),sync_curve_right1(m+1,2)] = eval(['synccalc(times_rcellid' a num2str(m) ',1/f)']);
end

% rate_curve_left1 = rate_curve_left1(2:end);
% sync_curve_left1 = sync_curve_left1(2:end,:);
lrcl = length(rate_curve_left1);
lrcr = length(rate_curve_right1);

% figure;plot(delay_axis,[rate_curve_left1(lrcl+1 - (1:lrcl));rate_curve_right1])
figure;plot(delay_axis,[rate_curve_left1; rate_curve_right1(2:end)])
title('left and right model MSO cells - Brughera96, inputs Meddis AN 20/105/0.7')
ylabel('Discharge Rate (spikes/sec)')
xlabel('internal delay (ms)')
axis([-2 2 0 250])

% present valid point in sync plot
valid_SI_idx_left1 = find(sync_curve_left1(:,2)>13.8)
valid_SI_idx_right1 = find(sync_curve_right1(2:end,2)>13.8)+1
valid_SI_axis_left1 =  lrcl+1 - flipud(valid_SI_idx_left1)
valid_SI_axis_right1 = lrcr-1 + valid_SI_idx_right1
xp = [delay_axis(valid_SI_axis_left1); delay_axis(valid_SI_axis_right1)]
yp = [sync_curve_left1(valid_SI_idx_left1);sync_curve_right1(valid_SI_idx_right1)]
figure;plot(xp,yp,'+')
% axis([-T T 0 1])
title('left and right model MSO cells - Brughera96, inputs Meddis AN 20/105/0.7')
ylabel('synchronization index')
xlabel('internal delay (ms)')
axis([-2 2 0 1])
