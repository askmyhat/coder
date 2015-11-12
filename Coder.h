#ifndef CODER_H
#define CODER_H

#include <cmath>
#include <iostream>
#include <valarray>
#include <numeric>

// TODO: exception when signal_length_ < 0;
// TODO: make Coder abstract;
class Coder {
 public:
  Coder() : signal_length_(0) {}
  Coder(int signal_length) : signal_length_(signal_length) {}
  virtual ~Coder() {};

  int signal_length() const { return signal_length_; };

  static std::size_t Weight(const std::valarray<bool>& string) {
    std::size_t weight = 0;
    for (int bit = 0; bit < string.size(); ++bit) {
      weight += static_cast<int>(string[bit]);
    }
    return weight;
  }

 protected:
  int signal_length_;

  virtual std::valarray<bool> Code(const std::valarray<bool>& word) = 0;
  virtual std::valarray<bool> Decode(const std::valarray<bool>& word) = 0;
  virtual int distance() = 0;
};

#endif // CODER_H

