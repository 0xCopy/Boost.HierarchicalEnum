#ifndef boost_hierarchical_enum_is_child_of_hpp
#define boost_hierarchical_enum_is_child_of_hpp

#include "details/collection.hpp"

namespace boost {
namespace hierarchical_enum {
namespace details {

template< typename Child_c, typename Parent_c >
struct is_child_of :
    data::filiation<
        typename Parent_c::value_type,
        Parent_c::value,
        Child_c::value
    >::value
{};

} // Namespace details
} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
