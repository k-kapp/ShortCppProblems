#!/usr/bin/python

import subprocess
import sys
import os

if len(sys.argv) < 3 or len(sys.argv) > 4:
    print ("usage: ./match_outs.py <executable> <input_file> [output_file]")
    exit()

output = subprocess.check_output("cat " + sys.argv[2] + " | " + sys.argv[1], shell=True).decode("UTF-8").strip().split('\n')

out_filename = None
if (len(sys.argv) == 3):
    out_filename = sys.argv[2]
    out_filename = out_filename.replace("input", "output")
else:
    out_filename = sys.argv[3]

with open(out_filename) as out_file:
    match = True
    line_count = 0
    for line_memo, line_out in zip(out_file, output):
        line_memo = line_memo.strip().split(" ")
        line_out = line_out.strip().split(" ")
        count = 0
        for num_memo, num_out in zip(line_memo, line_out):
            if (num_memo != num_out):
                print ("mismatch at position %i on line %i" % (count, line_count))
                match = False
            count += 1
        line_count += 1

if match:
    print ("Answer matches that of output file " + out_filename)
