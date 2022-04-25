function correctedAngle = correctAngle(startAngle, degAngle)

% recalculate angle
correctedAngle = degAngle - startAngle; % this tares the position

if correctedAngle < 0 % if the calculated angle is negative, we need to "normalize" it
    correctedAngle = correctedAngle + 360; % correction for negative numbers (i.e. -15 becomes +345)
    disp("Corrected angle: ");
    disp(correctedAngle); % print the corrected/tared angle
end
end