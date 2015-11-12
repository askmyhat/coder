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
  int distance();

 protected:
  int order_;
  int length_power_;
  static int Binomial(int n, int k);
  static bool MonomDegreeOrder(const std::valarray<bool>& lhs, const std::valarray<bool>& rhs);
};

/*
class ReedMullerSystematicCoder : public ReedMullerCoder {
 public:
  ReedMullerSystematicCoder(int order, int length_power);

  std::valarray<bool> Code(const std::valarray<bool>& code);
  std::valarray<bool> Decode(const std::valarray<bool>& code);
  std::valarray<bool> ToSystematic(const std::valarray<bool>& code);
  std::valarray<bool> FromSystematic(const std::valarray<bool>& code);

  void PrintSystematicFactor();

 private:
  std::valarray<bool> systematic_factor_;
  std::valarray<bool> systematic_divider_;
};
*/

#endif // REED_MULLER_CODER_H

