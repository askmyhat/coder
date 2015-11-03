#ifndef HAMMING_CODER_H
#define HAMMING_CODER_H

#include <cmath>
#include <iostream>
#include <valarray>
#include <numeric>

#include "LinearCoder.h"

class HammingCoder : public LinearCoder {
 public:
  HammingCoder (int signal_length);
  std::valarray<bool> Decode(const std::valarray<bool>& code);
  int getDistance();
};

#endif // HAMMING_CODER_H

