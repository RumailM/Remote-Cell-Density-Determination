function [log] = log_parse_json(file_name)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

%%
% file_name= "MF_0.5_200_rattle.txt"
% disp(file_name);
fid=fopen(file_name);
tlines = cell(0,1);
tline = fgetl(fid);
while ischar(tline)
    tlines{end+1,1} = tline;
    tline = fgetl(fid);
end
fclose(fid);

start_index = [];


%%
out = struct("x415nm",[],"x445nm",[],"x480nm",[],...
    "x515nm",[],"x555nm",[],"x590nm",[], ...
    "x630nm",[], "x680nm",[],"Clear",[], ...
    "NIR",[],"time",[],"gain",[]);

%%
init_time = 0;
for i = 1:numel(tlines)
    row = jsondecode(char(tlines{i,1}));
    if i == 1
        init_time = row.TIME/1000;
        out.time = [out.time 0];
    else
        out.time = [out.time row.TIME/1000-init_time];
    end
    
    
   
    out.x555nm = [out.x555nm row.DATA(1)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))];
    out.x590nm = [out.x590nm row.DATA(2)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))];
    out.x630nm = [out.x630nm row.DATA(3)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))];
    out.x680nm = [out.x680nm row.DATA(4)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))];
    out.Clear = [out.Clear row.DATA(5)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))]; 
    out.NIR = [out.NIR row.DATA(6)/(2^(row.GAIN-1) *0.00278*(row.ATIME+1)*(row.ASTEP+1))]; 
    out.gain = [out.gain row.GAIN];
             
%     out.x415nm = [out.x415nm row.readings(1)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x445nm = [out.x445nm row.readings(2)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x480nm = [out.x480nm row.readings(3)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x515nm = [out.x515nm row.readings(4)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x555nm = [out.x555nm row.readings(5)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x590nm = [out.x590nm row.readings(6)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x630nm = [out.x630nm row.readings(9)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.x680nm = [out.x680nm row.readings(10)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];
%     out.Clear = [out.Clear row.readings(7)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))]; 
%     out.NIR = [out.NIR row.readings(8)/(2^(row.gain-1) *0.00278*(row.atime+1)*(row.astep+1))];  
         
   
       
    
    
end

%%
log = out;
end

