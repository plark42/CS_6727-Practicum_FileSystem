from sklearn.metrics import accuracy_score, precision_score, recall_score
y =  []
yp = []

f = open('results.txt', 'r')
#f = open('results_enc_ratio_85.txt', 'r')
for line in f:
    line = line.strip()
    words = line.split(' ')
    filename = words[0]
    print(filename)
    result = words[1]
    if 'plaintext' in filename:
        y.append(0)
    else:
        y.append(1)

    if result == 'SUCCESS':
        yp.append(0)
    else:
        yp.append(1)
f.close()

acc = 100* accuracy_score(y, yp)
pre = 100*precision_score(y, yp)
rec = 100*recall_score(y,yp)
print('overall:', acc, pre, rec)


