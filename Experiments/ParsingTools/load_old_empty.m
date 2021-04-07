function [out] = load_old_empty()
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
cd('empty_old')
out = parse_log_folder_json();
cd ..
end

