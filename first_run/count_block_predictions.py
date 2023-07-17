#!/usr/bin/env python3
from feature_extraction import get_features 
import sys
import pickle
from collections import Counter

def main():
    if len(sys.argv) != 2:
        print('usage: ./count_block_predictions.py file.txt')
        exit(1)

    f = open('random_forest.pkl', 'rb')
    random_forest = pickle.load(f)
    f.close()

    f = open(sys.argv[1], 'rb')
    _bytes = f.read()
    f.close()

    pred = []
    for i in range(0, len(_bytes), 4096):
        data = _bytes[i:i+4096]
        if len(data) < 4096:
            diff = 4096 - len(data)
            data += bytes(diff) 

        features = get_features(data)
        yp = random_forest.predict(features)
        pred.append(yp[0])
    
    counts = Counter(pred)

    pred = ','.join([str(x) for x in pred])
    output = '%s; %s; pln=%d; enc=%d' % (sys.argv[1], pred, counts[0], counts[1])
    print(output)
    print(output, file=sys.stderr)


if __name__ == '__main__':
    main()
