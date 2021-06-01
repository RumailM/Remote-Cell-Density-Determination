function logs = parse_log_folder_json(folder_name)
%%
    cd(folder_name)
    logList = dir(fullfile(pwd, '*.txt'));
    fileNames = {};
    data = {};
  %% 
%%
    for i = 1:size(logList, 1)
        fileNames = [fileNames logList(i).name];
        data = [data log_parse_json(logList(i).name)];
    end
    
    logs = struct('filename', fileNames, 'data', data);
    cd ..
end

