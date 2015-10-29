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

class LinearCoder : public Coder {
 public:
  LinearCoder(int signal_length) : Coder(signal_length) {}

  int GetCodeLength() { return code_length_; }
  double GetSpeed() { return static_cast<double>(signal_length_) / code_length_; }

  void PrintGenerator() {
    for (int i = 0; i < generator_.size(); ++i) {
      std::cout << generator_[i] << " ";
      if ((i + 1) % code_length_ == 0) {
        std::cout << std::endl;
      }
    }
  }

 protected:
  int code_length_;
  std::valarray<bool> generator_;
};

class HammingCoder : public LinearCoder {
 public:
  HammingCoder (int signal_length) :
    LinearCoder(signal_length)
  {
    int parity_bits = 0;
    for (parity_bits = 0; code_length_ - signal_length < parity_bits; ++parity_bits) {
      code_length_ = pow(2, parity_bits + 1) - 1;
    }
    code_length_ -= pow(2, parity_bits) - 1 - parity_bits - signal_length_;

    generator_ = std::valarray<bool>(signal_length_ * code_length_);

    int i = 0;
    for (int row = 1; row <= code_length_; ++row) {
      for (int column = 1; column <= code_length_; ++column) {
        if (!(row & (row - 1))) continue; // if row is power of 2
        if (!(column & (column - 1))) {
          generator_[i] = static_cast<bool>(row & (1 << static_cast<int>(log2(column))));
        } else {
          generator_[i] = (row == column);
        }
        i++;
      }
    }
  }

  std::valarray<bool> Code(const std::valarray<bool>& signal) {
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

  std::valarray<bool> Decode(const std::valarray<bool>& code) {
    std::valarray<bool> decoded(signal_length_);

    return decoded;
  }

  int getDistance() {
    return 3;
  }
};

