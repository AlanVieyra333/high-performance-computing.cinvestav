INTERFACES = disperse_matrix.hpp
DEPS = disperse_matrix.cpp
ARGS = -O2
MAIN = main.cpp

run: main.o
	./main.o test/matriz1.txt test/matriz_a.txt

main.o: ${MAIN} ${DEPS} ${INTERFACES}
	@g++ -o main.o ${ARGS} ${MAIN} ${DEPS}
	@echo "Compilado"