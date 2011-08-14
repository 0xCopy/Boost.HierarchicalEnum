#ifndef boost_hierarchical_enum_ascendants_hpp
#define boost_hierarchical_enum_ascendants_hpp

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/count_if.hpp>

#include "parents.hpp"

namespace boost {
namespace hierarchical_enum {
namespace details {

template< typename Ascendant_c, typename Child_c >
struct is_strict_ascendant_of;

template< typename Ascendant_c, typename Child_c >
struct is_ascendant_of :
    ::boost::mpl::or_<
        is_strict_ascendant_of<
            Ascendant_c,
            Child_c
        >,
        ::boost::mpl::equal_to<
            Ascendant_c,
            Child_c
        >
    >
{};

/// @todo Use find_if instead of count_if
template< typename Ascendant_c, typename Child_c >
struct is_strict_ascendant_of :
    ::boost::mpl::not_<
        ::boost::mpl::equal_to<
            ::boost::mpl::count_if< parents< Child_c >, is_ascendant_of< Ascendant_c, boost::mpl::_ > >,
            boost::mpl::int_< 0 >
        >
    >
{};

} // Namespace details

BOOST_HE_IMPLEMENT_COLLECTION( ascendant, ascendants )
BOOST_HE_IMPLEMENT_COLLECTION( strict_ascendant, strict_ascendants )

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
