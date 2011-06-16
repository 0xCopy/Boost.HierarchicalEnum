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

#define IMPLEMENT_COLLECTION( singular, plural )                                \
NARY_FUNCTIONS( singular, plural )                                              \
VARIADIC_FUNCTIONS( singular, plural )


#define NARY_FUNCTIONS( singular, plural )                                      \
namespace details {                                                             \
NARY_PREDICATE( BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of ) )            \
NARY_COLLECTION( BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of ), plural )   \
}

#define VARIADIC_FUNCTIONS( singular, plural )                                  \
VARIADIC_PREDICATE( BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of ) )        \
VARIADIC_COLLECTION( plural )

#define NARY_PREDICATE( predicate )                                             \
template< typename u, typename v1 >                                             \
struct BOOST_PP_CAT( predicate, 1 ) : predicate< u, v1 >                        \
{};                                                                             \
                                                                                \
template< typename u, typename v1, typename v2 >                                \
struct BOOST_PP_CAT( predicate, 2 ) : ::boost::mpl::and_<                       \
    predicate< u, v1 >,                                                         \
    predicate< u, v2 >                                                          \
>                                                                               \
{};                                                                             \

#define NARY_COLLECTION( predicate, collection )                                \
template< typename v1 >                                                         \
struct BOOST_PP_CAT( collection, 1 ) : boost::mpl::copy_if<                     \
    data::all< typename v1::value_type >,                                       \
    BOOST_PP_CAT( predicate, 1 )< boost::mpl::_1, v1 >,                         \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};                                                                             \
                                                                                \
template< typename v1, typename v2 >                                            \
struct BOOST_PP_CAT( collection, 2 ) : boost::mpl::copy_if<                     \
    data::all< typename v1::value_type >,                                       \
    BOOST_PP_CAT( predicate, 2 )< boost::mpl::_1, v1, v2 >,                     \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};

#define VARIADIC_PREDICATE( predicate )                                         \
template< typename u, typename v1, typename v2 = details::NoneType >            \
struct predicate;                                                               \
                                                                                \
template< typename u, typename v1 >                                             \
struct predicate< u, v1, details::NoneType > :                                  \
    details::BOOST_PP_CAT( predicate, 1 )< u, v1 >                              \
{};                                                                             \
                                                                                \
template< typename u, typename v1, typename v2 >                                \
struct predicate :                                                              \
    details::BOOST_PP_CAT( predicate, 2 )< u, v1, v2 >                          \
{};

#define VARIADIC_COLLECTION( collection )                                       \
template< typename v1, typename v2 = details::NoneType >                        \
struct collection;                                                              \
                                                                                \
template< typename v1 >                                                         \
struct collection< v1, details::NoneType > :                                    \
    details::BOOST_PP_CAT( collection, 1 )< v1 >                                \
{};                                                                             \
                                                                                \
template< typename v1, typename v2 >                                            \
struct collection :                                                             \
    details::BOOST_PP_CAT( collection, 2 )< v1, v2 >                            \
{};

/*
#define COLLECTION_SIZE( collection )                                           \
template< typename Node_c >                                                     \
struct BOOST_PP_CAT( nb_, collection ) : boost::mpl::size<                      \
    collection< Node_c >                                                        \
>                                                                               \
{};
*/

#endif // Include guard
