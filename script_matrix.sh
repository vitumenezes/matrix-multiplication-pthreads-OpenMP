#!/bin/bash

#SBATCH --partition=test
#SBATCH --job-name=PI
#SBATCH --output=PI.out
#SBATCH --error=PI.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=6
#SBATCH --time=0-0:30
#SBATCH --hint=compute_bound
#SBATCH --exclusive

rm tempo_de_exec.txt

#Compila o código
gcc -g -Wall -o matrix matrix_calc.c -lpthread

#Loop principal de execuções. São 4 tentativas

	tentativas=4 #Quantas vezes o código será executado

	for size in 100 200 300 400 500 #tamanho do problema
	do
		#echo -e "Size of problem: $size" >> "tempo_de_exec.txt"
			for thread in 8 16 32 63 #números de threads utilizadas
			do
				#echo -e "\n$thread\t$size\t\t\c" >> "tempo_de_exec.txt"
				for tentativa in $(seq $tentativas)
				do
					echo -e `./matrix $thread $size`
				done
			done
		#echo -e "\n" >> "tempo_de_exec.txt"
	done

exit
