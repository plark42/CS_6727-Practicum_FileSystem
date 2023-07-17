import os, sys
DATAPATH = '/Users/fac_rubin/Documents/GeorgiaTech/CS_6727-Practicum/data'

for i in range(1,6):
    model_file = '%s/classify/full_files/random_forest_%d.pkl' % (DATAPATH, i)
    test_file = '%s/classify/full_files/test_%d.txt' % (DATAPATH, i)
    f = open(test_file, 'r')
    files = f.readlines()
    f.close()

    num = 0
    for f in files:
        num += 1
        print('testing %d of %d' % (num, len(files)), file=sys.stderr)
        infile = '%s/%s' % (DATAPATH, f.strip())
        os.system('./Test %s %s' % (model_file, infile)) 

