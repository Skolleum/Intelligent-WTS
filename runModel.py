# Author: Yusuf Baran Tanrıverdi
# Contact: yusufbtanriverdi@hotmail.com

import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
import time
import joblib
from utils import get_scaler

gp = joblib.load("Gaussian Model")

data_df = pd.DataFrame(columns=['Date',
                                'ActivePower',
                                'NacellePosition',
                                'RotorRPM',
                                'PitchAngle',
                                'WindDirection',
                                'WindSpeed',
                                'DateF'])

sc = get_scaler()
ct = 0

while True:
    tmp_df = pd.read_csv("data.csv")

    if len(tmp_df) > data_df:
        tmp_df = tmp_df[['Date',
                         'ActivePower',
                         'NacellePosition',
                         'RotorRPM',
                         'PitchAngle',
                         'WindDirection',
                         'WindSpeed'
                         'DateF']]

        tmp_df = tmp_df[tmp_df.index >= len(data_df)]
        # Define columns to use.
        feature_cols = tmp_df.columns[3:-1]
        target_col = tmp_df.columns[1]

        X_train = tmp_df[feature_cols]
        y_train = tmp_df[target_col]

        X_train = sc.fit_transform(np.array(X_train))
        gp.fit(X_train, y_train)

        ct = ct+1
        if ct >= 1000:
            joblib.dump(gp, f"Gaussian_model_updated_at_{time.asctime()}")

        data_df = tmp_df




