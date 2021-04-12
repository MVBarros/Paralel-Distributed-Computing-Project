#!/bin/python3
import subprocess
import sys
import os
from tabulate import tabulate

if len(sys.argv) < 3:
    print("Usage: test_thread_time.py <serial_executable> <parallel_executable")
    exit(1)

serial_executable = "./" + str(sys.argv[1])
parallel_executable = "./" + str(sys.argv[2])

env = dict(os.environ)

num_threads_list = ["1", "2", "4" ,"8"]

alg_args = ['2 5 0',
            '2 8 0',
            '20 1000000 0',
            '3 5000000 0',
            '4 10000000 0',
            '3 20000000 0',
            '4 20000000 0'
            ]

table=[]

for args in alg_args:
    result = subprocess.run([serial_executable, *args.split(' ')], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    time = result.stderr.strip()
    table.append([args, time])

for num_threads in num_threads_list:
    env["OMP_NUM_THREADS"] = num_threads
    for args, entry in zip(alg_args, table):
        result = subprocess.run([parallel_executable, *args.split(' ')], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, env=env)
        time = result.stderr.strip()
        entry.append(time)


headers = ["Arguments", "Serial", *(f"Parallel {num} threads" for num in num_threads_list)]

print(tabulate(table, headers=headers, tablefmt="github"))