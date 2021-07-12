%% Plotting from parse_log_folder_json

for i = 1:2
    figure();
    plot(tests(i).data.time, tests(i).data.Clear);
%     hold on;
    grid on;
    title(tests(i).filename);
    ylim([min(tests(i).data.Clear)-0.01 max(tests(i).data.Clear)+0.01]);
%     xlim([0 60]);
    legend(sprintf('%f', mean(tests(i).data.Clear)));
%     saveas(gcf, strcat(tests(i).filename, ".png"));
end