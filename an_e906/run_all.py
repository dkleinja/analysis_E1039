#!/usr/bin/env python

import os

#Command Line Arguments are stored in
nCpus = 9;
    
for jobno in range(0, nCpus):
    print '\t This is job number %d' % jobno;
    #print '\n';
    runstart = 500*jobno + 1
    shelljob = "./run_all.sh 67 %d " % runstart
    print shelljob;
    os.system(shelljob)
print 'Done with submitting %d jobs' % nCpus

os.system("echo finalstatement");
