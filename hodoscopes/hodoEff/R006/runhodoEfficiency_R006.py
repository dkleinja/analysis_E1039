import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)
firstargv = int(sys.argv[1])
print firstargv
import os

print 'OK, let us start running over the files'
os.system("date")
print '\n'

with open('raweventlist_67.txt') as fp:
	#lines = [line[:-1] for line in fp]
	elines = [line.rstrip('\n') for line in fp]
        #print elines
	#print '\n'

with open('trackletlist_67.txt') as fp:
        #lines = [line[:-1] for line in fp]
        tlines = [line.rstrip('\n') for line in fp]
        #print tlines
        #print '\n'

with open('nim1list_67.txt') as fp:
        #lines = [line[:-1] for line in fp]
        nlines = [line.rstrip('\n') for line in fp]
        #print tlines
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
	#dccp_rawEvent = "dccp %s ./" % (elines[run])
	#dccp_tracklet = "dccp %s ./" % (nlines[run])
	dccp_nim1 = "dccp %s ./" % (nlines[run])
        print dccp_nim1
        hodostatement = "./hodoEfficiency track_%s_r1.6.0.root ./root_files/R006files_getgeo_%s.root > ./logs/R006files_getgeo_%s.log 2>&1" % (rlines[run], rlines[run], rlines[run]) 
        #hodostatement = "./hodoEfficiency track_%s_r1.6.0.root ./root_files/R006files_nim1_%s.root" % (rlines[run], rlines[run]) 
	rmfiles = "rm track_%s_r1.6.0.root" % (rlines[run])
        #print dccp_rawEvent
        #print dccp_tracklet
        print dccp_nim1
        print hodostatement
        print rmfiles
	#os.system(dccp_rawEvent)
	#os.system(dccp_tracklet)
	os.system(dccp_nim1)
        os.system(hodostatement)
        os.system(rmfiles)
        os.system("date")
print 'OK, we are done running'
os.system("date")
