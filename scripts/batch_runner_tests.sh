#!/bin/bash
#SBATCH --tasks-per-node 1
#SBATCH -n 64
#SBATCH -N 64
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null

mkdir -p trees
mkdir -p queries/obtained

for n in 1 2 4 8 16 32 64
do
    mkdir -p queries/obtained/${n}
done

for n in 1 2 4 8 16 32 64
do
    srun -n ${n} -e /dev/null -o trees/20-1000000-0.tree ../src/ballAlg-mpi 20 1000000 0
    srun -n 1 -e /dev/null -o queries/obtained/${n}/20-1000000-0.query ../src/ballQuery trees/20-1000000-0.tree 1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 1 2
    rm trees/20-1000000-0.tree

    srun -n ${n} -e /dev/null -o trees/3-5000000-0.tree ../src/ballAlg-mpi 3 5000000 0
    srun -n 1 -e /dev/null -o queries/obtained/${n}/3-5000000-0.query ../src/ballQuery trees/3-5000000-0.tree 4 5 6
    rm trees/3-5000000-0.tree

    srun -n ${n} -e /dev/null -o trees/4-10000000-0.tree ../src/ballAlg-mpi 4 10000000 0
    srun -n 1 -e /dev/null -o queries/obtained/${n}/4-10000000-0.query ../src/ballQuery trees/4-10000000-0.tree 4 5 6
    rm trees/4-10000000-0.tree

    srun -n ${n} -e /dev/null -o trees/3-20000000-0.tree ../src/ballAlg-mpi 3 20000000 0
    srun -n 1 -e /dev/null -o queries/obtained/${n}/3-20000000-0.query ../src/ballQuery trees/3-20000000-0.tree 4 5 6
    rm trees/3-20000000-0.tree

    srun -n ${n} -e /dev/null -o trees/4-20000000-0.tree ../src/ballAlg-mpi 4 20000000 0
    srun -n 1 -e /dev/null -o queries/obtained/${n}/4-20000000-0.query ../src/ballQuery trees/4-20000000-0.tree 4 5 6
    rm trees/4-20000000-0.tree
done
