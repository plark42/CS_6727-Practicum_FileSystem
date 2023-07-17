#!/usr/bin/env python3
import warnings
#warnings.filterwarnings("ignore")

from feature_extraction import get_features, mime_to_number
import sys
import pickle

def main():
    #read in file (4096 bytes)
    if len(sys.argv) != 4:
        print("usage: ./predict.py file model.pkl mime")
        print("  where file: 4096 bytes")
        print("use --stdin for standard input")
        print("e.g., cat pln.txt | ./predict.py --stdin model.pkl application/text")
        exit(1)
    
    #read in bytes
    filename = sys.argv[1]
    model = sys.argv[2]
    mime = sys.argv[3]

    if filename == '--stdin':
        f = open(0, 'rb')
    else:
        f = open(filename, 'rb')
    data = f.read()
    f.close()
    
    
    if len(data) != 4096: 
        print("ERROR: data must be 4096 bytes")
        sys.exit(1)
    
    #run feature extraction
    features = get_features(data)
    features['mime'] = mime_to_number(mime)
    
    #load random forest
    f = open(model, 'rb')
    random_forest = pickle.load(f)
    f.close()
    yp = random_forest.predict(features)
    if yp[0] == 0:
        print('plaintext')
    else:
        print('encrypted')
    sys.stdout.flush()
    
if __name__ == '__main__':
    main()
