#!/bin/bash
#SBATCH --tasks-per-node 1
#SBATCH -n 64
#SBATCH -N 64
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null

mkdir -p exec_times

for n in 1 2 4 8 16 32 64
do
    mkdir -p exec_times/${n}
done

for n in 1 2 4 8 16 32 64
do
    srun -n ${n} -o /dev/null -e exec_times/${n}/20-1000000-0.time ../src/ballAlg-mpi 20 1000000 0
    srun -n ${n} -o /dev/null -e exec_times/${n}/3-5000000-0.time ../src/ballAlg-mpi 3 5000000 0
    srun -n ${n} -o /dev/null -e exec_times/${n}/4-10000000-0.time ../src/ballAlg-mpi 4 10000000 0
    srun -n ${n} -o /dev/null -e exec_times/${n}/3-20000000-0.time ../src/ballAlg-mpi 3 20000000 0
    srun -n ${n} -o /dev/null -e exec_times/${n}/4-20000000-0.time ../src/ballAlg-mpi 4 20000000 0
done
