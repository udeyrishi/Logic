#pragma once

namespace Logic {

template <typename T>
class UnaryOperator {
public:
  virtual T operator()(const T in) const = 0;
};

template <typename T>
class BinaryOperator {
public:
  virtual T operator()(const T first, const T second) const = 0;
};

template <typename T>
class Not : public UnaryOperator<T> {
public:
    virtual T operator()(const T in) const {
        return !in;
    }
};

template <typename T>
class Or : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
        return first || second;
    }
};

template <typename T>
class And : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
        return first && second;
    }
};

template <typename T>
class Xor : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
        return first != second;
    }
};
}
