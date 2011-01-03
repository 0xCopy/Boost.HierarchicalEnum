// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef boost_hierarchical_enum_ct2rt_hpp
#define boost_hierarchical_enum_ct2rt_hpp

#include <vector>

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/bind.hpp>

#include <boost/fusion/include/make_tuple.hpp>
#include <boost/fusion/include/pop_front.hpp>

/// @todo "Mixed-time" version of all binary and ternary functions

namespace boost {
namespace hierarchical_enum {
namespace details {

struct thrower {};

/****************************************************************************
                                                                   translator
****************************************************************************/

template< typename To, typename From, typename enable = void >
struct translator;

template< typename HE_c >
struct translator< typename HE_c::value_type, HE_c, void > {
    static typename HE_c::value_type value() { return HE_c::value; }
};

template< typename To >
struct translator< To, thrower, void > {
    static To value() { throw 0; } /// @todo throw something standard
};

template< typename To, typename From >
struct translator< const std::vector< To >&, From, typename boost::enable_if< boost::mpl::not_< boost::is_same< From, thrower > > >::type > {
    static const std::vector< To >& value() {
        static const std::vector< To > v( valueImpl() );
        return v;
    }
    static std::vector< To > valueImpl() {
        std::vector< To > v;
        boost::mpl::for_each< From >( boost::bind( &std::vector< To >::push_back, &v, _1 ) );
        return v;
    }
};

/****************************************************************************
                                                                  always_true
****************************************************************************/

template< typename HE_c >
struct always_true_1 : boost::mpl::true_
{};

template< typename HE_c1, typename HE_c2 >
struct always_true_2 : boost::mpl::true_
{};

template< typename HE_c1, typename HE_c2, typename HE_c3 >
struct always_true_3 : boost::mpl::true_
{};

/****************************************************************************
                                                                     function
****************************************************************************/

template<
    typename HE,
    typename ReturnType,
    int NumberOfArgs,
    template< typename > class Function,
    template< typename > class Condition,
    class Enums = boost::mpl::vector<>,
    typename enable = void
> struct Switch;

template< typename HE, typename ReturnType, template< typename > class Function, template< typename > class Condition, class Enums >
struct Switch< HE, ReturnType, 0, Function, Condition, Enums > {
    template< typename Tuple >
    static ReturnType exec( Tuple ) {
        return translator<
            ReturnType,
            typename boost::mpl::if_<
                Condition< Enums >,
                Function< Enums >,
                thrower
            >::type
        >::value();
    };
};

template< int NumberOfArguments > struct MyAdapter;
template<> struct MyAdapter< 1 > {
    template< template< typename > class Function >
    struct Adapt {
        template< typename Enums >
        struct type : Function< typename boost::mpl::at_c< Enums, 0 >::type > {};
    };
};
template<> struct MyAdapter< 2 > {
    template< template< typename, typename > class Function >
    struct Adapt {
        template< typename Enums >
        struct type : Function< typename boost::mpl::at_c< Enums, 0 >::type, typename boost::mpl::at_c< Enums, 1 >::type > {};
    };
};
template<> struct MyAdapter< 3 > {
    template< template< typename, typename, typename > class Function >
    struct Adapt {
        template< typename Enums >
        struct type : Function< typename boost::mpl::at_c< Enums, 0 >::type, typename boost::mpl::at_c< Enums, 1 >::type, typename boost::mpl::at_c< Enums, 2 >::type > {};
    };
};

template<
    typename HE,
    typename ReturnType,
    template< typename HE_c > class Function,
    template< typename HE_c > class Condition = always_true_1
>
struct unary_function : Switch<
    HE,
    ReturnType,
    1,
    MyAdapter< 1 >::template Adapt< Function >::template type,
    MyAdapter< 1 >::template Adapt< Condition >::template type
>
{
    static ReturnType value( HE e ) {
        return exec( boost::fusion::make_tuple( e ) );
    }
};

template<
    typename HE,
    typename ReturnType,
    template< typename HE_c1, typename HE_c2 > class Function,
    template< typename HE_c1, typename HE_c2 > class Condition = always_true_2
>
struct binary_function : Switch<
    HE,
    ReturnType,
    2,
    MyAdapter< 2 >::template Adapt< Function >::template type,
    MyAdapter< 2 >::template Adapt< Condition >::template type
>{
    static ReturnType value( HE e1, HE e2 ) {
        return exec( boost::fusion::make_tuple( e1, e2 ) );
    }
};

template<
    typename HE,
    typename ReturnType,
    template< typename HE_c1, typename HE_c2, typename HE_c3 > class Function,
    template< typename HE_c1, typename HE_c2, typename HE_c3 > class Condition = always_true_3
>
struct ternary_function : Switch<
    HE,
    ReturnType,
    3,
    MyAdapter< 3 >::template Adapt< Function >::template type,
    MyAdapter< 3 >::template Adapt< Condition >::template type
> {
    static ReturnType value( HE e1, HE e2, HE e3 ) {
        return exec( boost::fusion::make_tuple( e1, e2, e3 ) );
    }
};


} // namespace details
} // namespace hierarchical_enum
} // namespace boost

#endif // Include guard
