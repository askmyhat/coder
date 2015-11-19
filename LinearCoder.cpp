#include "LinearCoder.h"

LinearCoder::LinearCoder() : Coder() {}
LinearCoder::LinearCoder(int signal_length) : Coder(signal_length) {}

std::valarray<bool> LinearCoder::MultiplyVectorToMatrix(const std::valarray<bool>& string, const std::valarray<bool>& matrix) {
  int rows = string.size();
  int columns = matrix.size() / rows;

  std::valarray<bool> product(columns);
  std::valarray<bool> column(rows);

  for (int column_index = 0; column_index < columns; ++column_index) {
    column = matrix[std::slice(column_index, rows, columns)];
    column *= string;
    product[column_index] = 0;
    for (int j = 0; j < rows; ++j) {
      product[column_index] ^= column[j];
    }
  }

  return product;
}

std::valarray<bool> LinearCoder::Code(const std::valarray<bool>& signal) {
  return MultiplyVectorToMatrix(signal, generator_);
}

void LinearCoder::PrintGenerator() {
  PrintMatrix(generator_, code_length_);
}

void LinearCoder::PrintMatrix(const std::valarray<bool>& matrix, int columns) {
  for (size_t i = 0; i < matrix.size(); ++i) {
    std::cout << matrix[i] << " ";
    if ((i + 1) % columns == 0) {
      std::cout << std::endl;
    }
  }
}

