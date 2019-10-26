// g++ -lpthread -O2 -fpermissive -w -o main.o main.cpp

/**
 * 1) Transformar el formato de los archivos de texto a binario.
 * 2) ? Voltear la segunda matriz para su facil lectura (por filas).
 * 2) Almacenar en memoria 1Mb de datos. (10 filas/columnas).
 * 3) Dividir el cálculo de cada celda. SUM A[i]*B[i] -> 5 hilos para cada celda calculada.
 * 
 * */

#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <unistd.h>
#include <regex>
#include <sstream>
#include <fstream>

#define N 21000 // Tamano maximo de matriz.
#define MAX_THREADS 15 // Hilos cuando la matriz es de 20000*20000

using namespace std;

typedef double decimal_t;
typedef unsigned short int_t;

char *progname_convert = "./file_matrix_convert.o";
char *progname_product = "./file_matrix_product.o";
int_t m, n, p;
decimal_t buff_decimal[N];

void matrix_convert_txt_to_bin(const char *filename1, const char *filename2)
{
  pid_t pid_conv[2];
  int status[2];
  int cout_pipe[2];
  char buff[1024];
  posix_spawn_file_actions_t action;

  memset(buff, '\0', 1024);
  string prog1 = string(progname_convert) + " " + string(filename1);
  string prog2 = string(progname_convert) + " " + string(filename2);

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
          perror(string("Error al convertir los archivos a binario.(" + to_string(status[0]) + " " + to_string(status[1]) + ")").c_str());
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

void create_file_result(int_t _m, int_t _p)
{
  FILE *f = fopen("result_tmp", "wb");

  while (_m--)
    fwrite(buff_decimal, sizeof(decimal_t), _p, f);

  fclose(f);
}

void matrix_product(string filename1, string filename2)
{
  int_t NUM_PROC = (p*m) * MAX_THREADS / 400000000; // 15 procesos para 20000*20000
  if(NUM_PROC == 0) NUM_PROC = 1;
  pid_t pid_conv[NUM_PROC];
  int status[NUM_PROC];
  int_t block_size = p / NUM_PROC;
  string cmd, cmd_aux = string(progname_product) + " " + filename1 + " " + filename2 + " " + to_string(m) + " " + to_string(n) + " " + to_string(p) + " ";

  printf("Procesos: %d\n", NUM_PROC);

  for (int_t i = 0; i<NUM_PROC; i++) {
    if (i + 1 == NUM_PROC)
      cmd = cmd_aux + to_string(i * block_size) + " " + to_string(p - 1);
    else
      cmd = cmd_aux + to_string(i * block_size) + " " + to_string(((i + 1) * block_size) - 1);

    char *arg_proc[] = {"sh", "-c", (char *)cmd.c_str(), NULL};

    if (posix_spawn(&pid_conv[i], "/bin/sh", NULL, NULL, arg_proc, NULL) != 0) {
      perror("Error al invocar proceso para la conversion de la matriz 1.\n");
      //exit(EXIT_FAILURE);
      //i--; // Reintentar
    }
  }

  // Wait
  for(int_t i = 0; i<NUM_PROC; i++) {
    if (waitpid(pid_conv[i], &status[i], 0) != -1)
    {
      if (status[i] == 0)
      {
        printf("Proceso %d completado.\n", i+1);
      }
      else
      {
        fprintf(stderr, "Error al realizar la multiplicacion de matrices para el proceso %d.", i+1);
        //exit(EXIT_FAILURE);
      }
    }
    else
    {
      fprintf(stderr, "Error al intentar esperar al proceso %d.", i+1);
      //exit(EXIT_FAILURE);
    }
  }
}

void matrix_convert_bin_to_txt(int_t _m, int_t _p) {
  FILE *f_tmp = fopen("result_tmp", "rb");
  FILE *f = fopen("result.txt", "w");
  decimal_t value;

  for (int_t i=0; i<_m; i++) {
    for (int_t j=0; j<_p; j++) {
      fread(&value, sizeof(decimal_t), 1, f_tmp);
      fprintf(f, "%.6f ", value);
    }
    fseek(f, ftell(f) - 1, SEEK_SET);
    fprintf(f, "\n");
  }

  fclose(f_tmp);
  fclose(f);
}

void delete_file(string filename) {
  pid_t pid_tmp;
  string cmd = "rm " + filename;
  char *arg_proc[] = {"sh", "-c", (char *)cmd.c_str(), NULL};

  posix_spawn(&pid_tmp, "/bin/sh", NULL, NULL, arg_proc, NULL);
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("Error: main.o matrix1.txt matrix2.txt\n");
    return 1;
  }

  printf("Convirtiendo archivos a formato binario...\n");
  matrix_convert_txt_to_bin(argv[1], argv[2]);
  create_file_result(m, p);
  
  string newfilename1 = string(argv[1]) + "_tmp";
  string newfilename2 = string(argv[2]) + "_tmp";

  printf("Multiplicando matrices...\n");
  matrix_product(newfilename1, newfilename2);

  // Eliminar archivos temporales m1 m2.
  delete_file(newfilename1);
  delete_file(newfilename2);

  printf("Escribiendo resultado.\n");
  matrix_convert_bin_to_txt(m, p);
  delete_file("result_tmp");

  printf("Tarea completada, puede revisar el resultado en result.txt.\n\n");

  //matrix_convert_txt_to_bin(argv[1], argv[2]);
  //create_file_result(20000, 20000);
  //matrix_convert_bin_to_txt(20000, 20000);

  return 0;
}
