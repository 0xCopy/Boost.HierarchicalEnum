#ifndef boost_hierarchical_enum_details_collection_hpp
#define boost_hierarchical_enum_details_collection_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/and.hpp>

#define BOOST_HE_MAX_ARITY 4 // More than 4 doesn't compile in the copy_if...

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
template< typename u, typename v0, BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS( BOOST_HE_MAX_ARITY, typename v, = details::NoneType BOOST_PP_INTERCEPT ) >            \
struct predicate : ::boost::mpl::and_<                                          \
    details::predicate< u, v0 >,                                                \
    predicate< u, BOOST_PP_ENUM_SHIFTED_PARAMS( BOOST_HE_MAX_ARITY, v ) >                                                          \
>                                                                               \
{};                                                                             \
                                                                                \
template< typename u, typename v0 >                                             \
struct predicate< u, v0 > :                                  \
    details::predicate< u, v0 >                                                 \
{};

#define BOOST_HE_VARIADIC_COLLECTION( predicate, collection )                   \
template< typename v0, BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS( BOOST_HE_MAX_ARITY, typename v, = details::NoneType BOOST_PP_INTERCEPT ) >                        \
struct collection : boost::mpl::copy_if<                                        \
    data::all< typename v0::value_type >,                                       \
    predicate< boost::mpl::_1, BOOST_PP_ENUM_PARAMS( BOOST_HE_MAX_ARITY, v ) >,                                        \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};

#define BOOST_HE_VARIADIC_SIZE( collection )                                    \
template< typename v0, BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS( BOOST_HE_MAX_ARITY, typename v, = details::NoneType BOOST_PP_INTERCEPT ) >                        \
struct BOOST_PP_CAT( nb_, collection ) : boost::mpl::size<                      \
    collection< BOOST_PP_ENUM_PARAMS( BOOST_HE_MAX_ARITY, v ) >                                                        \
>                                                                               \
{};

#endif // Include guard
