#include "disperse_matrix.hpp"

#include <stdio.h>

disperse_matrix::disperse_matrix(/* args */)
{
}

disperse_matrix::~disperse_matrix()
{
}

void disperse_matrix::scan() {

}

void disperse_matrix::add(float element, int row, int col) {
  // Add new row
  if(row > current_row){
    rows.push_back(data.size());
    current_row = row;
  }
  cols.push_back(col);
  data.push_back(element);
}

void disperse_matrix::print() {
  printf("data:\n");

  for (auto&& element : data) printf("%f ", element);

  printf("\nrows:\n");
  for (auto&& element : rows) printf("%d ", element);

  printf("\ncols:\n");
  for (auto&& element : cols) printf("%d ", element);

  printf("\n");
}
