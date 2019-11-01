#include <stdlib.h>
#include <fstream>

#define N 21000             // Tamano maximo de matriz.
#define MAX_THREADS 220L     // Hilos cuando la matriz es de 20000*1333

typedef double decimal_t;
typedef unsigned short int_t;

int_t m, n, p;
char* filename_m1;
char* filename_m2;

typedef struct _block {
  int_t start;
  int_t end;
} block;

decimal_t vector_product(decimal_t *vA, decimal_t *vB, int_t ini, int_t end) {
  decimal_t result = 0;

  for (int_t i = ini; i <= end; i++)
  {
    result += vA[i] * vB[i];
  }
  
  return result;
}

void read_col(FILE *f, decimal_t *v, int_t col) {
  fseek(f, sizeof(decimal_t) * col, SEEK_SET);

  for (int_t i = 0; i < n; i++)  // Filas
  {
    fread(&(v[i]), sizeof(decimal_t), 1, f);
    fseek(f, ftell(f) + (sizeof(decimal_t) * (p - 1)), SEEK_SET);
  }
}

void* thread_func(void* arg) {
  block *my_block = (block*) arg;

  FILE *f1, *f2, *f3;         // Matriz 1, 2 y resultado
  decimal_t vA[N], vB[N];

  f1 = fopen(filename_m1, "rb");
  f2 = fopen(filename_m2, "rb");
  f3 = fopen("result_tmp", "rb+");

  //printf("hilo: %d %d\n", my_block->start, my_block->end);

  for (int_t j = my_block->start; j <= my_block->end; j++)  // Columnas
  {
    read_col(f2, vB, j);

    fseek(f1, 0, SEEK_SET);
    for (int_t i = 0; i < m; i++) // Filas
    {
      fread(vA, sizeof(decimal_t), n, f1);

      decimal_t cell_result = vector_product(vA, vB, 0, n-1);
      //printf("Cell prod: %lf\n", cell_result);
      fseek(f3, sizeof(decimal_t) * ((i * p) + j), SEEK_SET);
      fwrite(&cell_result, sizeof(decimal_t), 1, f3);
    }
  }

  fclose(f1);
  fclose(f2);
  fclose(f3);

  return NULL;
}

void thread_manager(int_t col_start, int_t col_end)
{
  int_t cols_proc = ((col_end - col_start) + 1);
  int_t NUM_THREADS = (cols_proc * m * MAX_THREADS) / (20000L*1333L); // Hilos para leer todas las filas
  //int_t NUM_THREADS = MAX_THREADS;
  if(NUM_THREADS == 0) NUM_THREADS = 1;
  pthread_t threads[NUM_THREADS];
  block blocks[NUM_THREADS];
  int_t block_size = cols_proc / NUM_THREADS;

  printf("Se crearon %d hilos.\n", NUM_THREADS);

  for (int_t i = 0; i < NUM_THREADS; i++)
  {
    blocks[i].start = (i * block_size);
    blocks[i].end = ((i + 1) * block_size) - 1;
    if (i + 1 == NUM_THREADS)
      blocks[i].end = col_end;

    //printf("block: %d %d\n", blocks[i].start, blocks[i].end);

    pthread_create(&(threads[i]), NULL, thread_func, &blocks[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(threads[i], NULL);
}

/*
void matrix_product(int_t start_col, int_t end_col)
{
  for (int_t j = start_col; j <= end_col; j++)  // Columnas
  {
    fseek(f1, 0, SEEK_SET);
    read_m2_col(j);

    for (int_t i = 0; i < m; i++) // Filas
    {
      fread(vA, sizeof(decimal_t), n, f1);

      decimal_t cell_result = vector_product(0, n-1);
      //printf("Cell prod: %lf\n", cell_result);
      fseek(f3, sizeof(decimal_t) * ((i * p) + j), SEEK_SET);
      fwrite(&cell_result, sizeof(decimal_t), 1, f3);
    }
  }
}*/

int main(int argc, char const *argv[])
{
  if (argc != 8)
  {
    printf("Error: main.o matrix1_bin matrix2_bin <M> <N> <P> <START_COL> <END_COL>\n");
    return 1;
  }

  filename_m1 = (char*) argv[1];
  filename_m2 = (char*) argv[2];

  m = atoi(argv[3]);
  n = atoi(argv[4]);
  p = atoi(argv[5]);

  int col_start = atoi(argv[6]);
  int col_end = atoi(argv[7]);

  thread_manager(col_start, col_end);

  return 0;
}
