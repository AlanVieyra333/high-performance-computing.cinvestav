INTERFACES = disperse_matrix.hpp
DEPS = disperse_matrix.cpp
ARGS = -O2
MAIN = main.cpp

run: main.o
	./main.o

main.o: ${MAIN} ${DEPS} ${INTERFACES}
	@g++ -o main.o ${ARGS} ${MAIN} ${DEPS}
	@echo "Compilado"