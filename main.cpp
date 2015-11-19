#include "HammingCoder.h"
#include "ReedMullerCoder.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>


int main() {
  int first_param = 2;
  int second_param = 5;
  int iterations = 3;

  ReedMullerSystematicCoder c(first_param, second_param);
  int signal_length = c.signal_length();
  int code_length = c.code_length();
  int error_length = c.distance() / 2 - 1;
  int error_bit = 0;

  std::valarray<bool> signal(signal_length);
  std::valarray<bool> code(code_length);
  std::valarray<bool> decoded(signal_length);

  std::cout << "RM(" << first_param << ", " << second_param << "): ";
  std::cout << "signal_length =  " << signal_length << ", code_length = " << code_length;
  std::cout << ", error_length = " << error_length << std::endl;
  std::cout << std::endl;

  srand(time(0));
  for (int i = 0; i < iterations; ++i) {
    std::cout << "  Signal: ";
    for (int j = 0; j < signal_length; ++j) {
      signal[j] = static_cast<bool>(rand() % 2);
      std::cout << signal[j];
    }

    std::cout << std::endl;
    std::cout << "    Code: ";
    code = c.Code(signal);
    for (int j = 0; j < code_length; ++j) {
      std::cout << code[j];
    }

    std::cout << std::endl;
    std::cout << "Bad code: ";
    for (int j = 0; j < error_length; ++j) {
      error_bit = rand() % code_length;
      code[error_bit] ^= 1;
    }
    for (int j = 0; j < code_length; ++j) {
      std::cout << code[j];
    }

    std::cout << std::endl;
    std::cout << " Decoded: ";
    decoded = c.Decode(code);
    for (int j = 0; j < signal_length; ++j) {
      std::cout << decoded[j];
    }

    std::cout << std::endl;
    std::cout << "  Status: ";
    if ((signal - decoded).max() == 0) {
      std::cout << "OK";
    } else {
      std::cout << "ERROR";
    }

    std::cout << std::endl;
    std::cout << std::endl;
  }

  return 0;
}

