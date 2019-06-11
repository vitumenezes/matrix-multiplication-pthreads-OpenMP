#!/bin/bash
#SBATCH --partition=cluster
#SBATCH --job-name=matrix_multiplication_omp 
#SBATCH --time=0-1:00
#SBATCH --output=serial_out_matrix_calc_omp.out
#SBATCH --error=serial_err_matrix_calc_omp.err
#SBATCH --nodes=1
#SBATCH --hint=compute_bound
#SBATCH --exclusive

rm ~/openmp/runtimes/serial_tempo_de_exec.txt
touch ~/openmp/runtimes/serial_tempo_de_exec.txt

#Compila o código
gcc -g -Wall -o ~/openmp/serial_matrix_omp ~/openmp/src/serial_matrix_calc.c

#Loop principal de execuções. São 4 tentativas

	tentativas=15 #Quantas vezes o código será executado

	for size in 1500 1800 2000 2300 #tamanho do problema
	do
		echo -e "===================================================\n" >> "/home/vgdmenezes/openmp/runtimes/serial_tempo_de_exec.txt"
		for tentativa in $(seq $tentativas)
		do
			echo -e `/home/vgdmenezes/openmp/serial_matrix_omp $size`
		done
		echo -e " " >> "/home/vgdmenezes/openmp/runtimes/serial_tempo_de_exec.txt"
		echo -e "\n===========================================" >> "/home/vgdmenezes/openmp/runtimes/serial_tempo_de_exec.txt"
	done

exit
