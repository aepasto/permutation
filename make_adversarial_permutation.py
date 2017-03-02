#! /usr/bin/env python

import argparse
import os
import sys
import gzip
import time
import random
import math
from collections import defaultdict

""" Read a permutation file and randomized the end of the permutation in adversarial way (i.e. copying the beginning of another permutation of a different category) """

def randomize_perm(filename_permutations, percentage_randomized):

    cat_ranks = defaultdict(list)

    with open(filename_permutations, 'r') as file_:
        for line in file_:
            fields = line.strip().split("\t")
            if len(fields) < 3:
                continue
            page_id = int(fields[0].strip())
            cat = str(fields[1]).strip()
            rank = [x.strip() for x in str(fields[2]).split(',')]
            ranks = []
            for x in rank:
                if len(x)>0:
                    ranks.append(int(x))

            cat_ranks[cat].append((page_id, ranks))


    randomized_ranks = []
    categories = list(cat_ranks.keys())
    for cat, page_ranks in cat_ranks.items():
        for page_rank in page_ranks:
            page, old_ranks = page_rank
            rand_other_cat = None
            while True:
                ra = random.choice(categories)
                if ra <> cat:
                    rand_other_cat = ra
                    break
            other_page, other_ranking = random.choice(cat_ranks[rand_other_cat])
            new_ranking = []
            new_ranking_set = set()

            # first add the original positions
            pos_stop = percentage_randomized * len(old_ranks)
            for i in range(int(pos_stop)):
                new_ranking.append(old_ranks[i])
                new_ranking_set.add(old_ranks[i])

            # second add the other positions from a random adversarial ranking
            for x in other_ranking:
                if x not in new_ranking_set:
                    new_ranking_set.add(x)
                    new_ranking.append(x)

            assert len(new_ranking) == len(old_ranks)

            print "\t".join([str(page), cat, ",".join([str(x) for x in new_ranking])])

"""
./make_adversarial_permutation.py -f ../dataset/rankings-category-8-small-10percent.txt -p 0.5 > ../dataset/rankings-category-8-small-10percent-adversarial50.txt
"""
def main():
    #print "MUST USE TransitiveType NOT SimpleType for yago"
    parser = argparse.ArgumentParser(description='Sample yago')
    parser.add_argument('-f', '--filename_yago', action='store', dest='filename', required=True)
    parser.add_argument('-p', '--fractio_randomized', action='store', dest='fraction_randomized', required=True)


    conf = parser.parse_args()
    randomize_perm(conf.filename, float(conf.fraction_randomized))

if __name__ == '__main__':
    main()
