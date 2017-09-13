// gcc7, -std=c++17

#include <type_traits>

template <typename ...>
struct type_list {};

template <template <auto...> class T, typename L, typename... I>
struct lookup_adaptor_ {};

template <template <auto...> class T, typename U, U F, typename... LR, typename... IR>
struct lookup_adaptor_<T, type_list<LR...>, std::integral_constant<U, F>, IR...> {
    inline decltype(T<LR::value..., F, IR::value...>::value) operator()(const U &f, const typename IR::value_type&... i) const {
        return (F == f) ? lookup_adaptor_<T, type_list<LR..., std::integral_constant<U, F>>, IR...>()(i...) :
                          lookup_adaptor_<T, type_list<LR...>, std::integral_constant<U, F-1>, IR...>()(f, i...);
    }
};

template <template <auto...> class T, typename U, typename... LR, typename... IR>
struct lookup_adaptor_<T, type_list<LR...>, std::integral_constant<U, U(0)>, IR...> {
    inline decltype(T<LR::value..., U(0), IR::value...>::value) operator()(const U &f, const typename IR::value_type&... i) const {
        return lookup_adaptor_<T, type_list<LR..., std::integral_constant<U, U(0)>>, IR...>()(i...);
    }
};

template <template <auto...> class T, typename... LR>
struct lookup_adaptor_<T, type_list<LR...>> {
    inline decltype(T<LR::value...>::value) operator()() const {
        return T<LR::value...>::value;
    }
};

template <template <auto...> class T, typename... I>
using lookup_adaptor = lookup_adaptor_<T, type_list<>, I...>;

template <unsigned N>
struct fib {
    static const unsigned value = fib<N-1>::value + fib<N-2>::value;
};

template <>
struct fib<1> {
    static const unsigned value = 1;
};

template <>
struct fib<0> {
    static const unsigned value = 1;
};

template <int... N>
struct add {
    static const int value = 0;
};

template <int F, int... R>
struct add<F, R...> {
    static const int value = F + add<R...>::value;
};

template <int I, unsigned U, long L>
struct add_3 {
    static const long value = I + U + L;
};


int main(int argc, char** argv) {
    lookup_adaptor<fib, std::integral_constant<int, 20>> fib_table;
    lookup_adaptor<add, std::integral_constant<int, 5>, std::integral_constant<int, 5>, std::integral_constant<int, 5>> add_table;
    lookup_adaptor<add_3, std::integral_constant<int, 5>, std::integral_constant<unsigned, 5>, std::integral_constant<long, 5>> add_3_table;
   
    return fib_table(argc) + add_table(argc, argc + 1, argc + 2) + add_3_table(argc, argc + 1, argc + 2);
}