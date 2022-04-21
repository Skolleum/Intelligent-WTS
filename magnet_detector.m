% Create ThingSpeak account:
%       Sign in MATLAB onto your browser with school e-mail.
%       Then, navigate thingspeak.com, it will be signing in automatically.
% Click new channel
% Enter desired sensor name, description etc.
% Regard channel credentials here.
% Here is an example with PIR motion sensor for writing to channel.

% Clear all connection
clear a

% Create connection
a = arduino('COM5', 'uno');

% Define pins
LED = 'D2';
OUT = 'D3';

% Count number of occurrences
ct = 0;

% Test LED connection.
for i=1:5
    writeDigitalPin(a, LED, 0);
    pause(0.1);
    writeDigitalPin(a, LED, 1);
    pause(0.1);
end

tStart = tic;   
% Infitine loop
while true
    % Form data to upload
    % Calculate speed
    detected = readDigitalPin(a, OUT);
    if detected == 0
        writeDigitalPin(a, LED, 0);
        pause(0.3);
        writeDigitalPin(a, LED, 1);
        pause(0.3);
        ct = ct +1;
    end
    tEnd = toc(tStart);
    if tEnd >=60
        thingSpeakWrite(1693575, ct,'Fields', 3, 'WriteKey','FE03OXDEU2ELEJON')
        % Limit for free licence in ThingSpeak is 15 seconds, so put at least
        % 15 sec pause.
        "rest for 15 secs..."
        pause(15);
    end
end


