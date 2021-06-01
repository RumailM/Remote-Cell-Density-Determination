%%
addpath("ParsingTools")
%%
tests = parse_log_folder_json("Tests_7_April_2021")
%%
means = [];
for i=1:numel(tests)
    times = tests(i).data.time;
    y = zeros(1,numel(times));


    for i = numel(times):-1:2
        y(i) = times(i) - times(i-1);
    end

    average_period = mean(y);
    means = [means average_period];
end
%%
time_between_samples = mean(means);
%%
num_cycles = 5;
window_static = 10;
seconds_per_revolution_100 =    60/100;
window_100 = ceil(num_cycles*(seconds_per_revolution_100/time_between_samples));
seconds_per_revolution_200 =    60/200;
window_200 = ceil(num_cycles*(seconds_per_revolution_200/time_between_samples));
seconds_per_revolution_300 =    60/300;
window_300 = ceil(num_cycles*(seconds_per_revolution_300/time_between_samples));
num_levels_rpms = 4;
num_od_levels = 8;

% labels = ["0.0 Lower","0.0","0.0 Upper",...
%     "0.5 Lower","0.5","0.5 Upper",...
%     "1.0 Lower","1.0","1.0 Upper",...
%     "2.0 Lower","2.0","2.0 Upper",...
%     "3.0 Lower","3.0","3.0 Upper",...
%     "4.0 Lower","4.0","4.0 Upper",...
%     "5.0 Lower","5.0","5.0 Upper",...
%     "6.0 Lower","6.0","6.0 Upper"];

labels = ["0.0","0.5","1.0","2.0","3.0","4.0","5.0","6.0"]
%%  Plot Moving Means
figure(1)
sgtitle("Moving Means, 5 cycles")

for i =1:num_levels_rpms
    subplot(2,2,i)
    hold on
    if i == 1
        window = window_static;
        title("Static");
    elseif i == 2
        window = window_100;
        title("100RPM");
    elseif i == 3
        window = window_200;
        title("200RPM");
    else
        window = window_300;
        title("300RPM");
    end
    xlabel("Time[s]")
    pvec = [];
    for j=1:num_od_levels
        if j == 1
            color = '#0072BD';
        elseif j == 2
            color ='#D95319';
        elseif j == 3  
            color = '#FF0000';
        elseif j == 4
            color = '#7E2F8E';
        elseif j == 5
            color = '#77AC30';
        elseif j == 6
            color = '#4DBEEE';
        elseif j == 7
            color ='#A2142F';
        else
            color = '#EDB120';
        end
        graph_id = (j-1)*4 + i;
        values = movmean(tests(graph_id).data.x630nm,window);
        prctiles = prctile(values,[2.5,97.5]);
        plot([0 60], [prctiles(1) prctiles(1)],"--",'Color',color,'LineWidth',1.0);
        pl = plot(tests(graph_id).data.time,values,'Color',color,'LineWidth',1.0);
        plot([0 60], [prctiles(2) prctiles(2)],"--",'Color',color,'LineWidth',1.0);
        pvec = [pvec pl];
    end
    legend([pvec],labels);
    hold off
end
%% Plot Time Series
figure(2)
sgtitle("Time Series")

for i =1:num_levels_rpms
    subplot(2,2,i)
    hold on
    if i == 1
        window = window_static;
        title("Static, no move mean");
    elseif i == 2
        window = window_100;
        title("100RPM, no move mean");
    elseif i == 3
        window = window_200;
        title("200RPM, no move mean");
    else
        window = window_300;
        title("300RPM, no move mean");
    end
    xlabel("Time[s]")
    pvec = [];
    for j=1:num_od_levels
        if j == 1
            color = '#0072BD';
        elseif j == 2
            color ='#D95319';
        elseif j == 3  
            color = '#FF0000';
        elseif j == 4
            color = '#7E2F8E';
        elseif j == 5
            color = '#77AC30';
        elseif j == 6
            color = '#4DBEEE';
        elseif j == 7
            color ='#A2142F';
        else
            color = '#EDB120';
        end
        graph_id = (j-1)*4 + i;
        values = tests(graph_id).data.x630nm;
        prctiles = prctile(values,[2.5,97.5]);
        plot([0 60], [prctiles(1) prctiles(1)],"--",'Color',color,'LineWidth',1.0);
        pl = plot(tests(graph_id).data.time,values,'Color',color,'LineWidth',1.0);
        plot([0 60], [prctiles(2) prctiles(2)],"--",'Color',color,'LineWidth',1.0);
        pvec = [pvec pl];
    end
    legend(labels);
    hold off
end




