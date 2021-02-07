function logs = parse_log_folder()
    logList = dir(fullfile(pwd, '*.log'));
    fileNames = {};
    data = {};

    for i = 1:size(logList, 1)
        fileNames = [fileNames logList(i).name];
        data = [data log_parse(logList(i).name)];
    end
    
    logs = struct('filename', fileNames, 'data', data);

end