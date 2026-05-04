#pragma once

#include <type_traits>
#include <utility>

template<std::size_t N>
struct accept_any_type
{
    std::size_t ignore;
    template <class Type> constexpr operator Type&() const noexcept;
};

template<typename T, std::size_t... I>
constexpr auto check_impl(std::index_sequence<I...>) 
    -> decltype(T{ accept_any_type<I>{I}... } );

template<typename T, std::size_t N, typename = void>
struct can_init_with : std::false_type {};

template<typename T, std::size_t N>
struct can_init_with<T, N, std::void_t<decltype(check_impl<T>(std::make_index_sequence<N>()))>>
    : std::true_type {};
template <std::size_t I>
using size_t_i_c = std::integral_constant<std::size_t, I>;

template<class T>
struct count_type_fields
{
private:
    template <std::size_t N>
    static constexpr
    std::size_t count(size_t_i_c<N>, size_t_i_c<N>) noexcept
    { return N; }
    template <std::size_t lowerBound, std::size_t upperBound>
    static constexpr std::size_t
    count(size_t_i_c<lowerBound>, size_t_i_c<upperBound>) noexcept
    {
        if constexpr(can_init_with<T, upperBound>::value)
        // if constexpr(sizeof(decltype_t<T, upperBound>) != 0)
        {
            return count(size_t_i_c<upperBound>{}, size_t_i_c<upperBound +
                        (upperBound - lowerBound + 1) / 2>{});
        }
        else
        {
                return count(size_t_i_c<lowerBound>{},
                     size_t_i_c<(lowerBound + upperBound) / 2>{});
        }
    }
    static constexpr std::size_t EST_MAX_COUNT()
    { return (sizeof(T) / 16) + 1; } //works faster
public:
    static constexpr std::size_t count_fields() noexcept
    {
        return count(size_t_i_c<0>{}, size_t_i_c<EST_MAX_COUNT()>{});
    }
    enum { result = count_fields() };
};

template<class T>
constexpr std::size_t fields_count()
{
    using flds = count_type_fields<T>;
    return flds::result;
}
