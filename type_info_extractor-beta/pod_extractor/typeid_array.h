#pragma once

#include <type_traits>
#include "mp.h"
#include "build_type.h"
struct meta_info
{
    std::size_t nth{};
    std::size_t type_index{};
    std::size_t type_size{};
    std::size_t internal_count{};
    std::size_t aux_data{};
    constexpr meta_info(std::size_t nth, std::size_t type_index,
                        std::size_t type_size,std::size_t internal_count,
                        std::size_t aux_data)
        : nth(nth),type_index(type_index),type_size(type_size),
    internal_count(internal_count),
    aux_data (aux_data)
    { }
    constexpr meta_info(){}
};

template<std::size_t N, bool InsertBraces = false>
struct typeid_array
{
    std::size_t type_ids[N]{};
    std::size_t type_aligns[N]{};
    std::size_t type_sizes[N]{};
    meta_info type_meta_info[N]{};

    std::size_t index = 0;
    std::size_t field_count = 0;
    bool insert_braces = InsertBraces;
	static constexpr std::size_t size() noexcept { return N; }
    constexpr std::size_t addInfo(const meta_info & info)
    CPP_11_NO_CONST_WARN_OFF noexcept
	{
        return
#if defined(__clang__) && !defined(CPP_11)
            type_meta_info[index] = info,
#else
#endif
#if defined(CPP_11) || defined(TRY_CPP_14_TESTS)
    #warning You are in defined(CPP_11) or defined(TRY_CPP_14_TESTS) MODE
#else
#endif
#if defined(CPP_11) || defined(TRY_CPP_14_TESTS)            
            const_cast<typeid_array*>(this)->
#endif
            type_ids[index] = info.type_index,
#if defined(CPP_11) || defined(TRY_CPP_14_TESTS)
            const_cast<typeid_array*>(this)->
#endif
            index = index + 1;
	}
    constexpr std::size_t addInfo(const std::size_t type_id) const
        noexcept
    {
        return type_ids[index] = type_id;
    }
};
