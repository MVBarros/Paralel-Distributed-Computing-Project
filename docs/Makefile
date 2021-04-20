.PHONY: omp clean

omp:
	make zip -C ../src
	@mv ../src/g29omp.zip .
	@zip -u g29omp.zip g29report.pdf

clean:
	@rm *.zip