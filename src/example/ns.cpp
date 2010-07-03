#include <iostream>

#include <boost/hierarchical_enum/hierarchical_enum.hpp>
#include <boost/hierarchical_enum/children.hpp>
#include <boost/hierarchical_enum/ascendants.hpp>

// A hierarchical_enum in a namespace
BOOST_HIERARCHICAL_ENUM_NS(
    (test)(sub_test),
    MovingState,
    (( Still, DEF(), BOOST_PP_SEQ_NIL ))
    (( Running, DEF(), BOOST_PP_SEQ_NIL ))
    (( Creeping, DEF(), BOOST_PP_SEQ_NIL ))
    (( Moving, DEF(), (Walking)(Creeping) ))
    (( Walking, DEF(), (Running) ))
)

#define PRINT( expr ) std::cout << BOOST_PP_STRINGIZE( expr ) << " = " << ( expr ) << std::endl;

int main() {
    namespace bhe = boost::hierarchical_enum;
    namespace tst = test::sub_test;

    PRINT( bhe::is_child_of( tst::Walking, tst::Moving ) );
    PRINT( bhe::is_ascendant_of( tst::Walking, tst::Moving ) );
}
