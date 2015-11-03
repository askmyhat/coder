#include "HammingCoder.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>


int main() {
  int input_length = 15;
  HammingCoder c(input_length);
  int code_length = c.GetCodeLength();
  std::valarray<bool> signal(input_length);
  std::valarray<bool> code(code_length);
//  std::valarray<bool> decoded(code_length);

  std::cout << "Signal\tCode\tBad code\tDecoded" << std::endl;

  srand(time(0));
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < input_length; ++j) {
      signal[j] = static_cast<bool>(rand() % 2);
      std::cout << signal[j];
    }

    std::cout << "\t";
    code = c.Code(signal);
    for (int j = 0; j < code_length; ++j) {
      std::cout << code[j];
    }

    std::cout << "\t";
    int error_bit = rand() % code_length;
    code[error_bit] = !code[error_bit];
    for (int j = 0; j < code_length; ++j) {
      std::cout << code[j];
    }

/*
    std::cout << "\t";
    decoded = c.Decode(code);
    for (int j = 0; j < input_length; ++j) {
      std::cout << decoded[j];
    }
*/

    std::cout << std::endl;
  }

  return 0;
}

