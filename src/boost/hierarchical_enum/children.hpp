#ifndef boost_hierarchical_enum_children_hpp
#define boost_hierarchical_enum_children_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy_if.hpp>

namespace boost {
namespace hierarchical_enum {
    
template< typename Child_c, typename Parent_c >
struct is_child_of : data::filiation< typename Parent_c::value_type, Parent_c::value, Child_c::value >::value
{};

template< typename Parent_c >
struct children : boost::mpl::copy_if<
    data::all< typename Parent_c::value_type >,
    is_child_of< boost::mpl::_1, Parent_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
