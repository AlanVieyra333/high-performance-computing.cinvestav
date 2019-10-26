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

void convert_row()
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

void read_file(char* name) {
  string filename(name);
  string newFilename(filename + "_tmp");

  fIn = fopen(filename.c_str(), "r");
  fOut = fopen(newFilename.c_str(), "wb");
  
  convert_row();

  printf("rows: %d, cols: %d\n", rows, cols);

  fclose(fIn);
  fclose(fOut);
}

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    printf("Error: <PROGRAM> <FILE_NAME>\n");
    exit(1);
  }

  read_file((char*) argv[1]);

  return 0;
}
