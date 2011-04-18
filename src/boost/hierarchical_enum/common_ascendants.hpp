// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef boost_hierarchical_enum_common_ascendants_hpp
#define boost_hierarchical_enum_common_ascendants_hpp

#include <boost/hierarchical_enum/ascendants.hpp>
#include <boost/mpl/and.hpp>

namespace boost {
namespace hierarchical_enum {

/****************************************************************************
                                                       is_common_ascendant_of
****************************************************************************/

namespace mp {

template< typename Asc_c, typename Desc1_c, typename Desc2_c >
struct is_common_ascendant_of : boost::mpl::and_<
    is_ascendant_of< Asc_c, Desc1_c >,
    is_ascendant_of< Asc_c, Desc2_c >
>
{};

} // namespace mp

template< typename HE >
bool is_common_ascendant_of( HE left, HE middle, HE right ) {
    return details::ternary_function<
        HE,
        bool,
        mp::is_common_ascendant_of
    >::value( left, middle, right );
}

/****************************************************************************
                                                            common_ascendants
****************************************************************************/

namespace mp {

template< typename Desc1_c, typename Desc2_c >
struct common_ascendants : boost::mpl::copy_if<
    original_data::all< typename Desc1_c::value_type >,
    is_common_ascendant_of< boost::mpl::_1, Desc1_c, Desc2_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
>::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& common_ascendants( HE left, HE right ) {
    return details::binary_function<
        HE,
        const std::vector< HE >&,
        mp::common_ascendants
    >::value( left, right );
}

/****************************************************************************
                                                        have_common_ascendant
****************************************************************************/

namespace mp {

template< typename Desc1_c, typename Desc2_c >
struct have_common_ascendant : boost::mpl::not_<
    boost::mpl::empty< common_ascendants< Desc1_c, Desc2_c > >
>
{};

} // namespace mp

template< typename HE >
bool have_common_ascendant( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::have_common_ascendant
    >::value( left, right );
}

/****************************************************************************
                                                is_common_strict_ascendant_of
****************************************************************************/

namespace mp {

template< typename Asc_c, typename Desc1_c, typename Desc2_c >
struct is_common_strict_ascendant_of : boost::mpl::and_<
    is_strict_ascendant_of< Asc_c, Desc1_c >,
    is_strict_ascendant_of< Asc_c, Desc2_c >
>
{};

} // namespace mp

template< typename HE >
bool is_common_strict_ascendant_of( HE left, HE middle, HE right ) {
    return details::ternary_function<
        HE,
        bool,
        mp::is_common_strict_ascendant_of
    >::value( left, middle, right );
}

/****************************************************************************
                                                     common_strict_ascendants
****************************************************************************/

namespace mp {

template< typename Desc1_c, typename Desc2_c >
struct common_strict_ascendants : boost::mpl::copy_if<
    original_data::all< typename Desc1_c::value_type >,
    is_common_strict_ascendant_of< boost::mpl::_1, Desc1_c, Desc2_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& common_strict_ascendants( HE left, HE right ) {
    return details::binary_function<
        HE,
        const std::vector< HE >&,
        mp::common_strict_ascendants
    >::value( left, right );
}

/****************************************************************************
                                                 have_common_strict_ascendant
****************************************************************************/

namespace mp {

template< typename Desc1_c, typename Desc2_c >
struct have_common_strict_ascendant : boost::mpl::not_<
    boost::mpl::empty< common_strict_ascendants< Desc1_c, Desc2_c > >
>
{};

} // namespace mp

template< typename HE >
bool have_common_strict_ascendant( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::have_common_strict_ascendant
    >::value( left, right );
}

/// @todo first_common_ascendant, first_common_strict_ascendant

} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
