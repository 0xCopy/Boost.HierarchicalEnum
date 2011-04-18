// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef boost_hierarchical_enum_descendants_hpp
#define boost_hierarchical_enum_descendants_hpp

#include <boost/hierarchical_enum/ascendants.hpp>

namespace boost {
namespace hierarchical_enum {

/****************************************************************************
                                                             is_descendant_of
****************************************************************************/

namespace mp {

template< typename Desc_c, typename Asc_c >
struct is_descendant_of : is_ascendant_of< Asc_c, Desc_c >
{};

} // namespace mp

template< typename HE >
bool is_descendant_of( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::is_descendant_of
    >::value( left, right );
}

/****************************************************************************
                                                                  descendants
****************************************************************************/

namespace mp {

template< typename Asc_c >
struct descendants : boost::mpl::copy_if<
    original_data::all< typename Asc_c::value_type >,
    is_descendant_of< boost::mpl::_1, Asc_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& descendants( HE e ) {
    return details::unary_function<
        HE,
        const std::vector< HE >&,
        mp::descendants
    >::value( e );
}

/****************************************************************************
                                          has_descendant would always be true
****************************************************************************/

/****************************************************************************
                                                      is_strict_descendant_of
****************************************************************************/

namespace mp {

template< typename Desc_c, typename Asc_c >
struct is_strict_descendant_of : is_strict_ascendant_of< Asc_c, Desc_c >
{};

} // namespace mp

template< typename HE >
bool is_strict_descendant_of( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::is_strict_descendant_of
    >::value( left, right );
}

/****************************************************************************
                                                           strict_descendants
****************************************************************************/

namespace mp {

template< typename Asc_c >
struct strict_descendants : boost::mpl::copy_if<
    original_data::all< typename Asc_c::value_type >,
    is_strict_descendant_of< boost::mpl::_1, Asc_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& strict_descendants( HE e ) {
    return details::unary_function<
        HE,
        const std::vector< HE >&,
        mp::strict_descendants
    >::value( e );
}

/****************************************************************************
                    has_strict_descendant would be equivalent to has_children
****************************************************************************/

} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
