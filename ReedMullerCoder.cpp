#include "ReedMullerCoder.h"

ReedMullerCoder::ReedMullerCoder(int order, int length_power) :
  LinearCoder(),
  order_(order),
  length_power_(length_power)
{
  signal_length_ = 0;
  for (int i = 0; i <= order_; ++i) {
    signal_length_ += Binomial(length_power_, i);
  }

  code_length_ = pow(2, length_power_);
  generator_ = std::valarray<bool>(signal_length_ * code_length_);

  std::vector<std::valarray<bool>> monom_degree(code_length_);
  for (int i = 0; i < code_length_; ++i) {
    monom_degree[i] = std::valarray<bool>(length_power_);
    for (int j = 0; j < length_power_; ++j) {
      monom_degree[i][j] = i & (1 << (length_power_ - 1 - j));
    }
  }

  std::sort(monom_degree.begin(), monom_degree.end(), MonomDegreeOrder);

  // TODO: rename i, generator_column
  int i = 0;
  std::valarray<bool> generator_column(length_power_);
  for (int row = 0; row < signal_length_; ++row) {
    for (int column = code_length_ - 1; column >= 0; --column) {
      for (int bit = 0; bit < length_power_; ++bit) {
        generator_column[bit] = static_cast<bool>(column & (1 << (length_power_ - 1 - bit)));
      }

      if ((monom_degree[row] * generator_column).max() == 0) {
        generator_[i] = 1;
      } else {
        generator_[i] = 0;
      }

      i++;
    }
  }
}

std::valarray<bool> ReedMullerCoder::Decode(const std::valarray<bool>& code) {
  std::valarray<bool> decoded(signal_length_);

  std::vector<std::valarray<bool>> monom_degree(code_length_);
  for (std::size_t i = 0; i < code_length_; ++i) {
    monom_degree[i] = std::valarray<bool>(length_power_);
    for (std::size_t j = 0; j < length_power_; ++j) {
      monom_degree[i][j] = static_cast<bool>(i & (1 << (length_power_ - 1 - j)));
    }
  }

  std::sort(monom_degree.begin(), monom_degree.end(), MonomDegreeOrder);

  std::valarray<bool> new_code = code;
  bool vote = 0;
  int zeros = 0;
  int ones = 0;
  int weight = 0;
  int old_weight = 0;

  int code_index = 0;
  int fixed_arg_bit = 0;
  int changing_arg_bit = 0;

  for (int decoded_bit = signal_length_ - 1; decoded_bit >= 0; --decoded_bit) {
    ones = 0;
    zeros = 0;

    old_weight = weight;
    weight = MonomDegreeWeight(monom_degree[decoded_bit]);
    
    // Reducing order of code
    if (weight < old_weight) {
      for (int monom_index = 0; monom_index < signal_length_; ++monom_index) {
        if (MonomDegreeWeight(monom_degree[monom_index]) == old_weight && decoded[monom_index] == 1) {
          for (int code_index = 0; code_index < code_length_; ++code_index) {
            std::valarray<bool> code_index_binary(length_power_);
            for (int code_index_bit = 0; code_index_bit < length_power_; ++code_index_bit) {
              code_index_binary[code_index_bit] = code_index & (1 << (length_power_ - 1 - code_index_bit));
            }
            if ((code_index_binary*monom_degree[monom_index] - monom_degree[monom_index]).max() == 0) {
              new_code[code_index] ^= 1;
            }
          }
        }
      }

/*
      std::cout << "New code: ";
      for (int i = 0; i < code_length_; ++i) {
        std::cout << new_code[i];
      }
      std::cout << std::endl;
*/
    }

/*
    std::cout << std::endl;
    std::cout << std::endl;
    for (int j = 0; j < length_power_; ++j) {
      std::cout << monom_degree[decoded_bit][j];
    }
    std::cout << std::endl;
    std::cout << std::endl;
*/

    // Majority logic deduction
    for (int fixed_arg = 0; fixed_arg < pow(2, length_power_ - weight); ++fixed_arg) {
      vote = 0;
      for (int changing_arg = 0; changing_arg < pow(2, weight); ++changing_arg) {
        code_index = 0;
        fixed_arg_bit = 0;
        changing_arg_bit = 0;
        for (int arg_bit = length_power_ - 1; arg_bit >= 0; --arg_bit) {
          if (monom_degree[decoded_bit][arg_bit] == 1) {
            code_index += static_cast<bool>(changing_arg & (1 << changing_arg_bit)) * pow(2, length_power_ - 1 - arg_bit);
//            std::cout << "-" <<  static_cast<bool>(changing_arg & (1 << changing_arg_bit)) << "-";
            changing_arg_bit++;
          } else {
            code_index += static_cast<bool>(fixed_arg & (1 << fixed_arg_bit)) * pow(2, length_power_ - 1 - arg_bit);
//            std::cout << "=" << static_cast<bool>(fixed_arg & (1 << fixed_arg_bit)) << "=";
            fixed_arg_bit++;
          }
        }
//        std::cout << " Code index/value: " << code_index << " " << new_code[code_index] << std::endl;

        vote ^= new_code[code_index];
      }

//      std::cout << "Vote: " <<  vote << std::endl;
      if (vote == 1) {
        ones++;
      } else {
        zeros++;
      }
    }
    
    decoded[decoded_bit] = (ones > zeros);

//    std::cout << "Zeros/ones: " << zeros << "/" << ones << std::endl;
  }

  return decoded;
}

int ReedMullerCoder::getDistance() {
  return pow(2, length_power_ - order_);
}

long long ReedMullerCoder::Binomial(int n, int k) {
  long long result = 0;
  result = std::tgamma(n + 1) / (std::tgamma(k + 1) * std::tgamma(n - k + 1));
  return result;
}

int ReedMullerCoder::MonomDegreeWeight(const std::valarray<bool>& monom_degree) {
  int weight = 0;
  for (int i = 0; i < monom_degree.size(); ++i) {
    weight += static_cast<int>(monom_degree[i]);
  }
  return weight;
}

bool ReedMullerCoder::MonomDegreeOrder(const std::valarray<bool>& lhs, const std::valarray<bool>& rhs) {
  if (lhs.size() != rhs.size()) return 0;

  if (MonomDegreeWeight(lhs) < MonomDegreeWeight(rhs)) return 1;
  if (MonomDegreeWeight(lhs) > MonomDegreeWeight(rhs)) return 0;

  for (int i = rhs.size() - 1; i >= 0; --i) {
    if (lhs[i] > rhs[i]) return 1;
    if (lhs[i] < rhs[i]) return 0;
  }

  return false;
}

