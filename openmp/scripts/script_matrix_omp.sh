rm ../runtimes/tempo_de_exec_omp.txt
touch ../runtimes/tempo_de_exec_omp.txt

#Compila o código
gcc -g -Wall -fopenmp -o ../matrix_calc_omp ../src/matrix_calc_omp.c

#Loop principal de execuções. São 4 tentativas

	tentativas=4 #Quantas vezes o código será executado

	for size in 50 100 200 400 #tamanho do problema
	do
		echo -e "===================================================================\n" >> "../runtimes/tempo_de_exec_omp.txt"
			for thread in 8 16 32 #números de threads utilizadas
			do
				for tentativa in $(seq $tentativas)
				do
					echo -e `../matrix $thread $size`
				done
				echo -e " " >> "../runtimes/tempo_de_exec_omp.txt"
			done
			echo -e "\n===================================================================" >> "../runtimes/tempo_de_exec_omp.txt"
	done

exit
