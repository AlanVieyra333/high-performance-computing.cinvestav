#include <stdlib.h>
#include <sstream>
#include <string.h>

#define BUF_SIZE 220000     // Tamano maximo de linea. m1-210276, m1-210300
#define N 21000             // Tamano maximo de matriz.

typedef double decimal_t;
typedef unsigned short int_t;

using namespace std;

FILE *fIn, *fOut;
char buff[BUF_SIZE];
int_t rows, cols;
decimal_t elements[N];

void convert_to_bin_by_row()
{
  //memset(buff, '\0', BUF_SIZE);
  int_t cols_aux;
  for (rows = 0; fscanf(fIn, "%[^\n]", buff) != EOF; rows++)
  {
    fgetc(fIn); // End line.
    istringstream iss(buff);

    //printf("line: %s \n", buff);

    for(cols_aux = 0; (iss >> elements[cols_aux]);){
      //printf("%lf \n", elements[cols]);
      cols_aux++;
      if(cols_aux > cols) cols = cols_aux;
    }
    //printf("\n");

    if(cols_aux == 0) break;    // End file.

    fwrite(elements, sizeof(decimal_t), cols_aux, fOut);
    memset(buff, '\0', BUF_SIZE);
  }
}

void convert_to_ascii_by_row(int_t m, int_t n) {
  decimal_t value;

  for (int_t i=0; i<m; i++) {
    for (int_t j=0; j<n; j++) {
      fread(&value, sizeof(decimal_t), 1, fIn);
      fprintf(fOut, "%.6lf ", value);
    }
    fseek(fOut, ftell(fOut) - 1, SEEK_SET);
    fprintf(fOut, "\n");
  }
}

/*
void convert_col()
{
  unsigned int endln_pos[N];
  unsigned int startln_pos[N];
  startln_pos[0] = 0;

  // Ubicar los finales de linea.
  for (rows = 0; fscanf(fIn, "%*[^\n]%*c") != EOF; rows++)
  {
    startln_pos[rows+1] = ftell(fIn);
    endln_pos[rows] = startln_pos[rows+1] - 1L;
  }

  // Recorrer por columna.
  for (cols = 0; startln_pos[rows-1] < endln_pos[rows-1]; cols++)
  {
    for (int_t i = 0; i < rows; i++)
    {
      fseek(fIn, startln_pos[i], SEEK_SET);
      fscanf(fIn, "%lf", &(elements[i]));
      startln_pos[i] = ftell(fIn);
    }

    fwrite(elements, sizeof(decimal_t), rows, fOut);
  }
}*/

void read_ascii_file(char* name) {
  string filename(name);
  string newFilename(filename + "_tmp");

  fIn = fopen(filename.c_str(), "r");
  fOut = fopen(newFilename.c_str(), "wb");
  
  convert_to_bin_by_row();

  printf("rows: %d, cols: %d\n", rows, cols);

  fclose(fIn);
  fclose(fOut);
}

void read_bin_file(char* name, int_t m, int_t n) {
  string filename(name);
  string newFilename(filename + ".txt");

  fIn = fopen(filename.c_str(), "rb");
  fOut = fopen(newFilename.c_str(), "w");
  
  convert_to_ascii_by_row(m, n);

  printf("Matriz convertida.\nrows: %d, cols: %d\n", m, n);

  fclose(fIn);
  fclose(fOut);
}

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    printf("Error: <PROGRAM> <FILE_NAME> <TO_TYPE{b|a}> <M> <N>\n");
    exit(1);
  }

  if (argv[2][0] == 'b')
  {
    read_ascii_file((char*) argv[1]);
  } else {
    int_t m = atoi(argv[3]);
    int_t n = atoi(argv[4]);
    read_bin_file((char*) argv[1], m, n);
  }

  return 0;
}
