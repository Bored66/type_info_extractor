#pragma once

#include "pod_inspector.h"
#include "build_type.h"

#ifdef CPP_14
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
template<typename...Ti, typename RetT = typename build_type<Ti...>::result>
constexpr auto form_type() -> decltype (RetT{})
{
    return {};
}

template<typename Type, size_t Count = fields_count<Type>(), size_t... I>
constexpr auto mimic_type(std::index_sequence<I...>)
    -> decltype (form_type<
                     typename get_type<
                           get_pod_meta_infos<Type, Count,true>().type_ids[I]
                        >::type...
                >())
{
    return form_type<
                typename get_type<
                    get_pod_meta_infos<Type, Count,true>().type_ids[I]
                >::type...
            >();
}
template<typename Type, size_t Count = fields_count<Type>()>
constexpr auto mimic_type()
    -> decltype (mimic_type<Type,Count>(std::make_index_sequence<
                                            get_pod_meta_infos<Type, Count, true>().index
                                        >{}))
{
    return mimic_type<Type,Count>(std::make_index_sequence<
                                    get_pod_meta_infos<Type, Count, true>().index
                                  >{});
}
#endif // CPP_14

