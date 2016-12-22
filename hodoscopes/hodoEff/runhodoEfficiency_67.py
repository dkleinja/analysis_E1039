import os

print 'OK, let us start running over the files'
os.system("date")
print '\n'

with open('filelist_67.txt') as fp:
	#lines = [line[:-1] for line in fp]
	flines = [line.rstrip('\n') for line in fp]
        #print flines
	#print '\n'

with open('runlist_67.txt') as rp:
        #lines = [line[:-1] for line in rp]
        rlines = [line.rstrip('\n') for line in rp]
        #print rlines
        #print '\n'


#for run in range(0, 100):
for run in range(0, 2822):
	#print flines[run]
	#print rlines[run]
        #hodostatement = "./hodoEfficiency %s ./root_files/hodoEff_%s.root > ./logs/hodoEff_%s.log 2>&1" % (flines[run], rlines[run], rlines[run]) 
        #hodostatement = "./hodoEfficiency %s ./root_files/geotest_%s.root > ./logs/geotest_%s.log 2>&1" % (flines[run], rlines[run], rlines[run]) 
        hodostatement = "./hodoEfficiency %s ./root_files/momcut10_ycut3_centcut90_trighits1_%s.root > ./logs/momcut10_ycut3_centcut90_trighits1_%s.log 2>&1" % (flines[run], rlines[run], rlines[run]) 
        print hodostatement
        os.system(hodostatement)
print 'OK, we are done running'
