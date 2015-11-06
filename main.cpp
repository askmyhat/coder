#include "HammingCoder.h"
#include "ReedMullerCoder.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>


int main() {
  ReedMullerCoder c(2, 3);
  int signal_length = c.GetSignalLength();
  int code_length = c.GetCodeLength();
  std::valarray<bool> signal(signal_length);
  std::valarray<bool> code(code_length);
  std::valarray<bool> decoded(signal_length);

  std::cout << "Signal > Code > Bad code > Decoded > Status" << std::endl;

  srand(time(0));
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < signal_length; ++j) {
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
    for (int j = 0; j < signal_length; ++j) {
      std::cout << decoded[j];
    }

    std::cout << "\t";
    if ((signal - decoded).max() == 0) {
      std::cout << "OK";
    } else {
      std::cout << "ERROR";
    }

*/
    std::cout << std::endl;
  }

  return 0;
}

