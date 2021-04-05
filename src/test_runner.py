#!/bin/python3
import subprocess
import sys
import os

if len(sys.argv) < 2:
    print("Usage: test_runner.py <executable>")
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

query_args = ['3 1',
              '8 8',
              '1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 1 2',
              '4 5 6',
              '2 4 6 8',
              '1 5 9',
              '8 6 4 2'
              ]

query_outputs = ['2.777747 5.539700',
                '7.830992 7.984400',
                '0.809638 0.609105 2.107746 2.097437 5.055056 7.446720 5.392867 8.758850 9.246286 3.082144 0.496353 2.529218 0.473570 3.954821 6.931734 5.535411 8.118067 9.333055 0.469908 2.162746',
                '3.979046 5.032039 6.011886',
                '1.996719 4.012344 5.988101 8.081113',
                '1.003042 4.986528 9.010856',
                '7.939939 5.934679 3.951869 1.930474'
                ]

tree_files = ['./trees/ex-' + arg.replace(' ', '-') + '.tree' for arg in alg_args] 

alg_args = [arg.split(' ') for arg in alg_args]

query_args = [[tree, *arg.split(' ')] for tree, arg in zip(tree_files, query_args)]

null_file = open('/dev/null', 'w')

if not os.path.exists('trees'):
    os.makedirs('trees')

for args, tree_file in zip(alg_args, tree_files):
    with open(tree_file, 'w+') as tree_fd:
        subprocess.run([executable, *args], stdout=tree_fd, stderr=null_file)

for args, expected_out in zip(query_args, query_outputs):
    result = subprocess.run(['./ballQuery', *args], capture_output=True, text=True)
    if result.stdout.strip() == expected_out.strip():
        print('.', end='', flush=True)
    else:
        print('x', end='', flush=True)
print()

os.rmdir('trees')