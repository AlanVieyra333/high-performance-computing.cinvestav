// g++ -lpthread -O2 -fpermissive -w -o main.o main.cpp

#include <stdio.h>
#include <spawn.h>

#include "disperse_matrix.hpp"

disperse_matrix m1, m2;

int main(int argc, char const *argv[])
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      float data = ((i+1)*3.0) + ((j+1)*5.0);
      m1.add(data, i, j);
    }
    
  }
  
  printf("Matriz: \n");
  m1.print();

  // TODO: https://www.systutorials.com/37124/a-posix_spawn-example-in-c-to-create-child-process-on-linux/

  return 0;
}
