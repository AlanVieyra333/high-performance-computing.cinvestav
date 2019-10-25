#include <stdlib.h>
#include <sstream>

#define BUF_SIZE 220000     // Tamano maximo de linea. m1-210276, m1-210300
#define N 21000             // Tamano maximo de matriz.

typedef float decimal_t;
typedef unsigned short int_t;

using namespace std;

FILE *fIn, *fOut;
char buff[BUF_SIZE];
int_t rows, cols;
decimal_t elements[N];

void convert_row()
{
  for (rows = 0; fscanf(fIn, "%[^\n]%*c", buff) != EOF; rows++)
  {
    istringstream iss(buff);
    cols = 0;

    while((iss >> elements[cols]) ){
      cols++;
      //printf("%f ", element);
    }
    //printf("\n");

    fwrite(elements, sizeof(decimal_t), cols, fOut);
  }
}

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
      fscanf(fIn, "%f", &(elements[i]));
      startln_pos[i] = ftell(fIn);
    }

    fwrite(elements, sizeof(decimal_t), rows, fOut);
  }
}

void read_file(char* name, char type) {
  string filename(name);
  string newFilename(filename + "_tmp");

  fIn = fopen(filename.c_str(), "r");
  fOut = fopen(newFilename.c_str(), "wb");

  if (type == 'c')
  {
    convert_col();
  } else {
    convert_row();
  }

  printf("rows: %d, cols: %d\n", rows, cols);

  fclose(fIn);
  fclose(fOut);
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("Error: <PROGRAM> <FILE_NAME> {r|c}\n");
    exit(1);
  }

  read_file((char*) argv[1], argv[2][0]);

  return 0;
}
