#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

#include <boost/hierarchical_enum/hierarchical_enum.hpp>
#include <boost/hierarchical_enum/children.hpp>
#include <boost/hierarchical_enum/descendants.hpp>
#include <boost/hierarchical_enum/common_ascendants.hpp>

// A hierarchical_enum outside any namespace
BOOST_HIERARCHICAL_ENUM(
    ThreeLevels,
    (( GrandFather, DEF(), (Father) ))
    (( Father, VAL( 42 ), (Son) ))
    (( Son, DEF(), BOOST_PP_SEQ_NIL ))
)

std::ostream& operator<<( std::ostream& str, ThreeLevels e ) {
    switch( e ) {
        case GrandFather: return str << "GrandFather";
        case Father: return str << "Father";
        case Son: return str << "Son";
        default: throw 0;
    }
}

std::ostream& operator<<( std::ostream& str, std::vector< ThreeLevels > v ) {
    std::copy( v.begin(), v.end(), std::ostream_iterator< ThreeLevels >( str, " " ) );
    return str;
}

#define PRINT( expr ) std::cout << BOOST_PP_STRINGIZE( expr ) << " = " << ( expr ) << std::endl;

int main() {
    namespace bhe = boost::hierarchical_enum;

    PRINT( bhe::mp::father< Son_c >::value );
    PRINT( bhe::mp::father< Father_c >::value );
    PRINT( bhe::mp::father< bhe::mp::father< Son_c > >::value );

    PRINT( bhe::is_descendant_of( Son, Father ) );
    PRINT( bhe::is_father_of( Son, Father ) );
    //PRINT( bhe::is_descendant_of< Father_c >( Son ) );

    PRINT( bhe::father( Son ) );
    PRINT( bhe::father( Father ) );
    PRINT( bhe::father( bhe::father( Son ) ) );
    try {
        bhe::father( GrandFather );
    } catch( ... ) {
        std::cout << "Got an exception while trying to call bhe::father( GrandFather )" << std::endl;
    }

    PRINT( bhe::has_father( Son ) );
    PRINT( bhe::has_father( Father ) );
    PRINT( bhe::has_father( GrandFather ) );

    PRINT( bhe::has_child( Son ) );
    PRINT( bhe::has_child( Father ) );
    PRINT( bhe::has_child( GrandFather ) );

    PRINT( bhe::children( Son ) );
    PRINT( bhe::children( Father ) );
    PRINT( bhe::children( GrandFather ) );

    PRINT( bhe::common_ascendants( Father, Son ) );
    PRINT( bhe::have_common_ascendant( Father, Son ) );
}
