#ifndef FLIP_BYTES_BITS_H
#define FLIP_BYTES_BITS_H
/*FIXME: it turns out do not need all the stuff below
even though it was a nice exercise in functional C++ programming
with bit masks
*/
/* code below was kept as comment/algorithm explanation
 *
constexpr auto tt5 = encode_type<test_type5>();
auto fund_tid = tt5 & cypher_base::fund_type_mask;
auto tid5 = tt5>>4;
auto result = 0;
for(;tid5; tid5>>=4, result<<=4)
    result |= tid5 & cypher_base::fund_type_mask;
printf("rev: 0x%.8X\n", flip_bytes_bits(encode_type<test_type5>()));
*/
constexpr std::size_t reverse(std::size_t result, std::size_t type_id)
{
    return type_id?
        reverse( ( (result | (type_id & cypher_base::fund_type_mask) )
                                                << cypher_base::shift ),
            type_id >> cypher_base::shift ) :
            result;
}
constexpr std::size_t flip_bytes_bits(std::size_t type_id)
{
    return reverse(0, type_id >> cypher_base::shift) |
                        (type_id & cypher_base::fund_type_mask);
}

#endif // FLIP_BYTES_BITS_H
