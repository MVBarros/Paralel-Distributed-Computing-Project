set term svg
set output "omp_speedup.svg"
set title 'Execution time graph'
set xlabel 'Number of threads (logarithmic scale)'
set border 3
set tics nomirror
set logscale x 2
set ylabel 'speedup'

plot 'omp_speedup.data' using 1:2 with linespoints title '20 1000000 0', \
    'omp_speedup.data' using 1:3 with linespoints title '3 5000000 0', \
    'omp_speedup.data' using 1:4 with linespoints title '4 10000000 0', \
    'omp_speedup.data' using 1:5 with linespoints title '3 20000000 0', \
    'omp_speedup.data' using 1:6 with linespoints title '4 20000000 0' \
