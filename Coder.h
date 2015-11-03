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
  Coder(int signal_length) : signal_length_(signal_length) {}
  ~Coder() {};

  int GetInputLength() const { return signal_length_; };
  void SetInputLength(int new_signal_length) { signal_length_ = new_signal_length; }

 protected:
  int signal_length_;

//  std::vector<bool> Code(const std::vector<<bool>& word) const;
//  std::vector<bool> Decode(const std::vector<bool>& word) const;
//  int getDistance();
};

#endif // CODER_H

