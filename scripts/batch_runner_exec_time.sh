#!/bin/bash
#SBATCH --tasks-per-node 1
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null

mkdir -p exec_times

mkdir -p exec_times/$SLURM_NTASKS

srun -o /dev/null -e exec_times/$SLURM_NTASKS/20-1000000-0.time ../src/ballAlg-mpi 20 1000000 0
srun -o /dev/null -e exec_times/$SLURM_NTASKS/3-5000000-0.time ../src/ballAlg-mpi 3 5000000 0
srun -o /dev/null -e exec_times/$SLURM_NTASKS/4-10000000-0.time ../src/ballAlg-mpi 4 10000000 0
srun -o /dev/null -e exec_times/$SLURM_NTASKS/3-20000000-0.time ../src/ballAlg-mpi 3 20000000 0
srun -o /dev/null -e exec_times/$SLURM_NTASKS/4-20000000-0.time ../src/ballAlg-mpi 4 20000000 0
