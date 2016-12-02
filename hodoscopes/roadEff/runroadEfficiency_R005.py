import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)
firstargv = int(sys.argv[1])
print firstargv
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



for values in range(0, 202):
#for values in range(0, 1):
#for run in range(0, 2822):
	run = 14*values + firstargv
	print run
	#print rlines[run]
	#dccp_file = "cp %s ./" % (flines[run])
        #print dccp_file
        roadstatement = "./roadEfficiency roadset67DSTs/track_%s_r1.5.0.root ./root_files/R005files_fixedbug_%s.root > ./logs/R005files_fixedbug_%s.log 2>&1" % (rlines[run], rlines[run], rlines[run]) 
        #roadstatement = "./roadEfficiency track_%s_r1.6.0.root ./root_files/R005files_nim1_%s.root" % (rlines[run], rlines[run]) 
	#rmfiles = "rm track_%s_r1.6.0.root" % (rlines[run])
        #print dccp_rawEvent
        #print dccp_tracklet
        #print dccp_file
        print roadstatement
        #print rmfiles
	#os.system(dccp_rawEvent)
	#os.system(dccp_tracklet)
	#os.system(dccp_file)
        os.system(roadstatement)
        #os.system(rmfiles)
        os.system("date")
print 'OK, we are done running'
os.system("date")
