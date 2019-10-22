// g++ -lpthread -O2 -fpermissive -w -o main.o main.cpp

#include <stdio.h>
#include <spawn.h>

#include "disperse_matrix.hpp"

FILE *f1, *f2;
disperse_matrix m1, m2, m3;

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("Error: main.o matrix1.txt matrix2.txt\n");
    return 1;
  }

  f1 = fopen(argv[1], "r");
  f2 = fopen(argv[2], "r");
  
  m1.scanCSR(f1);
  printf("Matriz 1: \n");
  m1.print();

  m2.scanCSR(f2);
  printf("\nMatriz 2: \n");
  m2.print();

  m3 = m1 * m2;
  printf("\nMatriz 3: \n");
  m3.print();

  // TODO: https://www.systutorials.com/37124/a-posix_spawn-example-in-c-to-create-child-process-on-linux/
  fclose(f1);
  fclose(f2);

  return 0;
}
