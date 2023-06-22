import sys
import numpy as np
import pandas as pd
import math
from collections import Counter
from sklearn.metrics import mean_squared_error
from scipy.stats import entropy, kurtosis, chisquare, hmean, skew
import pickle

def autocorr(data):
    return pd.Series(data).autocorr()

def num_bigrams(data):
    bg = [(data[i], data[i+1]) for i in range(len(data)-1)]
    return len(Counter(bg))

def calc_pi(data):
    hits = 0
    for i in range(0,len(data),4):
        b0 = hex(data[i])
        b1 = hex(data[i+1]).strip('0x')
        x = int(b0+b1, 16) 
        b0 = hex(data[i+2])
        b1 = hex(data[i+3]).strip('0x')
        y = int(b0+b1, 16) 
        x = x / (2**16)
        y = y / (2**16)
        if math.sqrt((x**2) + (y**2)) <= 1.0:
            hits += 1
    return 4.0 * (hits / (len(data)/4))

def calc_mse(data):
    #compare sorted row to line; compute mse
    norm = np.linspace(start=0, stop=255, num=4096, dtype='int')
    x = sorted(data)
    return mean_squared_error(norm, x)

def calc_ics(freqs):
    freqs = freqs.values
    #compute index of coincidence 
    c = 256.0
    N = 4096
    factor = N * (N-1) / c 
    total = 0 
    for i in range(256):
        n = freqs[i]
        total += n * (n-1)
    return total / factor

def get_ascii_freqs(data):
    totals = [(x,0) for x in range(256)]
    totals = Counter(dict(totals))
    counts = Counter(data)
    for c in counts:
        totals.update({c: counts[c]})
    return [totals[i] for i in totals]

#read in file (4096 bytes)
if len(sys.argv) != 2:
    print("usage: ./predict file")
    print("file: 4096 bytes")
    exit(1)

#read in bytes
filename = sys.argv[1]
f = open(filename, 'rb')
data = f.read()
f.close()

if len(data) != 4096: 
    print("ERROR: data must be 4096 bytes")
    exit(1)

#convert bytes to ASCII numbers
data = [int(x) for x in data]

#extract features
freqs = get_ascii_freqs(data)
freqs = pd.DataFrame(freqs)

columns = ['kurtosis', 'entropy', 'stdev', 'pi', 'mse',\
            'chi', 'p', 'autocorr', 'mean', 'bigrams', \
            'ics', 'hmean', 'skew']
features = pd.DataFrame(columns=columns)
features['kurtosis'] = kurtosis(freqs)
features['entropy'] = entropy(freqs)
features['stdev'] = freqs.std()
features['chi'] = chisquare(freqs)[0]
features['p'] = chisquare(freqs)[1]
features['autocorr'] = autocorr(data)
features['mean'] = np.mean(data)
features['bigrams'] = num_bigrams(data)
features['ics'] = calc_ics(freqs)
features['hmean'] = hmean(freqs)
features['skew'] = skew(freqs)
features['pi'] = calc_pi(data)
features['mse'] = calc_mse(data)

#load random forest
f = open('random_forest.pkl', 'rb')
random_forest = pickle.load(f)
f.close()
yp = random_forest.predict(features)
print(yp)



#algorithm,filetype,kurtosis,entropy,stdev,pi,mse,chi,p,autocorr,mean,bigrams,ics,hmean,skew,class
#aes,zip,1.0802615650051948,5.511024726051837,4.1952353926806065,3.32421875,1.2890625,280.5,0.1307481407105556,0.05115611912582848,127.2626953125,3956,1.0062271062271062,14.825624198545757,0.5143840589394348,0
