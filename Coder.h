#ifndef CODER_H
#define CODER_H


#include <cmath>
#include <iostream>
#include <valarray>
#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

class Coder {
 public:
  Coder();
  Coder(int signal_length);
  virtual ~Coder() {};

  inline int signal_length() const;
  inline int code_length() const;
  inline double speed() const;
  virtual int distance() = 0;

  virtual std::valarray<bool> Code(const std::valarray<bool>& word) = 0;
  virtual std::valarray<bool> Decode(const std::valarray<bool>& word) = 0;

  static int Weight(const std::valarray<bool>& string);

 protected:
  int signal_length_;
  int code_length_;
};

int Coder::signal_length() const {
  return signal_length_;
}

int Coder::code_length() const {
  return code_length_;
}

double Coder::speed() const {
  return static_cast<double>(signal_length_) / code_length_;
}

#endif // CODER_H

