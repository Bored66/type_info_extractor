#pragma once

#include "mp_utils.h"

template <std::size_t N, class T>
struct holder
{
    T value;
};
//#define nstd std
namespace nstd {
using size_t = long unsigned int;
template< class T, T... Ints >
struct integer_sequence
{
//public:
    static constexpr nstd::size_t size() noexcept
    { return sizeof...(Ints); }
    typedef T value_type;
    using type = integer_sequence;
    static_assert(
      std::is_integral<T>::value,
      "std::integer_sequence can only be instantiated with an integral type" );
};
template<nstd::size_t... Ints>
using index_sequence = nstd::integer_sequence<nstd::size_t, Ints...>;
// --------------------------------------------------------------

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <nstd::size_t... I1, nstd::size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
  : index_sequence<I1..., (sizeof...(I1)+I2)...>
{ };

// --------------------------------------------------------------

template <nstd::size_t N>
struct make_index_sequence
  : _merge_and_renumber<typename make_index_sequence<N/2>::type,
                        typename make_index_sequence<N - N/2>::type>
{
    using base = _merge_and_renumber<typename make_index_sequence<N/2>::type,
    typename make_index_sequence<N - N/2>::type>;
};

template<> struct make_index_sequence<0> : index_sequence<> {};// { using base = index_sequence<>; };
template<> struct make_index_sequence<1> : index_sequence<0>{};// { using base = index_sequence<0>; };
}
#ifndef USE_UTILITY
template <class I, class ...Tail>
struct tuple_holder_base;

template <nstd::size_t... I, class ...Tail>
struct tuple_holder_base< nstd::index_sequence<I...>, Tail... >
    : public holder<I , Tail>...
{
    static constexpr nstd::size_t size_v = sizeof...(I);

    constexpr tuple_holder_base() noexcept = default;
    //constexpr tuple_holder_base(tuple_holder_base&&) noexcept = default;
    //constexpr tuple_holder_base(const tuple_holder_base&) noexcept = default;

    constexpr tuple_holder_base(Tail... v) noexcept
        : holder<I, Tail>{ v }...
    {}
};


template <class ...Values>
using tuple_holder_seq =
    tuple_holder_base<nstd::make_index_sequence<sizeof...(Values)>, Values...>;

template <class ...Values >
struct seq_tuple: tuple_holder_seq<Values...>
//tuple_holder_base<std::make_index_sequence<sizeof...(Values)>, Values...>
{
    using tuple_holder_base<
    nstd::make_index_sequence<sizeof...(Values)>,
    Values...
    >::tuple_holder_base;
};


#else
template <class I, class ...Tail>
struct tuple_holder_base;

template <std::size_t... I, class ...Tail>
struct tuple_holder_base< std::index_sequence<I...>, Tail... >
    : public holder<I , Tail>...
{
    static constexpr std::size_t size_v = sizeof...(I);

    constexpr tuple_holder_base() noexcept = default;
    //constexpr tuple_holder_base(tuple_holder_base&&) noexcept = default;
    //constexpr tuple_holder_base(const tuple_holder_base&) noexcept = default;

    constexpr tuple_holder_base(Tail... v) noexcept
        : holder<I, Tail>{ v }...
    {}
    static const char* base_name() noexcept { return __PRETTY_FUNCTION__; }
};

template <>
struct tuple_holder_base<std::index_sequence<> > {
    static constexpr std::size_t size_v = 0;
};

template <class ...Values>
struct seq_tuple:
        tuple_holder_base<std::make_index_sequence<sizeof...(Values)>, Values...>
{
    using tuple_holder_base<
        std::make_index_sequence<sizeof...(Values)>,
        Values...
    >::tuple_holder_base;
    static const char* name() noexcept { return __PRETTY_FUNCTION__; }

};
#endif

template <std::size_t N, class T>
constexpr T& get_impl(holder<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr const T& get_impl(const holder<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr volatile T& get_impl(volatile holder<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr const volatile T& get_impl(const volatile holder<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr T&& get_impl(holder<N, T>&& t) noexcept {
    return std::forward<T>(t.value);
}

#ifdef CPP_14
template <std::size_t N, class ...T>
constexpr decltype(auto) getNth(seq_tuple<T...>& t) noexcept
{
    static_assert(N < seq_tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) getNth(const seq_tuple<T...>& t) noexcept {
    static_assert(N < seq_tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) getNth(const volatile seq_tuple<T...>& t) noexcept {
    static_assert(N < seq_tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) getNth(volatile seq_tuple<T...>& t) noexcept {
    static_assert(N < seq_tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) getNth(seq_tuple<T...>&& t) noexcept {
    static_assert(N < seq_tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(std::move(t));
}

template <std::size_t I, class T>
using tuple_element = std::remove_reference< decltype(
        getNth<I>( std::declval<T>() )
    ) >;

#endif
