#pragma once

#include <type_traits>
#include <string>

#include "pod_inspector.h"

template<std::size_t I>
using size_t_i_c = std::integral_constant<std::size_t, I>;

namespace meta_prog
{

    using mp_true = std::integral_constant<bool, true>;
    using mp_false = std::integral_constant<bool, false>;
	template<class P, class L> struct mp_find_impl;
	template<template<class...> class L, class P, class...C>
	struct mp_find_impl<P, L<P, C...>>
	{
		using type = mp_true;
	};
	template<template<class...> class L, class P>
	struct mp_find_impl<P, L<>>
	{
		using type = mp_false;
	};

	/// Access an element by an index

    template <std::size_t idx, class List>
	struct mp_list_type_at_impl;

    template <typename T, typename... Ts> // End of search, type is found
	struct mp_list_type_at_impl < 0, type_list<T, Ts...> >
	{
		using type = T;
	};
	/// Recursion
    template <std::size_t idx, typename T, typename...Ts>
	struct mp_list_type_at_impl <idx, type_list<T, Ts...>>
	{
        using type =
            typename mp_list_type_at_impl<idx - 1, type_list<Ts...>>::type;
	};
	/// Wrapper
    template <std::size_t idx, class List>
	struct mp_list_type_at;

    template <std::size_t idx, typename... Ts>
	struct mp_list_type_at<idx, type_list<Ts...>>
	{
	private:
		static_assert(sizeof...(Ts) > idx,
			"mp_list_type_at: Index out of bounds or called on empty list");
	public:
        using type =
            typename mp_list_type_at_impl<idx, type_list<Ts...>>::type;
	};

    constexpr std::size_t mp_type_not_found_value = 9999;
    using mp_type_not_found = size_t_i_c<mp_type_not_found_value>;
    constexpr std::size_t mp_type_open_brace_value = 10999;
    using mp_type_open_brace = size_t_i_c<mp_type_open_brace_value>;
    constexpr std::size_t mp_type_close_brace_value = 11999;
    using mp_type_close_brace = size_t_i_c<mp_type_close_brace_value>;

    using list_of_fundamental_types = type_list
    <
    int, long, long long, bool,
    signed char, short,
    unsigned char, unsigned short, unsigned int, unsigned long,
    unsigned long long,
    char,
    wchar_t, char16_t, char32_t,
    float, double, long double,
    //std::string,
    void
    >;
    template <std::size_t idx>
    using type_by_index =
        typename mp_list_type_at<idx, list_of_fundamental_types>::type;

    template <std::size_t idx, class T, class L>
	struct mp_list_index_of_impl;

    // The type T is not in the list
    template <std::size_t idx, typename T>
	struct mp_list_index_of_impl <idx, T, type_list<> >
	{
		using type = mp_type_not_found;
	};
    // End of search, type is found
    template <std::size_t idx, typename T, typename... Ts>
	struct mp_list_index_of_impl <idx, T, type_list<T, Ts...> >
	{
        using type = std::integral_constant<std::size_t, idx>;
	};
    //Recursion
    template <std::size_t idx, typename T, typename H, typename... Ts>
	struct mp_list_index_of_impl <idx, T, type_list<H, Ts...> >
	{
        using type =
            typename mp_list_index_of_impl<idx + 1, T,
                                    type_list<Ts...>>::type;
	};
	template <class T, class L>
	struct mp_list_index_of;
	template <class T, class...Ts>
	struct mp_list_index_of<T, type_list<Ts...>>
	{
        using type =
            typename mp_list_index_of_impl<0, T, type_list<Ts...>>::type;
	};
	template <typename T>
	using index_of_type =
        typename meta_prog::mp_list_index_of<T,
            list_of_fundamental_types>::type;
}
