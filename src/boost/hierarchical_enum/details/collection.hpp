#ifndef boost_hierarchical_enum_details_collection_hpp
#define boost_hierarchical_enum_details_collection_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/and.hpp>

namespace boost {
namespace hierarchical_enum {
namespace details {

class NoneType {};
    
} // Namespace details
} // Namespace hierarchical_enum
} // Namespace boost

//********************************************************** IMPLEMENT_COLLECTION
#define BOOST_HE_IMPLEMENT_COLLECTION( singular, plural )                       \
BOOST_HE_VARIADIC_PREDICATE( BOOST_HE_PREDICATE( singular ) )                   \
BOOST_HE_VARIADIC_COLLECTION( BOOST_HE_PREDICATE( singular ), plural )          \
BOOST_HE_VARIADIC_SIZE( plural )

//********************************************************************* UTILITIES
#define BOOST_HE_PREDICATE( singular )                                          \
BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of )

//************************************************************ VARIADIC_FUNCTIONS

#define BOOST_HE_VARIADIC_PREDICATE( predicate )                                \
template< typename u, typename v0, typename v1 = details::NoneType >            \
struct predicate;                                                               \
                                                                                \
template< typename u, typename v0 >                                             \
struct predicate< u, v0, details::NoneType > :                                  \
    details::predicate< u, v0 >                                                 \
{};                                                                             \
                                                                                \
template< typename u, typename v0, typename v1 >                                \
struct predicate : ::boost::mpl::and_<                                          \
    details::predicate< u, v0 >,                                                \
    predicate< u, v1 >                                                          \
>                                                                               \
{};

#define BOOST_HE_VARIADIC_COLLECTION( predicate, collection )                   \
template< typename v0, typename v1 = details::NoneType >                        \
struct collection : boost::mpl::copy_if<                                        \
    data::all< typename v0::value_type >,                                       \
    predicate< boost::mpl::_1, v0, v1 >,                                        \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};

#define BOOST_HE_VARIADIC_SIZE( collection )                                    \
template< typename v0, typename v1 = details::NoneType >                        \
struct BOOST_PP_CAT( nb_, collection ) : boost::mpl::size<                      \
    collection< v0, v1 >                                                        \
>                                                                               \
{};

#endif // Include guard
