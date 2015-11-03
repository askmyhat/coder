#include "LinearCoder.h"

LinearCoder::LinearCoder(int signal_length) : Coder(signal_length) {}

int LinearCoder::GetCodeLength() { return code_length_; }
double LinearCoder::GetSpeed() { return static_cast<double>(signal_length_) / code_length_; }

std::valarray<bool> LinearCoder::Code(const std::valarray<bool>& signal) {
  std::valarray<bool> code(code_length_);
  std::valarray<bool> temp(signal_length_);

  for (int i = 0; i < code_length_; ++i) {
    temp = generator_[std::slice(i, signal_length_, code_length_)];
    temp *= signal;
    code[i] = 0;
    for (int j = 0; j < signal_length_; ++j) {
      code[i] ^= temp[j];
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

