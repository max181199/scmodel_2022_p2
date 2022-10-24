module load SpectrumMPI
mpicc p2_par.c -o p2_par -O3

mpisubmit.pl -p 2 ./p2_par 3.0e-5
mpisubmit.pl -p 4 ./p2_par 3.0e-5
mpisubmit.pl -p 16 ./p2_par 3.0e-5
mpisubmit.pl -p 32 ./p2_par 3.0e-5

mpisubmit.pl -p 2 ./p2_par 5.0e-6
mpisubmit.pl -p 4 ./p2_par 5.0e-6
mpisubmit.pl -p 16 ./p2_par 5.0e-6
mpisubmit.pl -p 32 ./p2_par 5.0e-6

mpisubmit.pl -p 2 ./p2_par 15.0e-7
mpisubmit.pl -p 4 ./p2_par 15.0e-7
mpisubmit.pl -p 16 ./p2_par 15.0e-7
mpisubmit.pl -p 32 ./p2_par 15.0e-7