#ifndef DISPERSE_MATRIX_HPP
#define DISPERSE_MATRIX_HPP

#include <stdio.h>
#include <vector>

typedef unsigned int uint;

/**
 * Disperse matrix using the CSR format (Compressed Storage Row).
 * 
 * @Author Alan Fernando Rincón VIeyra
*/
class disperse_matrix
{
private:
  std::vector<uint> rows, cols;
  std::vector<double> data;
  uint current_row = -1;
  uint m = 0, n = 0; // m - totoal rows, n - total cols

public:
  disperse_matrix(/* args */);
  ~disperse_matrix();
  void add(double element, uint row, uint col);
  void scanCSR(FILE *f);
  void print();
  disperse_matrix operator*(disperse_matrix const &m2);
};

#endif
