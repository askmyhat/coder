#ifndef REED_MULLER_CODER_H
#define REED_MULLER_CODER_H

#include <cmath>
#include <iostream>
#include <valarray>
#include <numeric>

#include "LinearCoder.h"

class ReedMullerCoder : public LinearCoder {
 public:
  ReedMullerCoder(int order, int length_power);
  std::valarray<bool> Decode(const std::valarray<bool>& code);
  int getDistance();

 private:
  static long long Binomial(int n, int k);
  static long long Factorial(int n);
  static bool MonomDegreeOrder(const std::valarray<bool>& lhs, const std::valarray<bool>& rhs);
};

#endif // REED_MULLER_CODER_H

