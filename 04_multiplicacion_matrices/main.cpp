// g++ -lpthread -O2 -fpermissive -w -o main.o main.cpp

/**
 * 1) Transformar el formato de los archivos de texto a binario.
 * 2) ? Voltear la segunda matriz para su facil lectura (por filas).
 * 2) Almacenar en memoria 1Mb de datos. (10 filas/columnas).
 * 3) Dividir el cálculo de cada celda. SUM A[i]*B[i] -> 5 hilos para cada celda calculada.
 * 
 * */

#include <stdio.h>
#include <spawn.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <unistd.h>
#include <regex>
#include <sstream>

#define N 21000 // Tamano maximo de matriz.

using namespace std;

typedef float decimal_t;

char *progname_convert = "./file_matrix_convert.o";
char *progname_product = "./file_matrix_product.o";
unsigned short m, n, p;
decimal_t buff_decimal[N];

void matrix_convert(const char *filename1, const char *filename2)
{
  pid_t pid_conv[2];
  int status[2];
  int cout_pipe[2];
  char buff[1024];
  posix_spawn_file_actions_t action;

  memset(buff, '\0', 1024);
  string prog1 = string(progname_convert) + " " + string(filename1) + " r";
  string prog2 = string(progname_convert) + " " + string(filename2) + " r";

  char *argM1[] = {"sh", "-c", (char *)prog1.c_str(), NULL};
  char *argM2[] = {"sh", "-c", (char *)prog2.c_str(), NULL};

  if (pipe(cout_pipe))
    perror("Error con funcion pipe.");

  posix_spawn_file_actions_init(&action);
  posix_spawn_file_actions_addclose(&action, cout_pipe[0]);
  posix_spawn_file_actions_adddup2(&action, cout_pipe[1], 1);
  posix_spawn_file_actions_addclose(&action, cout_pipe[1]);

  if (posix_spawn(&pid_conv[0], "/bin/sh", &action, NULL, argM1, NULL) == 0)
  {
    if (posix_spawn(&pid_conv[1], "/bin/sh", &action, NULL, argM2, NULL) == 0)
    {
      close(cout_pipe[1]);

      if (waitpid(pid_conv[0], &status[0], 0) != -1 &&
          waitpid(pid_conv[1], &status[1], 0) != -1)
      {
        if (status[0] == 0 && status[1] == 0)
        {
          read(cout_pipe[0], &buff[0], 1024);
          string out = regex_replace(buff, regex("[^0-9]+"), string(" "));
          istringstream ss(out);
          int nAux;

          ss >> m >> n >> nAux >> p;

          if (n != nAux)
          {
            perror("Error: El numero de columnas de la matriz A no coincide con el numero de filas de la matriz B.");
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          perror("Error al convertir los archivos a binario.");
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        perror("Error al intentar esperar a los procesos.");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      // Parar al proceso 1.
      kill(pid_conv[0], SIGTERM);
      perror("Error al invocar proceso para la conversion de la matriz 2.\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    perror("Error al invocar proceso para la conversion de la matriz 1.\n");
    exit(EXIT_FAILURE);
  }
}

void create_file_result()
{
  FILE *f = fopen("result_tmp", "wb");
  int size = m;

  while (size--)
    fwrite(buff_decimal, sizeof(decimal_t), p, f);

  fclose(f);
}

void matrix_product(const char *filename1, const char *filename2)
{
  pid_t pid_conv[10];
  int status[10];
  string prog_aux = string(progname_product) + " " + string(filename1) + " " + string(filename2) + " " + to_string(m) + " " + to_string(n) + " " + to_string(p) + " ";
  string prog[] = {
      prog_aux + " 0 0",
  };

  char *arg_proc[] = {"sh", "-c", (char *)prog[0].c_str(), NULL};

  if (posix_spawn(&pid_conv[0], "/bin/sh", NULL, NULL, arg_proc, NULL) == 0)
  {
    if (waitpid(pid_conv[0], &status[0], 0) != -1)
    {
      if (status[0] == 0)
      {
        printf("Completado.\n");
      }
      else
      {
        perror("Error al convertir los archivos a binario.");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      perror("Error al intentar esperar a los procesos.");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    perror("Error al invocar proceso para la conversion de la matriz 1.\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("Error: main.o matrix1.txt matrix2.txt\n");
    return 1;
  }

  matrix_convert(argv[1], argv[2]);
  create_file_result();

  string newfilename1 = string(argv[1]) + "_tmp";
  string newfilename2 = string(argv[2]) + "_tmp";

  matrix_product(newfilename1.c_str(), newfilename2.c_str());

  return 0;
}
