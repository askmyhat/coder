#ifndef LINEAR_CODER_H
#define LINEAR_CODER_H

#include <cmath>
#include <iostream>
#include <valarray>
#include <numeric>

#include "Coder.h"

class LinearCoder : public Coder {
 public:
  LinearCoder();
  LinearCoder(int signal_length);

  int code_length();
  double speed();
  void PrintGenerator();

  std::valarray<bool> Code(const std::valarray<bool>& signal);
  static std::valarray<bool> MultiplyVectorToMatrix(const std::valarray<bool>& string, const std::valarray<bool>& matrix);

 protected:
  int code_length_;

  std::valarray<bool> generator_;
  void PrintMatrix(const std::valarray<bool>& matrix, int columns);
};

#endif // LINEAR_CODER_H

