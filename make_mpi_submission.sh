rm g29mpi.zip
mkdir g29mpi
cp src/*.c g29mpi
cp src/*.h g29mpi
cp src/Makefile g29mpi
cp docs/g29report.pdf g29mpi
zip g29mpi.zip g29mpi/*
rm g29mpi -r
