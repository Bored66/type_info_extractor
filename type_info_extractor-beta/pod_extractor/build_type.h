#pragma once

#include "pod_inspector.h"

using START_TAG = meta_prog::mp_type_open_brace;
using END_TAG = meta_prog::mp_type_close_brace;
using NOT_FOUND = meta_prog::mp_type_not_found;

template <typename>
struct pop_first;

template <typename T, typename... Tail>
struct pop_first<tuple_type<T,Tail...>>
{
    using first = T;
    using rest = tuple_type<Tail...>;
};

template <typename List>
using pop_first_t = typename pop_first<List>::first;

template <typename Head, typename... OHs>
struct build_type_impl;

// Обычный тип: добавляем в аккумулятор
template <typename... OHs, class Head, class... Ts >
struct build_type_impl<tuple_type<OHs...>, tuple_type<Head, Ts...>>
{
    using first = Head;
    using step = build_type_impl<tuple_type<OHs...,Head>, tuple_type<Ts...> >;
    using result = typename step::result;
    using tail = typename step::tail;
};

// Открывающая скобка: парсим вложенный тип
template < typename... OHs,
             class... Ts >
struct build_type_impl<tuple_type<OHs...>, tuple_type<START_TAG, Ts...>>
{
    using first = START_TAG;
    using fold = build_type_impl<Ts...>;
    using step = build_type_impl<tuple_type<OHs...,
            typename fold::result>, typename fold::tail >;
    using result = typename step::result;
    using tail = typename step::tail;
};

// Закрывающая скобка: возвращаем накопленное
template <typename... OHs, class... Ts >
struct build_type_impl<tuple_type<OHs...>, tuple_type<END_TAG, Ts...>>
{
    using first = END_TAG;
    using result = tuple_type<OHs...>;
    using tail = tuple_type<Ts...>;
};

// Базовый случай: всё разобрано
template <typename... OHs>
struct build_type_impl<tuple_type<OHs...>, tuple_type<>>
{
    using first = pop_first_t<tuple_type<OHs...>>;
    using result = tuple_type< OHs...>;
    using tail = tuple_type<>;
};

// ========== Пользовательский интерфейс ==========
template <typename Head, typename... Tail>
struct build_type
{
    using first = Head;
    using step = build_type_impl<tuple_type<>, tuple_type<Head, Tail...> >;
    using result = typename step::result;
    using tail = typename step::tail;
};

// Вспомогательный тип для типа
template<typename... Tokens>
using build_type_t = typename build_type<Tokens...>::result;