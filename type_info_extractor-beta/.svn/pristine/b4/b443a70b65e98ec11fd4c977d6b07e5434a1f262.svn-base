#pragma once

#include "pod_inspector.h"

using START_TAG = meta_prog::mp_type_open_brace;
using END_TAG = meta_prog::mp_type_close_brace;
using NOT_FOUND = meta_prog::mp_type_not_found;

template <typename T, typename... Tail>
struct pop_first
{
    using first = T;
    using rest = tuple_type<Tail...>;
};
template <template<class...> class TL, typename T, typename... Tail>
struct pop_first<TL<T,Tail...>>
{
    using first = T;
    using rest = tuple_type<Tail...>;
};
template <typename Head, typename... OHs>
struct build_type;
template <template<class...>
          class OuterH, typename... OHs,
            template<class...> class Tail, class Head, class... Ts >
struct build_type<OuterH<OHs...>, Tail<Head, Ts...>>
{
    using first = Head;
    using step = build_type<tuple_type<OHs...,Head>, tuple_type<Ts...> >;
    using result = typename step::result;
    using tail = typename step::tail;
};
template < typename... OHs,
             class... Ts >
struct build_type<tuple_type<OHs...>, tuple_type<START_TAG, Ts...>>
{
    using first = START_TAG;
    using fold = build_type<Ts...>;
    using step = build_type<tuple_type<OHs...,
            typename fold::result>, typename fold::tail >;
    using result = typename step::result;
    using tail = typename step::tail;
};
template <template<class...>
          class OuterH, typename... OHs,
            template<class...> class Tail, class... Ts >
struct build_type<OuterH<OHs...>, Tail<END_TAG, Ts...>>
{
    using first = END_TAG;
    using result = OuterH<OHs...>;
    using tail = tuple_type<Ts...>;
};

template <template<class...> class OuterH, typename... OHs>
struct build_type<OuterH<OHs...>, tuple_type<>>
{
    using first = typename pop_first<OuterH<OHs...>>::first;
    using result = OuterH< OHs...>;
    using tail = tuple_type<>;
};
template <typename Head, typename... Tail>
struct build_type
{
    using first = Head;
    using step = build_type<tuple_type<>, tuple_type<Head, Tail...> >;
    using result = typename step::result;
    using tail = typename step::tail;
};
