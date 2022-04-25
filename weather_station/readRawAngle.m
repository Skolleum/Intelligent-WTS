function degAngle = ReadRawAngle(devi)

%7:0 - bits
% devi.beginTransmission(0x36); % connect to the sensor
writeRegister(devi, 0x0D, 'uint8'); % figure 21 - register map: Raw angle (7:0)
% devi.endTransmission(); % end transmission
% devi.requestFrom(0x36, 1); % request from the sensor

% while devi.available() == 0  % wait until it becomes available
lowbyte = dec2bin(readRegister(devi, 0x0D,'uint8')); % Reading the data after the request
% disp("Low Byte: ");
% disp(lowbyte); % //absolute position of the encoder within the 0-360 circle

% 11:8 - 4 bits
% devi.beginTransmission(0x36);
writeRegister(devi, 0x0C, 'uint8'); % figure 21 - register map: Raw angle (11:8)
% devi.endTransmission();
% devi.requestFrom(0x36, 1);

% while devi.available() == 0
highbyte = dec2bin(readRegister(devi, 0x0C,'uint8'));
% disp("High Byte: ");
% disp(highbyte); % //absolute position of the encoder within the 0-360 circle

% 4 bits have to be shifted to its proper place as we want to build a 12-bit number
% highbyte = highbyte << 8; //shifting to left
% MATLAB version: c = bitsll(a, k)
% highbyte = bitshift(highbyte, 8); % shifting to left
% disp("Shifted high byte: ");
% disp(highbyte); % //absolute position of the encoder within the 0-360 circle

% What is happening here is the following: The variable is being shifted by 8 bits to the left:
% Initial value: 00000000|00001111 (word = 16 bits or 2 bytes)
% Left shifting by eight bits: 00001111|00000000 so, the high byte is filled in

% Finally, we combine (bitwise OR) the two numbers:
%High: 00001111|00000000
%Low:  00000000|00001111

% H|L:  00001111|00001111
rawAngle = [highbyte lowbyte] ; % int is 16 bits (as well as the word)
% disp("Raw angle: ");
% disp(rawAngle); % //absolute position of the encoder within the 0-360 circle

% We need to calculate the angle:
% 12 bit -> 4096 different levels: 360° is divided into 4096 equal parts:
% 360/4096 = 0.087890625
%Multiply the output of the encoder with 0.087890625
degAngle = bin2dec(rawAngle) * 0.087890625;

% disp("Deg angle: ");
% disp(degAngle); % //absolute position of the encoder within the 0-360 circle

end

