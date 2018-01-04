all:
	g++ -w src/tim.cpp -Wall -std=c++11 -O3 src/sfmt/SFMT.c  -o tim

mac:
	clang++ -Xclang -fopenmp src/tim.cpp -I ~/libomp/openmp/include/ -L ~/libomp/openmp/lib/ -lomp

sim:
	g++ -w src/ourGraph.cpp -Wall -std=c++11 -O3 src/sfmt/SFMT.c  -o sim.out

run:
	./sim.out

run_tim:
	./tim -model LT -dataset nethept/ -epsilon 0.1 -k 50