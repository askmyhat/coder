#include "HammingCoder.h"

HammingCoder::HammingCoder(int signal_length) :
  LinearCoder(signal_length)
{
  int check_length = 0;
  for (check_length = 0; code_length_ - signal_length < check_length; ++check_length) {
    code_length_ = pow(2, check_length + 1) - 1;
  }
  code_length_ -= pow(2, check_length) - 1 - check_length - signal_length_;

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
  int check_length = code_length_ - signal_length_;
  int broken_bit = 0;
  std::valarray<bool> decoded(signal_length_);
  std::valarray<bool> decode_matrix = std::valarray<bool>(check_length * code_length_);
  std::valarray<bool> decode_matrix_column = std::valarray<bool>(code_length_);
  std::valarray<bool> syndrome = std::valarray<bool>(check_length);

  int i = 0;
  for (int row = 1; row <= code_length_; ++row) {
    for (int column = 1; column <= check_length; ++column) {
      decode_matrix[i] = static_cast<bool>(row & (1 << (check_length - column)));
      i++;
    }
  }

  for (int bit_number = 0; bit_number < check_length; ++bit_number) {
    decode_matrix_column = decode_matrix[std::slice(bit_number, code_length_, check_length)];
    decode_matrix_column *= code;
    syndrome[bit_number] = 0;
    for (int j = 0; j < code_length_; ++j) {
      syndrome[bit_number] ^= decode_matrix_column[j];
    }
    broken_bit += syndrome[bit_number] * pow(2, check_length - 1 - bit_number);
  }

  i = 0;
  for (int bit_number = 1; bit_number <= code_length_; ++bit_number) {
    if (!(bit_number & (bit_number - 1))) continue;
    decoded[i] = code[bit_number - 1];
    if (bit_number == broken_bit) {
      decoded[i] ^= 1;
    }
    i++;
  }

  return decoded;
}

int HammingCoder::getDistance() {
  return 3;
}

