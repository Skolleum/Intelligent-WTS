# Author: Yusuf Baran Tanrıverdi
# Contact: yusufbtanriverdi@hotmail.com

import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler



def get_scaler():
    # Read data
    df = pd.read_csv('Turbine_Data.csv', parse_dates=['Unnamed: 0'])

    df.rename({'Unnamed: 0': 'Date'}, axis=1, inplace=True)
    df = df[['Date',
             'ActivePower',
             'NacellePosition',
             'RotorRPM',
             'Blade1PitchAngle', 'Blade2PitchAngle', 'Blade3PitchAngle',
             'WindDirection', 'WindSpeed']]
    df = df.dropna(axis=0, subset=['Date',
                                   'ActivePower',
                                   'NacellePosition',
                                   'RotorRPM',
                                   'Blade1PitchAngle', 'Blade2PitchAngle', 'Blade3PitchAngle',
                                   'WindDirection', 'WindSpeed'])
    df['PitchAngle'] = df[['Blade1PitchAngle', 'Blade2PitchAngle', 'Blade3PitchAngle']].mean(axis=1)
    df = df[['Date',
             'ActivePower',
             'NacellePosition',
             'RotorRPM',
             'PitchAngle',
             'WindDirection', 'WindSpeed']]
    date = df['Date']

    date = np.asarray(date.values, dtype='datetime64[s]')
    date_float = date.astype("float")
    df['DateF'] = date_float
    # Define columns to use.
    feature_cols = df.columns[3:-1]
    # target_col = df.columns[1]

    # Split train and validation sets according to 80-20 convenience.

    boundary = int(len(df) * 0.8)

    df_train = df[:boundary]
    df_test = df[boundary:]

    print("Training set size:", len(df_train))
    print("Validation set size:", len(df_test))

    # Create X and y data.
    X_train = df_train[feature_cols]
    # X_test = df_test[feature_cols]
    #
    # y_train = df_train[target_col]
    # y_test = df_test[target_col]
    # Scale features within their std and means

    sc = StandardScaler()

    sc.fit_transform(np.array(X_train))

    return sc

