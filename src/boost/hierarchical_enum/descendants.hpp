#ifndef boost_hierarchical_enum_descendants_hpp
#define boost_hierarchical_enum_descendants_hpp

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/count_if.hpp>

#include "children.hpp"

namespace boost {
namespace hierarchical_enum {
namespace details {

template< typename Descendant_c, typename Parent_c >
struct is_strict_descendant_of;

template< typename Descendant_c, typename Parent_c >
struct is_descendant_of :
    ::boost::mpl::or_<
        is_strict_descendant_of<
            Descendant_c,
            Parent_c
        >,
        ::boost::mpl::equal_to<
            Descendant_c,
            Parent_c
        >
    >
{};

/// @todo Use find_if instead of count_if
template< typename Descendant_c, typename Parent_c >
struct is_strict_descendant_of :
    ::boost::mpl::not_<
        ::boost::mpl::equal_to<
            ::boost::mpl::count_if< children< Parent_c >, is_descendant_of< Descendant_c, boost::mpl::_ > >,
            boost::mpl::int_< 0 >
        >
    >
{};

} // Namespace details

BOOST_HE_IMPLEMENT_COLLECTION( descendant, descendants )
BOOST_HE_IMPLEMENT_COLLECTION( strict_descendant, strict_descendants )

} // Namespace hierarchical_enum
} // Namespace boost

#endif // Include guard
