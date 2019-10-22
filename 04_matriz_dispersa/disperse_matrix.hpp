#ifndef DISPERSE_MATRIX_HPP
#define DISPERSE_MATRIX_HPP

#include <stdio.h>
#include <vector>

/**
 * Disperse matrix using the CSR format (Compressed Storage Row).
 * 
 * @Author Alan Fernando Rincón VIeyra
*/
class disperse_matrix
{
private:
  std::vector<short> rows, cols;
  std::vector<double> data;
  short current_row = -1;

public:
  disperse_matrix(/* args */);
  ~disperse_matrix();
  void add(double element, short row, short col);
  void scanCSR(FILE *f);
  void print();
  disperse_matrix operator*(disperse_matrix const &m2);
};

#endif
