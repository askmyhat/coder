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

  int matrix_index = 0;
  for (int row = 1; row <= code_length_; ++row) {
    for (int column = 1; column <= code_length_; ++column) {
      if (!(row & (row - 1))) continue; // if row is power of 2
      if (!(column & (column - 1))) {
        // extra bits
        generator_[matrix_index] = static_cast<bool>(row & (1 << static_cast<int>(log2(column))));
      } else {
        // signal bits
        generator_[matrix_index] = (row == column);
      }
      ++matrix_index;
    }
  }
}

std::valarray<bool> HammingCoder::Decode(const std::valarray<bool>& code) {
  int check_length = code_length_ - signal_length_;
  int broken_bit = 0;
  std::valarray<bool> decoded_signal(signal_length_);
  std::valarray<bool> decode_matrix = std::valarray<bool>(check_length * code_length_);
  std::valarray<bool> decode_matrix_column = std::valarray<bool>(code_length_);
  std::valarray<bool> syndrome = std::valarray<bool>(check_length);

  int matrix_index = 0;
  for (int row = 1; row <= code_length_; ++row) {
    for (int column = 1; column <= check_length; ++column) {
      decode_matrix[matrix_index] = static_cast<bool>(row & (1 << (check_length - column)));
      ++matrix_index;
    }
  }

  for (int syndrome_index = 0; syndrome_index < check_length; ++syndrome_index) {
    decode_matrix_column = decode_matrix[std::slice(syndrome_index, code_length_, check_length)];
    decode_matrix_column *= code;
    syndrome[syndrome_index] = 0;
    for (int column_index = 0; column_index < code_length_; ++column_index) {
      syndrome[syndrome_index] ^= decode_matrix_column[column_index];
    }
      broken_bit += syndrome[syndrome_index] * pow(2, check_length - 1 - syndrome_index);
  }

  int decoded_signal_index = 0;
  for (int code_index = 1; code_index <= code_length_; ++code_index) {
    if (!(code_index & (code_index - 1))) continue;
    decoded_signal[decoded_signal_index] = code[code_index - 1];
    if (code_index == broken_bit) {
      decoded_signal[decoded_signal_index] ^= 1;
    }
    ++decoded_signal_index;
  }

  return decoded_signal;
}

int HammingCoder::distance() {
  return 3;
}

