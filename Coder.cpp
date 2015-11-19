#include "Coder.h"

// TODO: exception when signal_length_ < 0;
Coder::Coder() : signal_length_(0) {}
Coder::Coder(int signal_length) : signal_length_(signal_length) {}

int Coder::Weight(const std::valarray<bool>& string) {
  int weight = 0;
  for (size_t bit = 0; bit < string.size(); ++bit) {
    weight += static_cast<int>(string[bit]);
  }
  return weight;
}

