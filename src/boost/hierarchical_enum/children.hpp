#ifndef boost_hierarchical_enum_children_hpp
#define boost_hierarchical_enum_children_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy_if.hpp>

namespace boost {
namespace hierarchical_enum {
    
template< typename Child_c, typename Father_c >
struct is_child_of : data::filiation< typename Father_c::value_type, Father_c::value, Child_c::value >::value
{};

template< typename Father_c >
struct children : boost::mpl::copy_if<
    data::all< typename Father_c::value_type >,
    is_child_of< boost::mpl::_1, Father_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
