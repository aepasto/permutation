#! /usr/bin/env python

import argparse
import os
import sys
import random
import math

""" Produces permutations based on the AP distribution --
    the center is fixed at 1, 2, 3 ... N"""
def ap_permutation(N, beta):
    perm = [1]

    for n in range(2, N+1):
        q = math.exp((1.0*N*beta)/(2.0*(n-1)))
        probs = [0.0] * (n+1) # at position i prob of going in i
        for i in range(1,n+1):
            probs[i] = q**(i)
        sp = sum(probs)
        for i in range(1,n+1):
            probs[i] /= sp

        r = random.random()
        i = 1
        r -= probs[i]
        while (r > 0):
            i += 1
            r -= probs[i]

        perm = perm[:i-1] + [n] + perm[i-1:]

    return perm

def main():


    parser = argparse.ArgumentParser(description='Produce Rand Perm according to model')
    parser.add_argument('-n', '--n_lenght_perm', action='store', dest='n_lenght_perm', required=True)
    parser.add_argument('-b', '--beta', action='store', dest='beta', required=True)
    parser.add_argument('-s', '--number_samples', action='store', dest='num_samples', required=True)


    conf = parser.parse_args()
    for i in range(int(conf.num_samples)):
        p = ap_permutation(int(conf.n_lenght_perm), float(conf.beta), )
        str_out = str(i) + "\t0\t" + ','.join([str(x) for x in p])
        print str_out

if __name__ == '__main__':
    main()
