#pragma once
#if 1 //|| __has_feature(cxx_access_control_sfinae)
#define FIELD_COUNT_WORKS
#elif 0
#define TEST_HOW
#define PESSIMISTIC
#endif
template<std::size_t N>
struct accept_any_type
{
    std::size_t ignore;
    template <class Type> constexpr operator Type&() const noexcept;
};
template <class T, std::size_t... I>
using type_declared_t = typename std::add_pointer<
    decltype(T{ accept_any_type<I>{ I }... })>::type;
template <class T,std::size_t... I>
constexpr auto fld_count_within_range(std::index_sequence<I...>) noexcept
->  type_declared_t< T, I... >;//we don't need any more: { return nullptr;}
/* We used to be using that monster before
 *         sizeof(decltype(fld_count_within_range<T>
                        (std::make_index_sequence<upperBound>())))!=0,
                            std::size_t
                            >::type//enable_if<
                            */
template <class T,std::size_t UpperBound>
    using decltype_t =
        decltype(fld_count_within_range<T>
            (std::make_index_sequence<UpperBound>()));
//NOTE: !=0 is very important for clang and lesser versions of g++!
//typename std::enable_if<sizeof(decltype_t<T, upperBound>)!=0,std::size_t>::type
template<class T,std::size_t UpperBound>
    using enable_if_upper_bound_is_ok_t =
        typename std::enable_if<sizeof(decltype_t<T, UpperBound>)!=0,
            std::size_t>::type;

template<class T>
struct count_type_fields
{
private:
    template <std::size_t N>
    static constexpr
    std::size_t count(size_t_i_c<N>, size_t_i_c<N>, long) noexcept
    { return N; }
    template <std::size_t lowerBound, std::size_t upperBound>
    static constexpr enable_if_upper_bound_is_ok_t<T, upperBound>
    count(size_t_i_c<lowerBound>, size_t_i_c<upperBound>, long) noexcept
    {
        return count(size_t_i_c<upperBound>{}, size_t_i_c<upperBound +
                     (upperBound - lowerBound + 1) / 2>{}, 1L);
    }
    template <std::size_t lowerBound, std::size_t upperBound>
    static constexpr std::size_t
    count(size_t_i_c<lowerBound>, size_t_i_c<upperBound>, int) noexcept
    {
        return count(size_t_i_c<lowerBound>{},
                     size_t_i_c<(lowerBound + upperBound) / 2>{}, 1L);
    }
    static constexpr
    std::size_t EST_MAX_COUNT()
#ifndef PESSIMISTIC
    { return (sizeof(T) / 16) + 1; } //works faster
#else    //FIXME: Original REMOVE IT! We multiply by 8 because we may have bitfields in T
    { return (sizeof(T) * 8) / 2 + 1; }
#endif
public:
    static constexpr
    std::size_t count_fields() noexcept
    {
        return count(size_t_i_c<0>{}, size_t_i_c<EST_MAX_COUNT()>{}, 1L);
    }
    enum { result = count_fields() };
};
template<class T>
constexpr std::size_t fields_count()
{
    using flds = count_type_fields<T>;
    return flds::
    #ifdef TEST_HOW
            count_fields();
    #else
            result;
    #endif
}
