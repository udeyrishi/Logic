#pragma once

namespace Logic {
    template <typename T>
    class BinaryOperator
    {
    public:
        virtual T operator()(const T first, const T second) const = 0;
    };
}