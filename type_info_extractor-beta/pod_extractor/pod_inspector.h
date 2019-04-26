#pragma once
#if (_MSC_VER > 1912)
#define CPP_14
#endif

#ifdef CPP_14
#define CPP_11_NO_CONST_WARN_OFF
#else
#define CPP_11_NO_CONST_WARN_OFF const
#endif
#include <type_traits>

#include "mp_utils.h"
#define USE_SEQ_TUPLE
#ifdef USE_SEQ_TUPLE
#include "seq_tuple.h"
#else
#include <tuple>
#endif
template<typename... Ts>
#ifdef USE_SEQ_TUPLE
    using type_list = seq_tuple<Ts...>;
#ifndef USE_UTILITY
template<typename... Ts>
    struct tuple_type : //type_list<Ts...>;//
                    tuple_holder_seq<Ts...>{};
#else
template<typename... Ts>
    using tuple_type = type_list<Ts...>;
#endif
#else
    using type_list = std::tuple<Ts...>;
#endif
#include "mp.h"
#include "typeid_array.h"

#ifdef IGNORE_DIAG
#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wundefined-inline"
#   pragma clang diagnostic ignored "-Wundefined-internal"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#endif

#include "field_count.h"
#include "extract_type_info.h"

#ifdef IGNORE_DIAG
#ifdef __clang__
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif
#endif

