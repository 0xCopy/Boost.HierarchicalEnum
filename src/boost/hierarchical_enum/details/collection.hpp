#ifndef boost_hierarchical_enum_details_collection_hpp
#define boost_hierarchical_enum_details_collection_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>

#define IMPLEMENT_COLLECTION( singular, plural )                                \
COLLECTION_FROM_PREDICATE(                                                      \
    plural,                                                                     \
    BOOST_PP_CAT( BOOST_PP_CAT( is_, singular ), _of )                          \
)                                                                               \
COLLECTION_SIZE( plural )

#define COLLECTION_FROM_PREDICATE( collection, predicate )                      \
template< typename Node_c >                                                     \
struct collection : boost::mpl::copy_if<                                        \
    data::all< typename Node_c::value_type >,                                   \
    predicate< boost::mpl::_1, Node_c >,                                        \
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
