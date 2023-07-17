from sklearn.ensemble import RandomForestClassifier 
import pandas as pd
import pickle

df = pd.read_csv('data.csv')
pd.set_option('mode.use_inf_as_na', True)
df.fillna(0, inplace=True)

X = df.drop(['algorithm', 'filetype', 'class'], axis=1)
y = df['class']

print('training..')
random_forest = RandomForestClassifier()
random_forest.fit(X,y)

with open('random_forest.pkl', 'wb') as f:
    pickle.dump(random_forest, f)
