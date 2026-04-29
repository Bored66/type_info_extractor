#pragma once

#include <cstdint>
#include <array>
#include <tuple>
#include <type_traits>

template <std::size_t I>
using size_t_i_c = std::integral_constant<std::size_t, I>;

namespace meta_prog
{

    //TODO inline ?
    constexpr std::size_t mp_type_not_found_value = 9999;
    using mp_type_not_found = size_t_i_c<mp_type_not_found_value>;

    constexpr std::size_t mp_type_open_brace_value = 10999;
    using mp_type_open_brace = size_t_i_c<mp_type_open_brace_value>;

    constexpr std::size_t mp_type_close_brace_value = 11999;
    using mp_type_close_brace = size_t_i_c<mp_type_close_brace_value>;

    using list_of_fundamental_types = std::tuple<
        int, long, long long, bool,
        signed char, short,
        unsigned char, unsigned short, unsigned int, unsigned long,
        unsigned long long,
        char,
        wchar_t, char16_t, char32_t,
        float, double, long double,
        void>;

    template <typename T, typename Tuple>
    struct index_in_tuple;

    template <typename T, typename... Us>
    struct index_in_tuple<T, std::tuple<Us...>>
    {
    private:
        static constexpr std::array<bool, sizeof...(Us)> matches = { std::is_same_v<T, Us>... };
        
        static constexpr std::size_t find() {
            for (std::size_t i = 0; i < matches.size(); ++i) {
                if (matches[i]) return i;
            }
            return mp_type_not_found_value;
        }
        
    public:
        static constexpr std::size_t value = find();
    };
    template <std::size_t idx>
    using type_by_index = std::tuple_element_t<idx, list_of_fundamental_types>;

    template <typename T>
    using index_of_type = size_t_i_c<index_in_tuple<T, list_of_fundamental_types>::value>;

}