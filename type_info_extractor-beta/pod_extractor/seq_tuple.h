#pragma once

#include <utility>
#include <type_traits>

template <std::size_t N, class T>
struct holder
{
    T value;
};
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
