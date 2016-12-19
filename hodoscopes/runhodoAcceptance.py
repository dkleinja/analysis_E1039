import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)
batchargv = int(sys.argv[1])
print batchargv
import os

print 'OK, let us start running over the files'
os.system("date")
print '\n'

for run in range(0, 1):
        for hodoeff in range(0, 2):
                for file in range(0, 5):
                        startfile = 5*run + file
                        scpfile = "scp dkleinja@seaquestgpvm01.fnal.gov:/e906/data/users/dkleinja/dpsim/output/R005/batch%s_dy_hodo%s_100K_R005_%s.root ./" % (batchargv, hodoeff, startfile)
                        print scpfile
                        os.system(scpfile)

                hodostatement = "root -b -q getphi_dpsim2.C\(%s,%s,%s\) > ./logs/phidist_batch%s_hodoEff%s_seed%s.log 2>&1" % (batchargv, hodoeff, run, batchargv, hodoeff, run) 
                print hodostatement
                os.system(hodostatement)

                for file in range(0, 5):
                        startfile = 5*run + file
                        rmfile = "rm ./batch%s_dy_hodo%s_100K_R005_%s.root" % (batchargv, hodoeff, startfile)
                        print rmfile
                        os.system(rmfile)
        os.system("date")
print 'OK, we are done running'
os.system("date")
