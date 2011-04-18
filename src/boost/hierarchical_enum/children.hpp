// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef boost_hierarchical_enum_children_hpp
#define boost_hierarchical_enum_children_hpp

#include <boost/hierarchical_enum/father.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace hierarchical_enum {

/****************************************************************************
                                                                  is_child_of
****************************************************************************/

namespace mp {

template< typename Child_c, typename Fath_c >
struct is_child_of : is_father_of< Fath_c, Child_c >
{};

} // namespace mp

template< typename HE >
bool is_child_of( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::is_child_of
    >::value( left, right );
}

/****************************************************************************
                                                                     children
****************************************************************************/

namespace mp {

template< typename Asc_c >
struct children : boost::mpl::copy_if<
    original_data::all< typename Asc_c::value_type >,
    is_child_of< boost::mpl::_1, Asc_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& children( HE e ) {
    return details::unary_function<
        HE,
        const std::vector< HE >&,
        mp::children
    >::value( e );
}

/****************************************************************************
                                                                    has_child
****************************************************************************/

namespace mp {

template< typename Asc_c >
struct has_child : boost::mpl::not_<
    boost::mpl::empty< children< Asc_c > >
>
{};

} // namespace mp

template< typename HE >
bool has_child( HE e ) {
    return details::unary_function< HE, bool, mp::has_child >::value( e );
}

} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
