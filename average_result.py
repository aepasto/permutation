#! /usr/bin/env python
import sys
from scipy import stats
import scipy as sp
import numpy as np
import math
import glob
import argparse

#def confidence(data, conf=0.95):
#    a = 1.0*np.array(data)
#    n = len(a)
#    m, se = np.mean(a), sp.stats.sem(a)
#    h = se * sp.stats.t._ppf((1+conf)/2., n-1)
#    return m, h

def read_stats(file_name):
    f = open(file_name,"r")
    labels = f.readline() # Title
    stat = []
    for line in f:
        if line[0]== 'C': continue #discards last line with center
        stat.append([float(x) for x in line.strip().split(",")])
    return labels, stat

""" ./average_result.py -f "../foo*.txt" """

def main():
    parser = argparse.ArgumentParser(description='Average table result')
    parser.add_argument('-f', '--filenames', action='store', dest='filenames', required=True)

    conf = parser.parse_args()
    files = glob.glob(conf.filenames)

    avg_stats = None

    total = 0
    labels_f = None
    for f in files:
        labels, stats = read_stats(f)
        labels_f = labels
        if avg_stats is None:
            avg_stats = [None] * len(stats)
        if (len(stats) < 30):
	    print "discarded"
            continue 
        assert len(avg_stats) == len(stats)
        for i in range(len(stats)):
            if avg_stats[i] is None:
                avg_stats[i] = [0.0] * len(stats[i])
            assert len(avg_stats[i]) == len(stats[i])
            for j in range(len(avg_stats[i])):
                avg_stats[i][j] += stats[i][j]
        total += 1

    for i in range(len(stats)):
        for j in range(len(stats[i])):
            avg_stats[i][j] /= total

    print labels_f.strip()
    for stat_l in avg_stats:
        print ",".join([str(x) for x in stat_l])

if __name__ == '__main__':
    main()
