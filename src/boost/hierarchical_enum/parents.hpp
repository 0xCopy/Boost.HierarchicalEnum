#ifndef boost_hierarchical_enum_parents_hpp
#define boost_hierarchical_enum_parents_hpp

#include "details/collection.hpp"

namespace boost {
namespace hierarchical_enum {
    
template< typename Parent_c, typename Child_c >
struct is_parent_of1 : data::filiation< typename Parent_c::value_type, Parent_c::value, Child_c::value >::value
{};

IMPLEMENT_COLLECTION( parent, parents )

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
