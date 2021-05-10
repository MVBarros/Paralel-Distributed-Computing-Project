if [[ $# -ne 1 ]]; then
	echo "Usage: $0 <mpirun|srun>"
	exit 1
fi

for i in {1..5}
do
	for n in 1 2 4 8 16 32 64
	do
		python3 test_runner_mpi.py $1 ../src/ballAlg-mpi ${n}
	done
done
