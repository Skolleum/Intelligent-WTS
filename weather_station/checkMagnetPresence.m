function checkMagnetPresence(devi, magnetStatus)
%This function runs in the setup() and it locks the MCU until the magnet is not positioned properly

while magnetStatus <= 32 %while the magnet is not adjusted to the proper distance - 32: MD = 1

    magnetStatus = 0; %reset reading
    % devi.beginTransmission(0x36); %connect to the sensor
    write(devi, 0x0B, 'uint8') %figure 21 - register map: Status: MD ML MH
    % devi.endTransmission(); %end transmission
    % devi.requestFrom(0x36, 1); %request from the sensor

    %while devi.available() == 0 % wait until it becomes available
    %Reading the data after the request
    magnetStatus = readRegister(devi, 0x0B, 'uint8');

    disp("Magnet status: ");
    disp(dec2bin(magnetStatus)); %print it in binary so you can compare it to the table (fig 21)
    %end

    %Status register output: 0 0 MD ML MH 0 0 0
    %MH: Too strong magnet - 100111 - DEC: 39
    %ML: Too weak magnet - 10111 - DEC: 23
    %MD: OK magnet - 110111 - DEC: 55

end
    disp("Magnet found!");
    pause(2);
end
