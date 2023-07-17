#!/usr/bin/env python3
from feature_extraction import get_features, mime_to_number
import sys
import pickle

if len(sys.argv) != 2:
    print('usage: ./predict.py model.pkl')
    sys.exit(1)

f = open(sys.argv[1], 'rb')
random_forest = pickle.load(f)
f.close()

try: 
    f = open(0, 'rb')
    sys.stdout.write("!")
    sys.stdout.flush()

    while True:
        mime = f.readline().decode().strip()
        data = f.read(4096)
        if len(data) != 4096: 
            sys.stderr.write("ERROR: data must be 4096 bytes")
            exit(1)
        
        #run feature extraction
        features = get_features(data)
        features['mime'] = mime_to_number(str(mime))
        print(features)
        
        #load random forest
        yp = random_forest.predict(features)
        if yp[0] == 0:
            print('plaintext')
        else:
            print('encrypted')
        sys.stdout.flush()

except Exception as err:
    print(err)
    exit(0)
