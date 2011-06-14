#ifndef boost_hierarchical_enum_parents_hpp
#define boost_hierarchical_enum_parents_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy_if.hpp>

namespace boost {
namespace hierarchical_enum {
    
template< typename Parent_c, typename Child_c >
struct is_parent_of : data::filiation< typename Parent_c::value_type, Parent_c::value, Child_c::value >::value
{};

template< typename Child_c >
struct parents : boost::mpl::copy_if<
    data::all< typename Child_c::value_type >,
    is_parent_of< boost::mpl::_1, Child_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
