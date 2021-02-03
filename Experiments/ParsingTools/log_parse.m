function [log] = log_parse(file_name)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here



fid=fopen(file_name);
tline = fgetl(fid);
tlines = cell(0,1);
while ischar(tline)
    tlines{end+1,1} = tline;
    tline = fgetl(fid);
end
fclose(fid);

start_index = [];


for i = 1:numel(tlines)
    found = strfind(tlines{i,1},"START");
    if  ~isempty(found)
        start_index = i;
        break
    end       
end

tlines = tlines(start_index+1:end,:);
%%
out = struct("x415nm",[],"x415nm_time",[],"x445nm",[],"x445nm_time",[],"x480nm",[],"x480nm_time",[],...
    "x515nm",[],"x515nm_time",[],"x555nm",[],"x555nm_time",[],"x590nm",[],"x590nm_time",[], ...
    "x630nm",[],"x630nm_time",[], "x680nm",[],"x680nm_time",[],"Clear",[],"Clear_time",[], ...
    "NIR",[],"NIR_time",[],"Gain",[],"Gain_time",[],"ATime",[],"ATime_time",[],...
    "AStep",[],"AStep_time",[],"SpIntEn",[],"SpIntEn_time",[],"SpAGCEn",[],"SpAGCEn_time",[],...
    "SpAGCLowTh",[],"SpAGCLowTh_time",[], "SpAGCHighTh",[],"SpAGCHighTh_time",[], ...
    "IntTime",[],"IntTime_time",[]);



for i = 1:numel(tlines)
    row = tlines{i,1};
    time = row(1:12);
    row = row(16:end);
    if strfind(row,"415nm")
        out.x415nm = [out.x415nm retVal(row)];
        out.x415nm_time = [out.x415nm_time ;time];
    elseif strfind(row,"445nm")
        out.x445nm = [out.x445nm retVal(row)];
        out.x445nm_time = [out.x445nm_time ;time];
    elseif strfind(row,"480nm")
        out.x480nm = [out.x480nm retVal(row)];
        out.x480nm_time = [out.x480nm_time ;time];
    elseif strfind(row,"515nm")
        out.x515nm = [out.x515nm retVal(row)];
        out.x515nm_time = [out.x515nm_time ;time];
    elseif strfind(row,"555nm")
        out.x555nm = [out.x555nm retVal(row)];
        out.x555nm_time = [out.x555nm_time ;time];
    elseif strfind(row,"590nm")
        out.x590nm = [out.x590nm retVal(row)];
        out.x590nm_time = [out.x590nm_time ;time];
    elseif strfind(row,"630nm")
        out.x630nm = [out.x630nm retVal(row)];
        out.x630nm_time = [out.x630nm_time ;time];
    elseif strfind(row,"680nm")
        out.x680nm = [out.x680nm retVal(row)];
        out.x680nm_time = [out.x680nm_time ;time];
    elseif strfind(row,"Clear")
        out.Clear = [out.Clear retVal(row)];
        out.Clear_time = [out.Clear_time ;time];
    elseif strfind(row,"NIR")
        out.NIR = [out.NIR retVal(row)];
        out.NIR_time = [out.NIR_time ;time];
    elseif strfind(row,"Gain")
        out.Gain = [out.Gain retVal(row)];
        out.Gain_time = [out.Gain_time ;time];
    elseif strfind(row,"ATime")
        out.ATime = [out.ATime retVal(row)];
        out.ATime_time = [out.ATime_time ;time];
    elseif strfind(row,"AStep")
        out.AStep = [out.AStep retVal(row)];
        out.AStep_time = [out.AStep_time ;time];
    elseif strfind(row,"SpIntEn")
        out.SpIntEn = [out.SpIntEn retVal(row)];
        out.SpIntEn_time = [out.SpIntEn_time ;time];
    elseif strfind(row,"SpAGCEn")
        out.SpAGCEn = [out.SpAGCEn retVal(row)];
        out.SpAGCEn_time = [out.SpAGCEn_time ;time];
    elseif strfind(row,"SpAGCLowTh")
        out.SpAGCLowTh = [out.SpAGCLowTh retVal(row)];
        out.SpAGCLowTh_time = [out.SpAGCLowTh_time ;time];
    elseif strfind(row,"SpAGCHighTh")
        out.SpAGCHighTh = [out.SpAGCHighTh retVal(row)];
        out.SpAGCHighTh_time = [out.SpAGCHighTh_time ;time];
    elseif strfind(row,"IntTime")
        out.IntTime = [out.IntTime retVal(row)];
        out.IntTime_time = [out.IntTime_time ;time];
        
    end
    
    
end
log = out
end

