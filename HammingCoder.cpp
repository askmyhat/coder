#include "HammingCoder.h"

HammingCoder::HammingCoder (int signal_length) :
  LinearCoder(signal_length)
{
  int parity_bits = 0;
  for (parity_bits = 0; code_length_ - signal_length < parity_bits; ++parity_bits) {
    code_length_ = pow(2, parity_bits + 1) - 1;
  }
  code_length_ -= pow(2, parity_bits) - 1 - parity_bits - signal_length_;

  generator_ = std::valarray<bool>(signal_length_ * code_length_);

  int i = 0;
  for (int row = 1; row <= code_length_; ++row) {
    for (int column = 1; column <= code_length_; ++column) {
      if (!(row & (row - 1))) continue; // if row is power of 2
      if (!(column & (column - 1))) {
        generator_[i] = static_cast<bool>(row & (1 << static_cast<int>(log2(column))));
      } else {
        generator_[i] = (row == column);
      }
      i++;
    }
  }
}

std::valarray<bool> HammingCoder::Decode(const std::valarray<bool>& code) {
  std::valarray<bool> decoded(signal_length_);
  return decoded;
}

int HammingCoder::getDistance() {
  return 3;
}

