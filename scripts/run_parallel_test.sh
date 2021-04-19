for i in {1..5}
do
	for n in 1 2 4 8
	do
		export OMP_NUM_THREADS="${n}"
		python3 test_runner.py ../src/ballAlg-omp
	done
done
