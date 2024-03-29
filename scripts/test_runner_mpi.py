#!/bin/python3
import subprocess
import sys
import os
import shutil

if len(sys.argv) < 4:
    print("Usage: test_runner.py <mpirun|srun> <executable> <n_threads>")
    exit(1)

runner = str(sys.argv[1])
executable =  str(sys.argv[2])
n_threads = str(sys.argv[3])

flags = ["-n", n_threads]
query_flags = ["-n", "1"]


if (runner == "srun"):
    flags.append("--ntasks-per-node=1")
    query_flags.append("--ntasks-per-node=1")


alg_args = ['2 5 0',
            '2 8 0',
            '3 3 3',
            '5 4 3',
            '2 6 8',
            '20 1000000 0',
            '3 5000000 0',
            '4 10000000 0',
            '3 20000000 0',
            '4 20000000 0'
            ]

query_args = ['3 1',
              '8 8',
              '5 5 5',
              '9 7 5 3 1',
              '3 7',
              '1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 1 2',
              '4 5 6',
              '2 4 6 8',
              '1 5 9',
              '8 6 4 2'
              ]

query_outputs = [b'2.777747 5.539700',
                b'7.830992 7.984400',
                b'5.613802 2.249833 3.930918',
                b'5.613802 2.249833 3.930918 4.439384 2.850413',
                b'3.527607 7.895896',
                b'0.809638 0.609105 2.107746 2.097437 5.055056 7.446720 5.392867 8.758850 9.246286 3.082144 0.496353 2.529218 0.473570 3.954821 6.931734 5.535411 8.118067 9.333055 0.469908 2.162746',
                b'3.979046 5.032039 6.011886',
                b'1.996719 4.012344 5.988101 8.081113',
                b'1.003042 4.986528 9.010856',
                b'7.939939 5.934679 3.951869 1.930474'
                ]

tree_files = ['./trees/ex-' + arg.replace(' ', '-') + '.tree' for arg in alg_args]

alg_args = [arg.split(' ') for arg in alg_args]

query_args = [[tree, *arg.split(' ')] for tree, arg in zip(tree_files, query_args)]

if not os.path.exists('trees'):
    os.makedirs('trees')

for alg_arg, tree_file, query_arg, expected_out in zip(alg_args, tree_files, query_args, query_outputs):
    with open(tree_file, 'w+') as tree_fd:
        subprocess.run([runner, *flags, executable, *alg_arg], stdout=tree_fd, stderr=subprocess.DEVNULL)
    result = subprocess.run([runner, *query_flags, '../src/ballQuery', *query_arg], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    if result.stdout.strip() == expected_out.strip():
        print('.', end='', flush=True)
    else:
        print('x', end='', flush=True)
    os.remove(tree_file)

print()

shutil.rmtree('trees')

