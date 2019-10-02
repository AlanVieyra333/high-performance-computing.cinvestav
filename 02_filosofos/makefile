INTERFACES = 
DEPS = 

run: main.o
	./main.o

main.o: main.c ${DEPS} ${INTERFACES}
	gcc -lpthread -fpermissive -w -o main.o -O2 main.cpp ${DEPS}