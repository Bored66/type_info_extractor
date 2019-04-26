#ifndef INDEX_SEQ_H
#define INDEX_SEQ_H

#if (__clang_major__ > 2 && __clang_minor__ > 3 && !defined(CPP_11)) || \
    __cplusplus >= 201404  || defined(CPP_14)
#define USE_UTILITY
#else
#define USE_UTILITY_NSTD
#endif
//#undef USE_UTILITY
#ifdef USE_UTILITY
#include <utility>
#include <type_traits>
#else
/*
* Borrowed from here:
* https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence/
*/
namespace std {
#if __code_model_32__
using size_t = unsigned int;
#else
using size_t = unsigned long;
#endif
template< class T, T... Ints >
struct integer_sequence
{
//public:
    static constexpr std::size_t size() noexcept
    { return sizeof...(Ints); }
    typedef T value_type;
    using type = integer_sequence;
    static_assert(
      std::is_integral<T>::value,
      "std::integer_sequence can only be instantiated with an integral type" );
};
template<std::size_t... Ints>
using index_sequence = std::integer_sequence<std::size_t, Ints...>;
// --------------------------------------------------------------

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <std::size_t... I1, std::size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
  : index_sequence<I1..., (sizeof...(I1)+I2)...>
{ };

// --------------------------------------------------------------

template <std::size_t N>
struct make_index_sequence
  : _merge_and_renumber<typename make_index_sequence<N/2>::type,
                        typename make_index_sequence<N - N/2>::type>
{
    using base = _merge_and_renumber<typename make_index_sequence<N/2>::type,
    typename make_index_sequence<N - N/2>::type>;
};

template<> struct make_index_sequence<0> : index_sequence<> {};// { using base = index_sequence<>; };
template<> struct make_index_sequence<1> : index_sequence<0>{};// { using base = index_sequence<0>; };
/* end of borrowed code */
//template<> struct make_index_sequence<2> : index_sequence<0,1> { };
//template<> struct make_index_sequence<3> : index_sequence<0,1,2> { };

template<class T, T N>
using make_integer_sequence = integer_sequence<T, N>;

template< bool B, class T = void >
using enable_if_t = typename enable_if<B,T>::type;

template< class T >
using remove_reference_t = typename remove_reference<T>::type;

template< class T >
using remove_all_extents_t = typename remove_all_extents<T>::type;

template< class T >
using add_const_t    = typename add_const<T>::type;
/*
template< class T >
using is_trivially_destructible = typename has_trivial_destructor<T>::value;
*/
template< class T >
using add_pointer_t = typename add_pointer<T>::type;
}
#endif

#endif // INDEX_SEQ_H
