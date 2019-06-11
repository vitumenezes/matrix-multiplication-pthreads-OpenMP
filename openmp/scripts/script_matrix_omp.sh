#!/bin/bash
#SBATCH --partition=cluster
#SBATCH --job-name=matrix_multiplication_omp
#SBATCH --output=out_matrix_calc_omp.out
#SBATCH --error=err_matrix_calc_omp.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=32
#SBATCH --time=0-2:30
#SBATCH --exclusive
#SBATCH --hint=compute_bound

rm ~/openmp/runtimes/tempo_de_exec_omp.txt
touch ~/openmp/runtimes/tempo_de_exec_omp.txt

#Compila o código
gcc -g -Wall -fopenmp -o ~/openmp/matrix_calc_omp ~/openmp/src/matrix_calc_omp.c

#Loop principal de execuções. São 4 tentativas

tentativas=15 #Quantas vezes o código será executado

for size in 1500 1800 2000 2300 #tamanho do problema
do
	echo -e "==============================================================\n" >> "/home/vgdmenezes/openmp/runtimes/tempo_de_exec_omp.txt"
	for thread in 2 4 8 16 32 #números de threads utilizadas
	do
		for tentativa in $(seq $tentativas)
		do
			echo -e `/home/vgdmenezes/openmp/matrix_calc_omp $thread $size`
		done
		echo -e " " >> "/home/vgdmenezes/openmp/runtimes/tempo_de_exec_omp.txt"
	done
	echo -e "\n==============================================================" >> "/home/vgdmenezes/openmp/runtimes/tempo_de_exec_omp.txt"
done

exit
