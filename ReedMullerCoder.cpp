#include "ReedMullerCoder.h"

ReedMullerCoder::ReedMullerCoder(int order, int length_power) :
  LinearCoder()
{
  signal_length_ = 0;
  for (int i = 0; i <= order; ++i) {
    signal_length_ += Binomial(length_power, i);
  }

  code_length_ = pow(2, length_power);
  generator_ = std::valarray<bool>(signal_length_ * code_length_);

  std::vector<std::valarray<bool>> monom_degree(code_length_);
  for (std::size_t i = 0; i < code_length_; ++i) {
    monom_degree[i] = std::valarray<bool>(length_power);
    for (std::size_t j = 0; j < length_power; ++j) {
      monom_degree[i][j] = i & (1 << (length_power - 1 - j));
    }
  }

  std::sort(monom_degree.begin(), monom_degree.end(), MonomDegreeOrder);

  // TODO: rename i, generator_column
  int i = 0;
  std::valarray<bool> generator_column(length_power);
  for (int row = 0; row < signal_length_; ++row) {
    for (int column = code_length_ - 1; column >= 0; --column) {
      for (int bit = 0; bit < length_power; ++bit) {
        generator_column[bit] = static_cast<bool>(column & (1 << bit));
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
  int check_length = code_length_ - signal_length_;
  int broken_bit = 0;
  std::valarray<bool> decoded(signal_length_);
  std::valarray<bool> decode_matrix = std::valarray<bool>(check_length * code_length_);
  std::valarray<bool> decode_matrix_column = std::valarray<bool>(code_length_);
  std::valarray<bool> syndrome = std::valarray<bool>(check_length);

  int i = 0;
  for (int row = 1; row <= code_length_; ++row) {
    for (int column = 1; column <= check_length; ++column) {
      decode_matrix[i] = static_cast<bool>(row & (1 << (check_length - column)));
      i++;
    }
  }

  for (int bit_number = 0; bit_number < check_length; ++bit_number) {
    decode_matrix_column = decode_matrix[std::slice(bit_number, code_length_, check_length)];
    decode_matrix_column *= code;
    syndrome[bit_number] = 0;
    for (int j = 0; j < code_length_; ++j) {
      syndrome[bit_number] ^= decode_matrix_column[j];
    }
    broken_bit += syndrome[bit_number] * pow(2, check_length - 1 - bit_number);
  }

  i = 0;
  for (int bit_number = 1; bit_number <= code_length_; ++bit_number) {
    if (!(bit_number & (bit_number - 1))) continue;
    decoded[i] = code[bit_number - 1];
    if (bit_number == broken_bit) {
      decoded[i] ^= 1;
    }
    i++;
  }

  return decoded;
}

int ReedMullerCoder::getDistance() {
  return 3;
}

long long ReedMullerCoder::Binomial(int n, int k) {
  long long result = 0;
  result = std::tgamma(n + 1) / (std::tgamma(k + 1) * std::tgamma(n - k + 1));
  return result;
}

bool ReedMullerCoder::MonomDegreeOrder(const std::valarray<bool>& lhs, const std::valarray<bool>& rhs) {
  if (lhs.size() != rhs.size()) return 0;

  int lhs_sum = 0;
  int rhs_sum = 0;
  for (int i = 0; i < lhs.size(); ++i) {
    lhs_sum += static_cast<int>(lhs[i]);
    rhs_sum += static_cast<int>(rhs[i]);
  }

  if (lhs_sum < rhs_sum) return 1;
  if (lhs_sum > rhs_sum) return 0;

  for (int i = rhs.size() - 1; i >= 0; --i) {
    if (lhs[i] > rhs[i]) return 1;
    if (lhs[i] < rhs[i]) return 0;
  }

  return false;
}

