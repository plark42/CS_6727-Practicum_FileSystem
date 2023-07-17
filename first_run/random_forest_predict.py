from sklearn.ensemble import RandomForestClassifier 
import pandas as pd
import pickle
import random

f = open('random_forest.pkl', 'rb')
random_forest = pickle.load(f)
f.close()

df = pd.read_csv('data.csv')
pd.set_option('mode.use_inf_as_na', True)
df.fillna(0, inplace=True)

row = df.sample()
x = row.drop(['algorithm', 'filetype', 'class'], axis=1)
y = row['class']


yp = random_forest.predict(x)
print(type(yp))
print(y, yp)


