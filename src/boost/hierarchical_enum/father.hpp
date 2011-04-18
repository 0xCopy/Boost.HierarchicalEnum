// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef boost_hierarchical_enum_father_hpp
#define boost_hierarchical_enum_father_hpp

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/hierarchical_enum/ct2rt.hpp>

namespace boost {
namespace hierarchical_enum {

namespace mp {
namespace original_data {

template< typename >
struct father {
    typedef boost::mpl::false_ exists;
};

template< typename >
struct all;

} // namespace original_data
} // namespace mp

/****************************************************************************
                                                                   has_father
****************************************************************************/

namespace mp {

template< typename HE_c >
struct has_father : original_data::father< typename HE_c::type >::exists
{};

} // namespace mp

template< typename HE >
bool has_father( HE e ) {
    return details::unary_function< HE, bool, mp::has_father >::value( e );
}

/****************************************************************************
                                                                       father
****************************************************************************/

namespace mp {
namespace details {

template< typename HE_c, typename enable = void >
struct father;

template< typename HE_c >
struct father< HE_c,
    typename boost::enable_if<
        has_father< HE_c >
    >::type
> : original_data::father< typename HE_c::type >::type
{};

} // namespace details

template< typename HE_c >
struct father : details::father< HE_c >
{};

} // namespace mp

template< typename HE >
HE father( HE e ) {
    return details::unary_function<
        HE,
        HE,
        mp::father,
        mp::has_father
    >::value( e );
}

/****************************************************************************
                                                                 is_father_of
****************************************************************************/

namespace mp {

template< typename Fath_c, typename Child_c >
struct is_father_of : boost::mpl::and_<
    has_father< Child_c >,
    boost::mpl::equal_to< Fath_c, father< Child_c > >
>
{};

} // namespace mp

template< typename HE >
bool is_father_of( HE left, HE right ) {
    return details::binary_function<
        HE,
        bool,
        mp::is_father_of
    >::value( left, right );
}

} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
