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
    std::size_t type_aligns[N]{}; //TODO: why USE_CPP_11_WAY
    std::size_t type_sizes[N]{}; //TODO: why USE_CPP_11_WAY
    meta_info type_meta_info[N]{};

    std::size_t index = 0;
    std::size_t field_count = 0;
    bool insert_braces = InsertBraces;
	static constexpr std::size_t size() noexcept { return N; }
    constexpr std::size_t addInfo(const meta_info & info) noexcept
	{
            type_meta_info[index] = info;
            type_ids[index] = info.type_index;
        return
            index = index + 1;
	}
    constexpr std::size_t addInfo(const std::size_t type_id) const
        noexcept
    {
        return type_ids[index] = type_id;
    }
};
