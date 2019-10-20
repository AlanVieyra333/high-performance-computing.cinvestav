#ifndef DISPERSE_MATRIX_HPP
#define DISPERSE_MATRIX_HPP

#include <vector>

/**
 * Disperse matrix using the CRS format (Compressed Storage Row).
 * 
 * @Author Alan Fernando Rincón VIeyra
*/
class disperse_matrix
{
private:
  std::vector<int> rows, cols;
  std::vector<float> data;
  int current_row = -1, current_col = -1;
public:
  disperse_matrix(/* args */);
  ~disperse_matrix();
  void scan();
  void add(float element, int row, int col);
  void print();
};

#endif
