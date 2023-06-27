import glob, os

KEY = 'password'

cmds = {}
cmds['aes'] = 'openssl enc -aes-256-cbc -pass pass:%s -nosalt -in %s -out %s'
cmds['bf'] = 'openssl enc -bf -pass pass:%s -nosalt -in %s -out %s'
cmds['cha'] = 'openssl enc -chacha -pass pass:%s -nosalt -in %s -out %s'
cmds['des3'] = 'openssl enc -des3 -pass pass:%s -nosalt -in %s -out %s'
cmds['rc4'] = 'openssl enc -rc4 -pass pass:%s -nosalt -in %s -out %s' 

for f in sorted(glob.glob('test.*')):
    filetype = f.split('.')[1]
    infile  = f

    for algo in ['aes', 'bf', 'cha', 'des3', 'rc4']:
        outfile = '%s.%s' % (algo, infile) 
        cmd = cmds[algo] % (KEY, infile, outfile)
        print(cmd)
        os.system(cmd)

