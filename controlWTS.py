# Importing Libraries
import serial
import time
import csv
import numpy as np
import datetime
from utils import get_scaler
import time
import joblib


def pa_fun(pa):
    return pa


arduino = serial.Serial(port='COM5', baudrate=115200, timeout=2)

startTime = time.time()
currentWindDirection = 0.0
currentWindSpeed = 0.0
features = {}
sc = get_scaler()
currentRotorRPM = 0.0
currentNacellePos = 0.0
currentPitchAngle = 5.0
numeric = None

outfile = open("data.csv", "w")
try:
    base_gp = joblib.load(f"Gaussian_model_updated_at_{time.asctime()}")
except Exception as e:
    base_gp = joblib.load("Gaussian_model")
    pass

while True:
    try:
        gp = joblib.load(f"Gaussian_model_updated_at_{time.asctime()}")
    except Exception as e:
        gp = base_gp
        pass
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
        # f = open('direction.txt', 'a')
        currentWindDirection = float(numeric)
    elif indicator == 'Speed':
        # f = open('speed.txt', 'a')
        currentWindSpeed = float(numeric)
    elif indicator == 'RotorRPM':
        # f = open('rotorRPM.txt', 'a')
        currentRotorRPM = float(numeric)
    elif indicator == 'Nacelle Pos':
        # f = open('rotorRPM.txt', 'a')
        currentNacellePos = float(numeric)
    elif indicator == 'PA':
        currentPitchAngle = float(numeric)
    else:
        f = open('log.txt', 'a')
        f.write(f"{time.asctime()}  : {str(numeric)}")
        f.close()

    # currentPitchAngle = readable
    # activePower = readable

    # dummy variables
    activePower = 1000

    newData = [np.datetime64(datetime.datetime.now()),
               activePower,
               currentNacellePos,
               currentRotorRPM,
               currentPitchAngle,
               currentWindDirection,
               currentWindSpeed,
               np.datetime64(datetime.datetime.now()).astype("float")]

    writer = csv.writer(outfile)
    writer.writerow(newData)

    features["RotorRPM"] = currentRotorRPM
    features["PitchAngle"] = currentPitchAngle
    features["WindDirection"] = currentWindDirection
    features["WindSpeed"] = currentWindSpeed
    features_tmp = features

    best_angle = currentPitchAngle
    max_power_val = 0.0
    for angle in range(0, 270):
        features_tmp['PitchAngle'] = angle
        feature_vector = np.asarray(list(features_tmp.values())).reshape(1, -1)
        test_features = sc.transform(feature_vector)
        # print(features_tmp)
        power_val = gp.predict(test_features)
        if max_power_val <= power_val:
            max_power_val = power_val
            best_angle = angle
        else:
            pass
            # print(angle)
    print(best_angle, max_power_val)
    arduino.write(bytes(f"{float(pa_fun(best_angle))}", "utf-8"))
    time.sleep(0.05)
    # change angle
    currentPitchAngle = best_angle




