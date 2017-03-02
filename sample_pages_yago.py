#! /usr/bin/env python

import argparse
import os
import sys
import gzip
import bz2
import time
import random
import math
from collections import defaultdict

""" Samples n pages at random from each category given in input in wipedia and output list (of numerical id) """

def parse_yago(filename_yago, filename_ids, cats, num_per_cat, output_sample, output_cat_nodes):
    lines = 0

    cats = set(cats)
    cat_count = defaultdict(int)
    cat_pages = defaultdict(list)

    target_pages = set()
    page_ids = {}

    with gzip.GzipFile(filename_yago, 'r') as zip_file:
        for line in zip_file:
            fields = line.strip().split("\t")
            if len(fields) < 4:
                continue
            page = str(fields[1]).strip()
            cat = str(fields[3])
            cat_count[cat] += 1


            # We now convert yago's id to webgraph unimi format of .ids.gz
            # spaces instead of '_' and does not have < > at begin and end
            p_in_unimi_format = page[1:-1].replace('_', ' ').strip()

            if cat in cats:
                cat_pages[cat].append(p_in_unimi_format)
                target_pages.add(p_in_unimi_format)

    print cat_pages

    # Print count per category
    #for ca, co in sorted(cat_count.items(), key=lambda y:y[1], reverse=True):
    #    print ca, co


    pos = 0
    with bz2.BZ2File(filename_ids, 'r') as zip_file:
        for line in zip_file:
            page = line.strip()
            print page
            if page in target_pages:
                page_ids[page] = pos
            pos+=1

    # remove pages not found
    for cat in cats:
        original_list = cat_pages[cat]
        new_list = []
        for p in original_list:
            if p in page_ids:
                # check not in other cats
                in_other_cat = False
                for cat2 in cats:
                    if cat2 != cat and p in cat_pages[cat2]:
                        in_other_cat = True
                if not in_other_cat:
                    new_list.append(p)
        cat_pages[cat] = new_list

#    for x,y in sorted(cat_count.items(), key=lambda y:y[1], reverse=True):
#        print x,y
    f_out_sample = open(output_sample, "w")

    for cat, list_p in cat_pages.items():
        #print cat, len(list_p)
        sample = random.sample(list_p, min(num_per_cat,len(list_p)))
        for p in sample:
            f_out_sample.write(str(p) + "\t" + str(cat) + "\t" + str(page_ids[p]) + "\n")

    f_out_cat = open(output_cat_nodes, "w")
    for cat, list_p in cat_pages.items():
        for p in list_p:
            f_out_cat.write(str(p) + "\t" + str(cat) + "\t" + str(page_ids[p]) + "\n")


"""<wikicat_Theoretical_computer_scientists>,<wikicat_20th-century_actors>"""

""" Use -c comma_separated_categories
yago is the catogory file, _ids is the file associating each wiki page with a numerical id. """

""" E.g.
./sample_pages_yago.py -f ../dataset/yagoTrasitiveTypes.tsv.gz -i ../dataset/enwiki-2013.ids.gz -c "<wikicat_Theoretical_computer_scientists>,<wikicat_20th-century_actors>" -n 10 -o output_sample -w output_cat_nodes
"""

def main():
    #print "MUST USE TransitiveType NOT SimpleType for yago"
    parser = argparse.ArgumentParser(description='Sample yago')
    parser.add_argument('-f', '--filename_yago', action='store', dest='filename_yago', required=True)
    parser.add_argument('-i', '--filename_ids', action='store', dest='filename_ids', required=True)
    parser.add_argument('-c', '--categories', action='store', dest='categories', required=True)
    parser.add_argument('-n', '--number_per_category', action='store', dest='num_per_cat', required=True)
    parser.add_argument('-o', '--output_sample', action='store', dest='output_sample', required=True)
    parser.add_argument('-w', '--output_cat_nodes', action='store', dest='output_cat_nodes', required=True)


    conf = parser.parse_args()
    parse_yago(conf.filename_yago, conf.filename_ids, conf.categories.split(','), int(conf.num_per_cat), conf.output_sample, conf.output_cat_nodes)

if __name__ == '__main__':
    main()
