#ifndef boost_hierarchical_enum_children_hpp
#define boost_hierarchical_enum_children_hpp

#include "details/collection.hpp"

namespace boost {
namespace hierarchical_enum {
    
template< typename Child_c, typename Parent_c >
struct is_child_of : data::filiation< typename Parent_c::value_type, Parent_c::value, Child_c::value >::value
{};

IMPLEMENT_COLLECTION( child, children )

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
