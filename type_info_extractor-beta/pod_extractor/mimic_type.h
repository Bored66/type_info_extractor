#pragma once

#if defined(CPP_14) || defined(TRY_CPP_14_TESTS)
#include "pod_inspector.h"
#include "build_type.h"

template<typename...Ti>
constexpr auto form_type()
{
    using RetT = typename build_type<Ti...>::result;
    return RetT{};
}
template<typename Type, size_t Count, size_t...I>
constexpr auto mimic_type(std::index_sequence<I...>)
{
    constexpr auto info = get_pod_meta_infos<Type, Count,true>();
    return form_type< typename get_type<info.type_ids[I]>::type... >();
}
template<typename Type, size_t Count = fields_count<Type>()>
constexpr auto mimic_type()
{
    constexpr auto info = get_pod_meta_infos<Type, Count, true>();
    return mimic_type<Type,Count>(std::make_index_sequence<info.index>{});
}
#else
#warning mimic_type works strictly only using c++14
#endif // CPP_14

