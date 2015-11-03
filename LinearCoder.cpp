#include "LinearCoder.h"

LinearCoder::LinearCoder(int signal_length) : Coder(signal_length) {}

int LinearCoder::GetCodeLength() { return code_length_; }
double LinearCoder::GetSpeed() { return static_cast<double>(signal_length_) / code_length_; }

std::valarray<bool> LinearCoder::Code(const std::valarray<bool>& signal) {
  std::valarray<bool> code(code_length_);
  std::valarray<bool> generator_column(signal_length_);

  for (int column = 0; column < code_length_; ++column) {
    generator_column = generator_[std::slice(column, signal_length_, code_length_)];
    generator_column *= signal;
    code[column] = 0;
    for (int j = 0; j < signal_length_; ++j) {
      code[column] ^= generator_column[j];
    }
  }

  return code;
}

void LinearCoder::PrintGenerator() {
  for (int i = 0; i < generator_.size(); ++i) {
    std::cout << generator_[i] << " ";
    if ((i + 1) % code_length_ == 0) {
      std::cout << std::endl;
    }
  }
}

