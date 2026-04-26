#pragma once

#include <type_traits>
#include <string>

template<std::size_t I>
using size_t_i_c = std::integral_constant<std::size_t, I>;

namespace meta_prog
{

    using mp_true = std::integral_constant<bool, true>;
    using mp_false = std::integral_constant<bool, false>;
	template<class P, class L> struct mp_find_impl;

}