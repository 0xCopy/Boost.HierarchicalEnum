#ifndef boost_hierarchical_enum_parents_hpp
#define boost_hierarchical_enum_parents_hpp

#include "details/collection.hpp"

namespace boost {
namespace hierarchical_enum {
    
template< typename Parent_c, typename Child_c >
struct is_parent_of : data::filiation< typename Parent_c::value_type, Parent_c::value, Child_c::value >::value
{};

COLLECTION_FROM_PREDICATE( parents, is_parent_of )

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
