#ifndef boost_hierarchical_enum_hierarchical_enum_hpp
#define boost_hierarchical_enum_hierarchical_enum_hpp

#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor.hpp> // @todo Limit inclusion of preprocessor

namespace boost {
namespace hierarchical_enum {
namespace data {
    template< typename T, T parent, T child >
    struct filiation {
        typedef boost::mpl::false_ value;
    };
    
    template< typename T >
    struct all {
    };
} // Namespace data
} // Namespace hierarchical_enum
} // Namespace boost

/***************************************************** BOOST_HIERARCHICAL_ENUM */

// @todo See BOOST_FUSION_ADAPT_STRUCT_FILLER_0 to remove the double parenthesis in BOOST_HIERARCHICAL_ENUM
// @todo See BOOST_FUSION_DEFINE_STRUCT for how to deal with namespaces
#define BOOST_HIERARCHICAL_ENUM( name, values, filiations )                     \
BOOST_HE_DEFINE_ENUM( name, values )                                            \
BOOST_HE_DEFINE_MPL_CONSTANTS( name, values )                                   \
namespace boost {                                                               \
namespace hierarchical_enum {                                                   \
namespace data {                                                                \
BOOST_HE_SPECIALIZE_ALL( name, values )                                         \
BOOST_HE_SPECIALIZE_FILIATIONS( name, filiations )                              \
}                                                                               \
}                                                                               \
}

/******************************************************** BOOST_HE_DEFINE_ENUM */

#define BOOST_HE_DEFINE_ENUM( name, values )                                    \
enum name { BOOST_PP_SEQ_ENUM( values ) };

/*********************************************** BOOST_HE_DEFINE_MPL_CONSTANTS */
#define BOOST_HE_DEFINE_MPL_CONSTANTS( name, values )                           \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_DEFINE_MPL_CONSTANT, name, values )

#define BOOST_HE_DEFINE_MPL_CONSTANT( r, name, value )                          \
typedef boost::mpl::integral_c< name, value > BOOST_PP_CAT( value, _c );

/********************************************** BOOST_HE_SPECIALIZE_FILIATIONS */

#define BOOST_HE_SPECIALIZE_FILIATIONS( name, filiations )                      \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_SPECIALIZE_FILIATION, name, filiations )

#define BOOST_HE_SPECIALIZE_FILIATION( r, name, _filiation )                    \
template<>                                                                      \
struct filiation<                                                               \
    name,                                                                       \
    BOOST_PP_TUPLE_ELEM( 2, 0, _filiation ),                                    \
    BOOST_PP_TUPLE_ELEM( 2, 1, _filiation )                                     \
> {                                                                             \
        typedef boost::mpl::true_ value;                                        \
};

/***************************************************** BOOST_HE_SPECIALIZE_ALL */
#define BOOST_HE_SPECIALIZE_ALL( name, values )                                 \
template<>                                                                      \
struct all< name > : boost::mpl::vector_c<                                      \
    name,                                                                       \
    BOOST_PP_SEQ_ENUM( values )                                                 \
> {};

#endif // Include guard
