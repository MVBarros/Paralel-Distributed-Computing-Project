for i in 1 2 4 8 16 32 64
do
    sbatch -n ${i} batch_runner_tests.sh
done