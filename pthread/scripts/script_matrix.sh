rm ../tempo_de_exec.txt
touch ../tempo_de_exec.txt

#Compila o código
gcc -g -Wall -o ../matrix ../src/matrix_calc.c -lpthread

#Loop principal de execuções. São 4 tentativas

	tentativas=4 #Quantas vezes o código será executado

	for size in 50 100 200 400 #tamanho do problema
	do
		echo -e "===================================================================\n" >> "../tempo_de_exec.txt"
			for thread in 8 16 32 #números de threads utilizadas
			do
				for tentativa in $(seq $tentativas)
				do
					echo -e `../matrix $thread $size`
				done
				echo -e " " >> "../tempo_de_exec.txt"
			done
			echo -e "\n===================================================================" >> "../tempo_de_exec.txt"
	done

exit
