INTERFACES = 
DEPS = 
ARGS = -lpthread -O2 -fpermissive -w
MAIN = main.cpp

run: main.o
	./main.o

main.o: ${MAIN} ${DEPS} ${INTERFACES}
	g++ -o main.o ${ARGS} ${MAIN} ${DEPS}