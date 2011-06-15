#ifndef boost_hierarchical_enum_details_collection_hpp
#define boost_hierarchical_enum_details_collection_hpp

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/copy_if.hpp>

#define COLLECTION_FROM_PREDICATE( collection, predicate )                      \
template< typename Node_c >                                                     \
struct collection : boost::mpl::copy_if<                                        \
    data::all< typename Node_c::value_type >,                                   \
    predicate< boost::mpl::_1, Node_c >,                                        \
    boost::mpl::back_inserter< boost::mpl::vector<> >                           \
    >::type                                                                     \
{};                                                                             \
                                                                                \
template< typename Node_c >                                                     \
struct BOOST_PP_CAT( nb_, collection ) : boost::mpl::size<                      \
    collection< Node_c >                                                        \
>                                                                               \
{};

#endif // Include guard
