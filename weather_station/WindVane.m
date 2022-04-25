% Clear all connection
clear all

% Create connection
a = arduino('COM5', 'uno');

% I2C pins:
SDA = 'A4';
SCL = 'A5';

% Magnetic sensor things
magnetStatus = 0; % value of the status register (MD, ML, MH)

quadrantNumber = 0;
previousquadrantNumber = 0; % quadrant IDs
numberofTurns = 0; % number of turns
correctedAngle = 0.0; % tared angle - based on the startup value
startAngle = 0.0;% starting angle
totalAngle = 0.0; % total absolute angular displacement
previoustotalAngle = 0.0; % for the display printing

addresses = scanI2CBus(a); 

AS5600 = device(a,'I2CAddress','0x36');

% Wire.begin(); % start i2C
% AS5600.BitRate = 800000; % fast clock

checkMagnetPresence(AS5600, magnetStatus) % check the magnet (blocks until magnet is found)

degAngle = ReadRawAngle(AS5600); % reading so the degAngle gets updated
 

startAngle = degAngle; % update startAngle with degAngle - for taring
disp("Start angle: ");
disp(startAngle); % //absolute position of the encoder within the 0-360 circle

disp("Welcome!");  % print a welcome message
disp("AS5600"); % print a welcome message
pause(3);

previousquadrantNumber = 1;

while true
      degAngle = ReadRawAngle(AS5600); % ask the value from the sensor
      disp("Deg angle: ");
      disp(degAngle); % //absolute position of the encoder within the 0-360 circle
      correctedAngle = correctAngle(startAngle, degAngle); % tare the value
      numberofTurns, totalAngle, previousquadrantNumber = checkQuadrant(correctedAngle, previousquadrantNumber, numberofTurns); % check quadrant, check rotations, check absolute angular position
    pause(1); % wait a little - adjust it for "better resolution"
end
