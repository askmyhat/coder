#include "ReedMullerCoder.h"

ReedMullerCoder::ReedMullerCoder(int order, int length_param) :
  LinearCoder(),
  order_(order),
  length_param_(length_param)
{
  signal_length_ = 0;
  for (int i = 0; i <= order_; ++i) {
    signal_length_ += Binomial(length_param_, i);
  }

  code_length_ = pow(2, length_param_);
  generator_ = std::valarray<bool>(signal_length_ * code_length_);

  std::vector<std::valarray<bool>> monom_degree(code_length_);
  for (int i = 0; i < code_length_; ++i) {
    monom_degree[i] = std::valarray<bool>(length_param_);
    for (int j = 0; j < length_param_; ++j) {
      monom_degree[i][j] = i & (1 << (length_param_ - 1 - j));
    }
  }

  std::sort(monom_degree.begin(), monom_degree.end(), MonomDegreeOrder);

  int generator_index = 0;
  std::valarray<bool> generator_column(length_param_);
  for (int row = 0; row < signal_length_; ++row) {
    for (int column = code_length_ - 1; column >= 0; --column) {
      for (int column_bit = 0; column_bit < length_param_; ++column_bit) {
        generator_column[column_bit] = static_cast<bool>(column & (1 << (length_param_ - 1 - column_bit)));
      }
      generator_[generator_index] = ((monom_degree[row] * generator_column).max() == 0);
      generator_index++;
    }
  }
}

std::valarray<bool> ReedMullerCoder::Decode(const std::valarray<bool>& code) {
  std::valarray<bool> decoded_signal(signal_length_);

  std::vector<std::valarray<bool>> monom_degree(code_length_);
  for (int i = 0; i < code_length_; ++i) {
    monom_degree[i] = std::valarray<bool>(length_param_);
    for (int j = 0; j < length_param_; ++j) {
      monom_degree[i][j] = static_cast<bool>(i & (1 << (length_param_ - 1 - j)));
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

  for (int decoded_signal_bit = signal_length_ - 1; decoded_signal_bit >= 0; --decoded_signal_bit) {
    ones = 0;
    zeros = 0;

    old_weight = weight;
    weight = Weight(monom_degree[decoded_signal_bit]);
    
    // Reducing order of code
    if (weight < old_weight) {
      for (int monom_index = 0; monom_index < signal_length_; ++monom_index) {
        if (Weight(monom_degree[monom_index]) == old_weight && decoded_signal[monom_index] == 1) {
          for (int code_index = 0; code_index < code_length_; ++code_index) {
            std::valarray<bool> code_index_binary(length_param_);
            for (int code_index_bit = 0; code_index_bit < length_param_; ++code_index_bit) {
              code_index_binary[code_index_bit] =
                  code_index & (1 << (length_param_ - 1 - code_index_bit));
            }
            if ((code_index_binary * monom_degree[monom_index] - monom_degree[monom_index]).max() == 0) {
              new_code[code_index] ^= 1;
            }
          }
        }
      }
    }

    // Majority logic deduction
    for (int fixed_arg = 0; fixed_arg < pow(2, length_param_ - weight); ++fixed_arg) {
      vote = 0;
      for (int changing_arg = 0; changing_arg < pow(2, weight); ++changing_arg) {
        code_index = 0;
        fixed_arg_bit = 0;
        changing_arg_bit = 0;
        for (int arg_bit = length_param_ - 1; arg_bit >= 0; --arg_bit) {
          if (monom_degree[decoded_signal_bit][arg_bit] == 1) {
            code_index += static_cast<bool>(changing_arg & (1 << changing_arg_bit)) * pow(2, length_param_ - 1 - arg_bit);
            changing_arg_bit++;
          } else {
            code_index += static_cast<bool>(fixed_arg & (1 << fixed_arg_bit)) * pow(2, length_param_ - 1 - arg_bit);
            fixed_arg_bit++;
          }
        }
        vote ^= new_code[code_index];
      }
      vote == 1 ? ++ones : ++zeros;
    }
    decoded_signal[decoded_signal_bit] = (ones > zeros);
  }
  return decoded_signal;
}

int ReedMullerCoder::distance() {
  return pow(2, length_param_ - order_);
}

int ReedMullerCoder::Binomial(int n, int k) {
  int result = 0;
  result = std::tgamma(n + 1) / (std::tgamma(k + 1) * std::tgamma(n - k + 1));
  return result;
}

bool ReedMullerCoder::MonomDegreeOrder(const std::valarray<bool>& lhs, const std::valarray<bool>& rhs) {
  if (lhs.size() != rhs.size()) {
    return 0;
  }

  if (Weight(lhs) < Weight(rhs)) {
    return 1;
  }

  if (Weight(lhs) > Weight(rhs)) {
    return 0;
  }

  for (int i = rhs.size() - 1; i >= 0; --i) {
    if (lhs[i] > rhs[i]) {
      return 1;
    }

    if (lhs[i] < rhs[i]) {
      return 0;
    }
  }

  return false;
}

ReedMullerSystematicCoder::ReedMullerSystematicCoder(int order, int length_power) :
  ReedMullerCoder(order, length_power),
  signal_permutation_(std::valarray<bool>(signal_length_ * signal_length_)),
  signal_inverse_permutation_(std::valarray<bool>(signal_length_ * signal_length_)),
  code_permutation_(std::valarray<bool>(code_length_ * code_length_)),
  code_inverse_permutation_(std::valarray<bool>(code_length_ * code_length_))
{
  // Finding the new order of columns from generator matrix making it triangular.
  // The value -1 means this element wasn't processed.
  std::vector<int> new_index(code_length_, -1);
  std::vector<int> old_index(code_length_, -1);
  for (int new_column = 0; new_column < signal_length_; ++new_column) {
    for (int old_column = 0; old_column < code_length_; ++old_column) {
      if (Weight(generator_[std::slice(new_column * code_length_ + old_column,
            signal_length_ - new_column, code_length_)]) == 1 &&
          Weight(generator_[std::slice((new_column + 1) * code_length_ + old_column,
            signal_length_ - new_column - 1, code_length_)]) == 0) {
        new_index[new_column] = old_column;
        old_index[old_column] = new_column;
        break;
      }
    }
  }

  // Permutation of columns from generator matrix
  for (int new_column = signal_length_; new_column < code_length_; ++new_column) {
    for (int old_column = 0; old_column < code_length_; ++old_column) {
      if (old_index[old_column] == -1) {
        new_index[new_column] = old_column;
        old_index[old_column] = new_column;
        break;
      }
    }
  }

  for (int row = 0; row < code_length_; ++row) {
    code_permutation_[row * code_length_ + old_index[row]] = 1;
    code_inverse_permutation_[row * code_length_ + new_index[row]] = 1;
  }

  for (int column = 0; column < signal_length_; ++column) {
    signal_inverse_permutation_[std::slice(column, signal_length_, signal_length_)] = generator_[std::slice(new_index[column], signal_length_, code_length_)];
  }

  // Filling signal_permutation_ by rows since source matrix have triangular form.
  // Product of signal_permutation_ and square submatrix of generator matrix with 
  // new order of columns must be the identity matrix.
  std::valarray<bool> permutation_row(signal_length_);
  std::valarray<bool> generator_column(signal_length_);
  int element_value = 0;
  for (int row = 0; row < signal_length_; ++row) {
    for (int column = 0; column < signal_length_; ++column) {
      permutation_row = signal_permutation_[std::slice(row * signal_length_, signal_length_, 1)];
      generator_column = generator_[std::slice(new_index[column], signal_length_, code_length_)];

      // Product of permutation row and generator column must be 1 on diagonal and 0 else.
      element_value = (Weight(permutation_row * generator_column) % 2) != (row == column);
      signal_permutation_[row * signal_length_ + column] = element_value;
    }
  }
}

std::valarray<bool> ReedMullerSystematicCoder::Code(const std::valarray<bool>& signal) {
  std::valarray<bool> permutated_signal;
  std::valarray<bool> code;

  permutated_signal = MultiplyVectorToMatrix(signal, signal_permutation_);
  code = ReedMullerCoder::Code(permutated_signal);
  code = MultiplyVectorToMatrix(code, code_permutation_);

  return code;
}

std::valarray<bool> ReedMullerSystematicCoder::Decode(const std::valarray<bool>& code) {
  std::valarray<bool> permutated_code;
  std::valarray<bool> decoded_signal;

  permutated_code = MultiplyVectorToMatrix(code, code_inverse_permutation_);
  decoded_signal = ReedMullerCoder::Decode(permutated_code);
  decoded_signal = MultiplyVectorToMatrix(decoded_signal, signal_inverse_permutation_);

  return decoded_signal;
}

