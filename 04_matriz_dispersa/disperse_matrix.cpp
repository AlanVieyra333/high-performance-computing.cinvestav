#include "disperse_matrix.hpp"

disperse_matrix::disperse_matrix(/* args */)
{
}

disperse_matrix::~disperse_matrix()
{
}

void disperse_matrix::add(double element, short row, short col)
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
  short i = 1, j = 1;

  while (fscanf(f, "%lf%c", &data, &tmp) != EOF)
  {
    if (data != 0.0)
      this->add(data, i, j);

    if (tmp == ' ')
      j++;
    else
    {
      i++;
      j = 1;
    }
  }

  this->rows.push_back(this->data.size() + 1);
}

void disperse_matrix::print()
{
  printf("data: ");

  for (auto &&element : data)
    printf("%f ", element);

  printf("\nrows: ");
  for (auto &&element : rows)
    printf("%d ", element);

  printf("\ncols: ");
  for (auto &&element : cols)
    printf("%d ", element);

  printf("\n");
}

disperse_matrix disperse_matrix::operator*(disperse_matrix const &m2)
{
  disperse_matrix result;
  for (short i = 1; i <= this->data.size(); i++)
  {
    short k1 = this->rows[i];
    short k2 = this->rows[i + 1] - 1;
    double sum = 0.0;

    for (short j = k1; j <= k2; j++)
    {
      sum = sum + this->data[j]; // * vec(JA(j));
    }

    //vec1(i) = sum;
  }

  return result;
}