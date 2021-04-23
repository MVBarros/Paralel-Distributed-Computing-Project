rm g29omp.zip
mkdir g29omp
cp src/*.c g29omp
cp src/*.h g29omp
cp src/Makefile g29omp
cp docs/g29report.pdf g29omp
zip g29omp.zip g29omp/*
rm g29omp -r
