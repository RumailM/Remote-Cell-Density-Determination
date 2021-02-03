function [val] = retVal(line)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

line = line(4:end);

B = regexp(line,'\d*','Match');

if numel(B) == 1
    val = str2num(cell2mat(B(1)));
else
    val = str2num(B(1)+"."+B(2));
end
end

