#include "disperse_matrix.hpp"

disperse_matrix::disperse_matrix(/* args */)
{
}

disperse_matrix::~disperse_matrix()
{
}

void disperse_matrix::add(double element, uint row, uint col)
{
  // Add new row
  if (row > current_row)
  {
    rows.push_back(data.size() + 1);
    current_row = row;
  }
  cols.push_back(col);
  data.push_back(element);
}

void disperse_matrix::scanCSR(FILE *f)
{
  double data;
  char tmp;
  uint i = 1, j = 1;

  while (fscanf(f, "%lf%c", &data, &tmp) != EOF)
  {
    if (data != 0.0)
      this->add(data, i, j);

    if (tmp == ' ')
      j++;
    else
    {
      if(this->n == 0) this->n = j;
      i++;
      j = 1;
    }
  }

  this->rows.push_back(this->data.size() + 1);
  this->m = i;
}

void disperse_matrix::print()
{
  printf("Tamano: m: %d, n: %d\n", this->m, this->n);
  /*
  printf("data: ");
  for (auto &&element : data)
    printf("%f ", element);

  printf("\nrows: ");
  for (auto &&element : rows)
    printf("%d ", element);

  printf("\ncols: ");
  for (auto &&element : cols)
    printf("%d ", element);

  printf("\n"); //*/
}

disperse_matrix disperse_matrix::operator*(disperse_matrix const &m2)
{
  disperse_matrix result;
  for (uint i = 1; i <= this->data.size(); i++)
  {
    uint k1 = this->rows[i];
    uint k2 = this->rows[i + 1] - 1;
    double sum = 0.0;

    for (uint j = k1; j <= k2; j++)
    {
      sum = sum + this->data[j]; // * vec(JA(j));
    }

    //vec1(i) = sum;
  }

  return result;
}