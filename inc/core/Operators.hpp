#pragma once

namespace Logic {
template <typename T> class UnaryOperator {
public:
  virtual T operator()(const T in) const = 0;
};

template <typename T> class BinaryOperator {
public:
  virtual T operator()(const T first, const T second) const = 0;
};
}
