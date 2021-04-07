#!/bin/python3
import subprocess
import sys
from tabulate import tabulate

if len(sys.argv) < 2:
    print("Usage: test_program_time.py <executable>")
    exit(1)

executable = "./" + str(sys.argv[1])

alg_args = ['2 5 0',
            '2 8 0',
            '20 1000000 0',
            '3 5000000 0',
            '4 10000000 0',
            '3 20000000 0',
            '4 20000000 0'
            ]

expected_times = ['0.0', '0.0', '7.3', '23.2', '57.2', '122.5', '131.6']

table=[]

for args, expected_time in zip(alg_args, expected_times):
    result = subprocess.run([executable, *args.split(' ')], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    time = result.stderr.strip()
    table.append([args, time, expected_time])
    
print(tabulate(table, headers=["Arguments", "Obtained Time", "Reference Time"], tablefmt="github"))