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

  int GetCodeLength();
  double GetSpeed();
  void PrintGenerator();

  std::valarray<bool> Code(const std::valarray<bool>& signal);

 protected:
  int code_length_;

  std::valarray<bool> generator_;
};

#endif // LINEAR_CODER_H

