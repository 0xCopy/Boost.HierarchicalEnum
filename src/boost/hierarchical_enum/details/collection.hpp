#ifndef boost_hierarchical_enum_details_collection_hpp
#define boost_hierarchical_enum_details_collection_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/and.hpp>

#define IMPLEMENT_COLLECTION( singular, plural )                                \
NARY_PREDICATE( BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of ) )            \
COLLECTION_FROM_PREDICATE(                                                      \
    plural,                                                                     \
    BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of )                          \
)                                                                               \
COLLECTION_SIZE( plural )

class NoneType {};

#define NARY_PREDICATE( predicate )                                             \
template< typename u, typename v1, typename v2 >                                \
struct BOOST_PP_CAT( predicate, 2 ) : boost::mpl::and_<                         \
    BOOST_PP_CAT( predicate, 1 )< u, v1 >,                                      \
    BOOST_PP_CAT( predicate, 1 )< u, v2 >                                       \
>                                                                               \
{};                                                                             \
                                                                                \
template< typename u, typename v1 = NoneType, typename v2 = NoneType >          \
struct predicate;                                                               \
                                                                                \
template< typename u, typename v1 >                                             \
struct predicate< u, v1, NoneType > :                                           \
    BOOST_PP_CAT( predicate, 1 )< u, v1 >                                       \
{};                                                                             \
                                                                                \
template< typename u, typename v1, typename v2 >                                \
struct predicate :                                                              \
    BOOST_PP_CAT( predicate, 2 )< u, v1, v2 >                                   \
{};

#define COLLECTION_FROM_PREDICATE( collection, predicate )                      \
template< typename Node_c >                                                     \
struct collection : boost::mpl::copy_if<                                        \
    data::all< typename Node_c::value_type >,                                   \
    BOOST_PP_CAT( predicate, 1 )< boost::mpl::_1, Node_c >,                     \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};

#define COLLECTION_SIZE( collection )                                           \
template< typename Node_c >                                                     \
struct BOOST_PP_CAT( nb_, collection ) : boost::mpl::size<                      \
    collection< Node_c >                                                        \
>                                                                               \
{};

#endif // Include guard
