from sklearn.metrics import accuracy_score, precision_score, recall_score
import pandas as pd
import matplotlib.pyplot as plt
y =  {}
yp = {}

f = open('results.txt', 'r')
for line in f:
    line = line.strip()
    words = line.split(' ')
    filename = words[0]
    filetype = filename.split('/')[-2]
    if filetype not in y:
        y[filetype] = []
    if filetype not in yp:
        yp[filetype] = []

    result = words[1]
    if 'plaintext' in filename:
        y[filetype].append(0)
    else:
        y[filetype].append(1)

    if result == 'SUCCESS':
        yp[filetype].append(0)
    else:
        yp[filetype].append(1)
f.close()

results = []
for filetype in y:
    acc = 100*accuracy_score(y[filetype], yp[filetype])
    pre = 100*precision_score(y[filetype], yp[filetype])
    rec = 100*recall_score(y[filetype],yp[filetype])
    print(filetype, acc, pre, rec)
    results.append((filetype, acc, pre, rec))

results = pd.DataFrame(results, columns=['filetype', 'accuracy', 'precision', 'recall'])
results.plot.bar(x='filetype')
plt.legend(loc='lower right')
plt.ylabel('percentage (%)')
plt.title('classification results');
plt.show()
