# Importing Libraries
import serial
import time
import os

arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)

startTime = time.time()
while True:
    message = arduino.readline()
    print(message)
    _ = message.split(b':')
    try:
        indicator = _[0].decode("utf-8")
        numeric = _[1].decode("utf-8")
    except Exception as e:
        indicator = 404
        numeric = str(e)

    if indicator == 'Corrected angle':
        f = open('direction.txt', 'a')
    elif indicator == 'Speed':
        f = open('speed.txt', 'a')
    elif indicator == 'RPM':
        f = open('rotorRPM.txt', 'a')
    else:
        f = open('log.txt', 'a')
    f.write(f"{time.asctime()}  : {str(numeric)}")
    f.close()

    # if time.time() - startTime >= 30:
    #     for i in range(1, 4):
    #         num = input(f"Enter a number for pitch angle {i}: ")  # Taking input from user
    #         arduino.write(bytes(num, 'utf-8'))
    #         time.sleep(0.05)
    #         f = open('pa_control.txt', 'a')
    #         f.write(f"{time.asctime()}  :   PA {i} set to {num}.")
    #         f.close()
