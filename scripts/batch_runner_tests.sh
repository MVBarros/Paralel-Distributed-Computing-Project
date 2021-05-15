#!/bin/bash
#SBATCH --tasks-per-node 1
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null

mkdir -p trees
mkdir -p trees/$SLURM_NTASKS/
mkdir -p queries/obtained
mkdir -p queries/obtained/$SLURM_NTASKS

srun -n $SLURM_NTASKS -e /dev/null -o trees/$SLURM_NTASKS/20-1000000-0.tree ../src/ballAlg-mpi 20 1000000 0
srun -n 1 -e /dev/null -o queries/obtained/$SLURM_NTASKS/20-1000000-0.query ../src/ballQuery trees/$SLURM_NTASKS/20-1000000-0.tree 1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 1 2
rm trees/$SLURM_NTASKS/20-1000000-0.tree

srun -n $SLURM_NTASKS -e /dev/null -o trees/$SLURM_NTASKS/3-5000000-0.tree ../src/ballAlg-mpi 3 5000000 0
srun -n 1 -e /dev/null -o queries/obtained/$SLURM_NTASKS/3-5000000-0.query ../src/ballQuery trees/$SLURM_NTASKS/3-5000000-0.tree 4 5 6
rm trees/$SLURM_NTASKS/3-5000000-0.tree

srun -n $SLURM_NTASKS -e /dev/null -o trees/$SLURM_NTASKS/4-10000000-0.tree ../src/ballAlg-mpi 4 10000000 0
srun -n 1 -e /dev/null -o queries/obtained/$SLURM_NTASKS/4-10000000-0.query ../src/ballQuery trees/$SLURM_NTASKS/4-10000000-0.tree 2 4 6 8
rm trees/$SLURM_NTASKS/4-10000000-0.tree

srun -n $SLURM_NTASKS -e /dev/null -o trees/$SLURM_NTASKS/3-20000000-0.tree ../src/ballAlg-mpi 3 20000000 0
srun -n 1 -e /dev/null -o queries/obtained/$SLURM_NTASKS/3-20000000-0.query ../src/ballQuery trees/$SLURM_NTASKS/3-20000000-0.tree 1 5 9
rm trees/$SLURM_NTASKS/3-20000000-0.tree

srun -n $SLURM_NTASKS -e /dev/null -o trees/$SLURM_NTASKS/4-20000000-0.tree ../src/ballAlg-mpi 4 20000000 0
srun -n 1 -e /dev/null -o queries/obtained/$SLURM_NTASKS/4-20000000-0.query ../src/ballQuery trees/$SLURM_NTASKS/4-20000000-0.tree 8 6 4 2
rm trees/$SLURM_NTASKS/4-20000000-0.tree
