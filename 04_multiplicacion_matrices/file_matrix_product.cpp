#include <stdlib.h>
#include <fstream>

#define N 21000             // Tamano maximo de matriz.

typedef float decimal_t;
typedef unsigned short int_t;

FILE *f1, *f2, *f3;         // Matriz 1, 2 y resultado
int_t m, n, p;
decimal_t vA[N], vB[N];

void readCol(int_t col) {
  fseek(f2, sizeof(decimal_t) * col, SEEK_SET);

  for (int_t i = 0; i < n; i++)  // Filas
  {
    fread(&(vB[i]), sizeof(decimal_t), 1, f2);
    fseek(f2, ftell(f2) + (sizeof(decimal_t) * (p - 1)), SEEK_SET);
  }
}

decimal_t vector_product(int_t ini, int_t end) {
  decimal_t result = 0;

  for (int_t i = ini; i <= end; i++)
  {
    result += vA[i] * vB[i];
  }
  
  return result;
}

void matrix_product(int_t start_col, int_t end_col)
{
  for (int_t j = start_col; j <= end_col; j++)  // Columnas
  {
    fseek(f1, 0, SEEK_SET);
    readCol(j);

    for (int_t i = 0; i < m; i++) // Filas
    {
      fread(vA, sizeof(decimal_t), n, f1);

      decimal_t cell_result = vector_product(0, n-1);
      //printf("Cell prod: %f\n", cell_result);
      fseek(f3, sizeof(decimal_t) * ((i * p) + j), SEEK_SET);
      fwrite(&cell_result, sizeof(decimal_t), 1, f3);
    }
  }
}

int main(int argc, char const *argv[])
{
  if (argc != 8)
  {
    printf("Error: main.o matrix1_bin matrix2_bin <M> <N> <P> <START_COL> <END_COL>\n");
    return 1;
  }

  f1 = fopen(argv[1], "rb");
  f2 = fopen(argv[2], "rb");
  f3 = fopen("result_tmp", "w+");

  m = atoi(argv[3]);
  n = atoi(argv[4]);
  p = atoi(argv[5]);

  int start_col = atoi(argv[6]);
  int end_col = atoi(argv[7]);

  matrix_product(start_col, end_col);

  fclose(f1);
  fclose(f2);
  fclose(f3);

  return 0;
}
