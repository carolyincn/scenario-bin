#!/usr/bin/python

from __future__ import print_function
import os  
import time
import csv
import re
import binascii
from util import *
read_path_in = "pipe-35"
read_path_out = "pipe-res-35"
CHAR_SIZE = 1

try:
    os.mkfifo(read_path_in)
    os.mkfifo(read_path_out)
except OSError, e:
    #print("mkfifo error", e)
    pass

# rf = open(read_path, 'r')
pf1 = os.open(read_path_in, os.O_RDONLY)
pf2 = os.open(read_path_out, os.O_WRONLY)
# print("os.open finished")
# out = open('res_csv1.csv','wb')
# csv_write = csv.writer(out)
# csv_write.writerow(['Node', 'inInterest', 'outInterest', 'inNack', 'outNack', 'csHit', 'csMiss'])

evillist = []
historyevillist = []
while True:
    lenchar = os.read(pf1, CHAR_SIZE)
    # line = rf.readline()
    # if line:
        # pass
        # print(line)
    
    if len(lenchar) == 0:
    #     # print("nodata")
        time.sleep(0.5)
    else:
        # data = binascii.b2a_uu(data)
        # msglen = binascii.b2a_uu(msglen)
        lenint = ord(lenchar)
        # print(lenint)
        data = os.read(pf1, lenint)
        data = data.strip('\x00')
        # print(len(data), data)
        datalist = dataProcess(data) # [evil1, evil2, ...]
        # if datalist.time - evillist[0].time > 0.5:
        
        # add into evillist
        for evil in datalist:
            if len(evillist) == 0:
                evillist.append(EvilMember(evil.time, evil.prefix, [[evil.nname, evil.fname, evil.num]]))
            else:
                if abs(evil.time - evillist[0].time) > 0.5:
                    evillist = [EvilMember(evil.time, evil.prefix, [[evil.nname, evil.fname, evil.num]])]
                
                else:
                    evilprefixlist = [i.prefix for i in evillist]
                    try:
                        evillist[evilprefixlist.index(evil.prefix)].list.append([evil.nname, evil.fname, evil.num])
                    except:
                        evillist.append(EvilMember(evil.time, evil.prefix, [[evil.nname, evil.fname, evil.num]]))
        
        for em in evillist:
            print(em.time, em.prefix, em.list)

        # calculate evil trace from evillist
        tracelist = []
        for el in evillist:
            trace = [el.prefix]
            # find first node
            nnamelist = [i[0] for i in el.list]
            fnamelist = [i[1] for i in el.list]
            # if ('outside' not in fnamelist) and ('other' not in fnamelist):
            #     # a = 1
            #     break
            # c = -1
            # for n in nnamelist:
            #     try:
            #         fnamelist.index(n)
            #         c = c + 1
            #     except:
            #         trace.append(n)
            #         c = c + 1
            #         break
            # try:
            #     while 1:
            #         if fnamelist[c] == 'outside' or fnamelist[c] == 'other':
            #             trace.append(fnamelist[c])
            #             break
            #         c = nnamelist.index(fnamelist[c])
            #         trace.append(nnamelist[c])
                    
            #     tracelist.append(trace)
            try:
                getdepth(trace + ['outside'], nnamelist, fnamelist, tracelist)
                getdepth(trace + ['other'], nnamelist, fnamelist, tracelist)
                evilwrap = ''
                for i in range(len(tracelist)): # write pipe if the evil source node and the corresponding prefix is not in history evil list
                    if [tracelist[i][0], tracelist[i][-1]] in historyevillist:
                        pass
                    else:
                        historyevillist.append([tracelist[i][0], tracelist[i][-1]])
                        evilwrap = evilwrap + tracelist[i][0] + ' ' + tracelist[i][-1] + '?' + ctrlfacelist[NameList.index(tracelist[i][-1])] + '\t'
                        print(evilwrap)
                evilwrap = chr(len(evilwrap)) + evilwrap
                os.write(pf2, evilwrap)
            except Exception, e:
                # tracelist.append(trace)
                print(e)

            # tracelist.append(trace)
        
        # if len(tracelist) == len(evillist):
            