#ifndef boost_hierarchical_enum_hierarchical_enum_hpp
#define boost_hierarchical_enum_hierarchical_enum_hpp

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/empty.hpp>
/// @todo Limit inclusions of boost/preprocessor

#include <boost/mpl/vector_c.hpp>

#include <boost/hierarchical_enum/father.hpp>

#include <boost/utility/enable_if.hpp>

/****************************************************************************
                                                      BOOST_HIERARCHICAL_ENUM
****************************************************************************/

#define BOOST_HIERARCHICAL_ENUM_NS( ns, name, nodes )                       \
BOOST_HE_OPEN_NS( ns )                                                      \
BOOST_HE_ENUM( name, nodes )                                                \
BOOST_HE_INTEGRAL_CONSTANTS( name, nodes )                                  \
BOOST_HE_CLOSE_NS( ns )                                                     \
BOOST_HE_FILIATIONS( BOOST_HE_CAT_NS( ns ), name, nodes )                   \
BOOST_HE_ALL( BOOST_HE_CAT_NS( ns ), name, nodes )                          \
BOOST_HE_SWITCH( BOOST_HE_CAT_NS( ns ), name, nodes )


#define BOOST_HIERARCHICAL_ENUM( name, nodes )                              \
BOOST_HE_ENUM( name, nodes )                                                \
BOOST_HE_INTEGRAL_CONSTANTS( name, nodes )                                  \
BOOST_HE_FILIATIONS( ::, name, nodes )                                      \
BOOST_HE_ALL( ::, name, nodes )                                             \
BOOST_HE_SWITCH( ::, name, nodes )

/****************************************************************************
                                                              BOOST_HE_NODE_*
****************************************************************************/

#define BOOST_HE_NODE_NAME( node )                                          \
BOOST_PP_TUPLE_ELEM( 3, 0, node )

#define BOOST_HE_NODE_VALUE( node )                                         \
BOOST_PP_TUPLE_ELEM( 3, 1, node )

#define BOOST_HE_NODE_SONS( node )                                          \
BOOST_PP_TUPLE_ELEM( 3, 2, node )

/****************************************************************************
                                                                BOOST_HE_ENUM
****************************************************************************/

#define BOOST_HE_ENUM( name, nodes )                                        \
enum name { BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_TRANSFORM( BOOST_HE_ENUM__MACRO, name, nodes ) ) };

#define BOOST_HE_ENUM__MACRO( s, name, node )                               \
BOOST_HE_NODE_NAME( node ) BOOST_PP_CAT( BOOST_HE_ENUM__MACRO_VAL_, BOOST_HE_NODE_VALUE( node ) )

#define BOOST_HE_ENUM__MACRO_VAL_DEF()

#define BOOST_HE_ENUM__MACRO_VAL_VAL( val ) = val

/****************************************************************************
                                                  BOOST_HE_INTEGRAL_CONSTANTS
****************************************************************************/

#define BOOST_HE_INTEGRAL_CONSTANTS( name, nodes )                          \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_INTEGRAL_CONSTANTS__MACRO, name, nodes )

#define BOOST_HE_INTEGRAL_CONSTANTS__MACRO( r, name, node )                 \
typedef boost::mpl::integral_c< name, BOOST_HE_NODE_NAME( node ) >          \
BOOST_HE_MAKE_INTEGRAL_C( BOOST_HE_NODE_NAME( node ) );

#define BOOST_HE_MAKE_INTEGRAL_C( value )                                   \
BOOST_PP_CAT( value, _c )

/****************************************************************************
                                                                 BOOST_HE_ALL
****************************************************************************/

#define BOOST_HE_ALL( ns, name, nodes )                                     \
namespace boost {                                                           \
namespace hierarchical_enum {                                               \
namespace mp {                                                              \
namespace original_data {                                                   \
template<>                                                                  \
struct all< ns name > : boost::mpl::vector_c<                               \
    ns name,                                                                \
    BOOST_PP_SEQ_ENUM(                                                      \
        BOOST_PP_SEQ_TRANSFORM( BOOST_HE_ALL__MACRO, ns, nodes )            \
    )                                                                       \
> {};                                                                       \
}                                                                           \
}                                                                           \
}                                                                           \
}

#define BOOST_HE_ALL__MACRO( s, ns, node )                                  \
ns BOOST_HE_NODE_NAME( node )

/****************************************************************************
                                                              BOOST_HE_SWITCH
****************************************************************************/

#define BOOST_HE_SWITCH( ns, name, nodes )                                  \
namespace boost {                                                           \
namespace hierarchical_enum {                                               \
namespace details {                                                         \
template<                                                                   \
    typename ReturnType,                                                    \
    int NumberOfArgs,                                                       \
    template< typename > class Function,                                    \
    template< typename > class Condition,                                   \
    class Enums                                                             \
> struct Switch<                                                            \
    ns name,                                                                \
    ReturnType,                                                             \
    NumberOfArgs,                                                           \
    Function,                                                               \
    Condition,                                                              \
    Enums,                                                                  \
    typename boost::enable_if_c< NumberOfArgs != 0 >::type                  \
> {                                                                         \
    template< typename Tuple >                                              \
    static ReturnType exec( Tuple enums ) {                                 \
        switch( boost::fusion::at_c< 0 >( enums ) ) {                       \
            BOOST_PP_SEQ_FOR_EACH(                                          \
                BOOST_HE_SWITCH__MACRO,                                     \
                ( ns, name ),                                               \
                nodes                                                       \
            )                                                               \
            default: return translator< ReturnType, thrower >::value();     \
        }                                                                   \
    }                                                                       \
};                                                                          \
}                                                                           \
}                                                                           \
}

#define BOOST_HE_SWITCH__DATA_NS( data )                                    \
BOOST_PP_TUPLE_ELEM( 2, 0, data )

#define BOOST_HE_SWITCH__DATA_NAME( data )                                  \
BOOST_PP_TUPLE_ELEM( 2, 1, data )

#define BOOST_HE_SWITCH__MACRO( r, data, node )                             \
case BOOST_HE_SWITCH__DATA_NS( data ) BOOST_HE_NODE_NAME( node ):           \
    return Switch<                                                          \
        BOOST_HE_SWITCH__DATA_NS( data ) BOOST_HE_SWITCH__DATA_NAME( data ),\
        ReturnType,                                                         \
        NumberOfArgs - 1,                                                   \
        Function,                                                           \
        Condition,                                                          \
        typename boost::mpl::push_back<                                     \
            Enums,                                                          \
            BOOST_HE_SWITCH__DATA_NS( data ) BOOST_HE_MAKE_INTEGRAL_C( BOOST_HE_NODE_NAME( node ) ) \
        >::type                                                             \
    >::exec( boost::fusion::pop_front( enums ) );

/****************************************************************************
                                                              BOOST_HE_CAT_NS
****************************************************************************/

#define BOOST_HE_CAT_NS( ns )                                               \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_CAT_NS__MACRO, _, ns )

#define BOOST_HE_CAT_NS__MACRO( r, data, level )                            \
level ::

/****************************************************************************
                                       BOOST_HE_OPEN_NS and BOOST_HE_CLOSE_NS
****************************************************************************/

#define BOOST_HE_OPEN_NS( ns )                                              \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_OPEN_NS__MACRO, _, ns )

#define BOOST_HE_OPEN_NS__MACRO( r, data, level )                           \
namespace level {

#define BOOST_HE_CLOSE_NS( ns )                                             \
BOOST_PP_SEQ_FOR_EACH( BOOST_HE_CLOSE_NS__MACRO, _, ns )

#define BOOST_HE_CLOSE_NS__MACRO( r, data, level )                          \
}

/****************************************************************************
                                                          BOOST_HE_FILIATIONS
****************************************************************************/

#define BOOST_HE_FILIATIONS( ns, name, nodes )                              \
namespace boost {                                                           \
namespace hierarchical_enum {                                               \
namespace mp {                                                              \
namespace original_data {                                                   \
BOOST_PP_FOR(                                                               \
    ( ns, name, nodes ),                                                    \
    BOOST_HE_FILIATIONS__PRED,                                              \
    BOOST_HE_FILIATIONS__OP,                                                \
    BOOST_HE_FILIATIONS__MACRO                                              \
)                                                                           \
}                                                                           \
}                                                                           \
}                                                                           \
}

#define BOOST_HE_FILIATIONS__STATE_NS( s )                                  \
BOOST_PP_TUPLE_ELEM( 3, 0, s )

#define BOOST_HE_FILIATIONS__STATE_NAME( s )                                \
BOOST_PP_TUPLE_ELEM( 3, 1, s )

#define BOOST_HE_FILIATIONS__STATE_NODES( s )                               \
BOOST_PP_TUPLE_ELEM( 3, 2, s )

#define BOOST_HE_FILIATIONS__PRED( r, state )                               \
BOOST_PP_SEQ_SIZE( BOOST_HE_FILIATIONS__STATE_NODES( state ) )

#define BOOST_HE_FILIATIONS__OP( r, state )                                 \
(                                                                           \
    BOOST_HE_FILIATIONS__STATE_NS( state ),                                 \
    BOOST_HE_FILIATIONS__STATE_NAME( state ),                               \
    BOOST_PP_SEQ_TAIL( BOOST_HE_FILIATIONS__STATE_NODES( state ) )          \
)

#define BOOST_HE_FILIATIONS__MACRO( r, state )                              \
BOOST_HE_FILIATION(                                                         \
    BOOST_HE_FILIATIONS__STATE_NS( state ),                                 \
    BOOST_HE_FILIATIONS__STATE_NAME( state ),                               \
    BOOST_HE_NODE_NAME(                                                     \
        BOOST_PP_SEQ_HEAD( BOOST_HE_FILIATIONS__STATE_NODES( state ) )      \
    ),                                                                      \
    BOOST_HE_NODE_SONS(                                                     \
        BOOST_PP_SEQ_HEAD( BOOST_HE_FILIATIONS__STATE_NODES( state ) )      \
    )                                                                       \
)

/***************************************************************************/

#define BOOST_HE_FILIATION( ns, name, father, sons )                        \
BOOST_PP_FOR(                                                               \
    ( ns, name, father, sons ),                                             \
    BOOST_HE_FILIATION__PRED,                                               \
    BOOST_HE_FILIATION__OP,                                                 \
    BOOST_HE_FILIATION__MACRO                                               \
)

#define BOOST_HE_FILIATION__STATE_NS( s )                                   \
BOOST_PP_TUPLE_ELEM( 4, 0, s )

#define BOOST_HE_FILIATION__STATE_NAME( s )                                 \
BOOST_PP_TUPLE_ELEM( 4, 1, s )

#define BOOST_HE_FILIATION__STATE_FATHER( s )                               \
BOOST_PP_TUPLE_ELEM( 4, 2, s )

#define BOOST_HE_FILIATION__STATE_SONS( s )                                 \
BOOST_PP_TUPLE_ELEM( 4, 3, s )

#define BOOST_HE_FILIATION__PRED( r, state )                                \
BOOST_PP_DEC( BOOST_PP_SEQ_SIZE( BOOST_PP_SEQ_PUSH_BACK( BOOST_HE_FILIATION__STATE_SONS( state ), dummy ) ) )

#define BOOST_HE_FILIATION__OP( r, state )                                  \
(                                                                           \
    BOOST_HE_FILIATION__STATE_NS( state ),                                  \
    BOOST_HE_FILIATION__STATE_NAME( state ),                                \
    BOOST_HE_FILIATION__STATE_FATHER( state ),                              \
    BOOST_PP_SEQ_TAIL( BOOST_HE_FILIATION__STATE_SONS( state ) )            \
)

#define BOOST_HE_FILIATION__MACRO( r, state )                               \
template<> struct                                                           \
father<                                                                     \
    BOOST_HE_FILIATION__STATE_NS( state )                                   \
    BOOST_HE_MAKE_INTEGRAL_C(                                               \
        BOOST_PP_SEQ_HEAD( BOOST_HE_FILIATION__STATE_SONS( state ) )        \
    )                                                                       \
> : BOOST_HE_FILIATION__STATE_NS( state )                                   \
    BOOST_HE_MAKE_INTEGRAL_C(                                               \
        BOOST_HE_FILIATION__STATE_FATHER( state )                           \
    )                                                                       \
{                                                                           \
    typedef boost::mpl::true_ exists;                                       \
};

/***************************************************************************/

#endif // Include guard
