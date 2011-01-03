// Copyright 2010-2011 Vincent Jacques <vincent@vincent-jacques.net>
// Distributed under the Boost license, version 1.0
// See http://www.boost.org/LICENSE_1_0.txt

#include <boost/hierarchical_enum/hierarchical_enum.hpp>
#include <boost/hierarchical_enum/children.hpp>

BOOST_HIERARCHICAL_ENUM(
    Test,
    (( A, DEF(), (AA)(AB) ))
    (( B, DEF(), (BA)(BB) ))
    (( AA, DEF(), BOOST_PP_SEQ_NIL ))
    (( AB, DEF(), BOOST_PP_SEQ_NIL ))
    (( BA, DEF(), BOOST_PP_SEQ_NIL ))
    (( BB, DEF(), BOOST_PP_SEQ_NIL ))
);

int main() {
    namespace bhe = boost::hierarchical_enum;

    Test bb = BB;
    Test b;
    for( int i = 0; i < 1000000; ++i ) {
        bhe::children( bb );
    }
    return b;
}
