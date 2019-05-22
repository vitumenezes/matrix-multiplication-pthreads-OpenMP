rm tempo_de_exec_test.txt
touch tempo_de_exec_test.txt

#Compila o código
gcc -g -Wall -o matrix matrix_calc.c -lpthread

#Loop principal de execuções. São 4 tentativas

	tentativas=4 #Quantas vezes o código será executado

	for size in 50 100 200 400 #tamanho do problema
	do
		echo -e "===================================================================\n" >> "tempo_de_exec_test.txt"
			for thread in 8 16 32 63 #números de threads utilizadas
			do
				for tentativa in $(seq $tentativas)
				do
					echo -e `./matrix $thread $size`
				done
				echo -e " " >> "tempo_de_exec_test.txt"
			done
			echo -e "\n===================================================================" >> "tempo_de_exec_test.txt"
	done

exit
