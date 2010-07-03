#ifndef boost_hierarchical_enum_ascendants_hpp
#define boost_hierarchical_enum_ascendants_hpp

#include <boost/mpl/or.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/copy_if.hpp>

namespace boost {
namespace hierarchical_enum {

/****************************************************************************
                                   is_strict_ascendant_of and is_ascendant_of
****************************************************************************/

namespace mp {

template< typename Asc_c, typename Desc_c >
struct is_strict_ascendant_of;

template< typename Asc_c, typename Desc_c>
struct is_ascendant_of;

template< typename Asc_c, typename Desc_c >
struct is_strict_ascendant_of : boost::mpl::and_<
    has_father< Desc_c >,
    is_ascendant_of< Asc_c, father< Desc_c > >
>
{};

template< typename Asc_c, typename Desc_c >
struct is_ascendant_of : boost::mpl::or_<
    boost::mpl::equal_to< Asc_c, Desc_c >,
    is_strict_ascendant_of< Asc_c, Desc_c >
>
{};

} // namespace mp

template< typename HE >
bool is_ascendant_of( HE left, HE right ) {
    return details::binary_function< HE, bool, mp::is_ascendant_of >::value( left, right );
}

template< typename HE >
bool is_strict_ascendant_of( HE left, HE right ) {
    return details::binary_function< HE, bool, mp::is_strict_ascendant_of >::value( left, right );
}

/****************************************************************************
                                                                   ascendants
****************************************************************************/

namespace mp {

template< typename Desc_c >
struct ascendants : boost::mpl::copy_if<
    original_data::all< typename Desc_c::value_type >,
    is_ascendant_of< boost::mpl::_1, Desc_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& ascendants( HE e ) {
    return details::unary_function< HE, const std::vector< HE >&, mp::ascendants >::value( e );
}

/****************************************************************************
                                           has_ascendant would always be true
****************************************************************************/

/****************************************************************************
                                                            strict_ascendants
****************************************************************************/

namespace mp {

template< typename Desc_c >
struct strict_ascendants : boost::mpl::copy_if<
    original_data::all< typename Desc_c::value_type >,
    is_strict_ascendant_of< boost::mpl::_1, Desc_c >,
    boost::mpl::back_inserter< boost::mpl::vector<> >
    >::type
{};

} // namespace mp

template< typename HE >
const std::vector< HE >& strict_ascendants( HE e ) {
    return details::unary_function< HE, const std::vector< HE >&, mp::strict_ascendants >::value( e );
}

/****************************************************************************
                       has_strict_ascendant would be equivalent to has_father
****************************************************************************/

} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
