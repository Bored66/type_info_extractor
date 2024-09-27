#include "test_types.h"
#include "../pod_inspector.h"

#include "pod_ops.h"

void pod_ops_demo()
{
    user_def_struct1 uds1 {nullptr,1,2,'3',{4.4f,5.5},'6',7};
    simple_enclosure se{11,uds1,0xffff,1.2f,2.3f};
    simple_enclosure2 se2{11,uds1,1,{1.2f,2.3},1};
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    dump_struct(uds1);
    dump_struct(se);
    dump_struct(se2);
    test_pod_ops_2<5,6>(se);
    test_pod_ops_2<5,6>(se2);
    constexpr auto counter_se = get_type_total_count<decltype(se)>();
    test_pod_ops_2<counter_se-2,counter_se-1>(se);
    constexpr auto counter_se2 = get_type_total_count<decltype(se2)>();
    test_pod_ops_2<counter_se2-2,counter_se2-1>(se2);
}
