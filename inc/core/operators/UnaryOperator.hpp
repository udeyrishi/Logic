#pragma once

namespace Logic {
    template <typename T>
    class UnaryOperator
    {
    public:
        virtual T operator()(const T in) const = 0;
    };
}