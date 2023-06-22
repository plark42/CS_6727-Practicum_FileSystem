import os
from random import shuffle

#DATAPATH = '/Users/fac_rubin/Documents/GeorgiaTech/CS_6727-Practicum/data/'
DATAPATH = '../../data/'

encrypted = []
plaintext = []

for dirpath, dirnames, filenames in os.walk(DATAPATH + 'plaintext'):
    if len(dirnames) == 0:
        for fname in filenames:
            if "DS_Store" in fname: 
                continue
            filepath = '%s/%s' % (dirpath, fname)
            plaintext.append(filepath)

allfiles = []
for p_file in plaintext:
    for enc in ['aes', 'bf', 'cha', 'des3', 'rc4']:
        s = 'encrypted/%s' % (enc)
        e_file = p_file.replace('plaintext', s)
        e_file = e_file + '.%s' % enc
        allfiles.append(p_file)
        allfiles.append(e_file)

index = list(range(0,len(allfiles),2))
shuffle(index)

train_index = index[:len(index)//2]
test_index  = index[len(index)//2:]

train = []
test = []
for i in train_index:
    train.append(allfiles[i])
    train.append(allfiles[i+1])
    
for i in test_index:
    test.append(allfiles[i])
    test.append(allfiles[i+1])

with open('train.txt', 'w') as f:
    f.write('\n'.join(train))

with open('test.txt', 'w') as f:
    f.write('\n'.join(test))





