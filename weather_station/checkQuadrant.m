function [numberofTurns, totalAngle, previousquadrantNumber] = checkQuadrant(correctedAngle, previousquadrantNumber, numberofTurns)
%
%   %Quadrants:
%   4  |  1
%   ---|---
%   3  |  2
%

% Quadrant 1
if correctedAngle >= 0 && correctedAngle <=90
    quadrantNumber = 1;
end

% %Quadrant 2
if correctedAngle > 90 && correctedAngle <=180
    quadrantNumber = 2;
    
end

% Quadrant 3
if(correctedAngle > 180 && correctedAngle <=270)
    quadrantNumber = 3;
end

% Quadrant 4
if(correctedAngle > 270 && correctedAngle <360)
    quadrantNumber = 4;
end

% disp("Quadrant: ");
% disp(quadrantNumber); % print our position "quadrant-wise"

if quadrantNumber ~= previousquadrantNumber %if we changed quadrant

    if(quadrantNumber == 1 && previousquadrantNumber == 4)

        numberofTurns = numberofTurns + 1; % 4 --> 1 transition: CW rotation
    end

    if(quadrantNumber == 4 && previousquadrantNumber == 1)

        numberofTurns = numberofTurns - 1;  % 1 --> 4 transition: CCW rotation
    end
    %this could be done between every quadrants so one can count every 1/4th of transition

previousquadrantNumber = quadrantNumber;  %update to the current quadrant
end
disp("Turns: ");
disp(numberofTurns); %number of turns in absolute terms (can be negative which indicates CCW turns)

%after we have the corrected angle and the turns, we can calculate the total absolute position
totalAngle = (numberofTurns*360) + correctedAngle; %number of turns (+/-) plus the actual angle within the 0-360 range
disp("Total angle: ");
disp(totalAngle); %absolute position of the motor expressed in degree angles, 2 digits
end