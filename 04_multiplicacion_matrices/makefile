INTERFACES= 
DEPS= 
ARGS=-O2 -w
MAIN=main.cpp

MATRIZ_1=./test/matriz_a.txt# test/matriz_a.txt
MATRIZ_2=./test/matriz_b.txt# test/matriz_b.txt

compile: main.o file_matrix_convert.o file_matrix_product.o
	@echo "Compilado"

run: main.o file_matrix_convert.o file_matrix_product.o
	time ./main.o ${MATRIZ_1} ${MATRIZ_2}

run2: file_matrix_convert.o
	time ./file_matrix_convert.o ${MATRIZ_1} b &
	time ./file_matrix_convert.o ${MATRIZ_2} b

run3: file_matrix_product.o
	truncate -s 0 result_tmp
	truncate -s 3200000000 result_tmp
	time ./file_matrix_product.o  ${MATRIZ_1}_tmp ${MATRIZ_2}_tmp 20000 20000 20000 0 49

run4:
	echo "Escribiendo resultado..."
	time ./file_matrix_convert.o result_tmp a 20000 20000

file_matrix_product.o: file_matrix_product.cpp
	@g++ -o file_matrix_product.o -O2 -lpthread file_matrix_product.cpp

file_matrix_convert.o: file_matrix_convert.cpp
	@g++ -o file_matrix_convert.o -O2 file_matrix_convert.cpp

main.o: ${MAIN} ${DEPS} ${INTERFACES}
	@g++ -o main.o ${ARGS} ${MAIN} ${DEPS}