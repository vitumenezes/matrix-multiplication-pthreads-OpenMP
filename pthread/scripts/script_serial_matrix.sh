rm ../tempo_de_exec_serial.txt
touch ../tempo_de_exec_serial.txt

#Compila o código
gcc -g -Wall -o ../serial_matrix ../src/serial_matrix_calc.c

#Loop principal de execuções. São 4 tentativas

tentativas=4 #Quantas vezes o código será executado
for size in 50 100 200 400 #tamanho do problema
do
	echo -e "===================================================================\n" >> "../tempo_de_exec_serial.txt"
		for tentativa in $(seq $tentativas)
		do
			echo -e `../serial_matrix $size`
		done
		echo -e "\n===================================================================" >> "../tempo_de_exec_serial.txt"
done

exit
