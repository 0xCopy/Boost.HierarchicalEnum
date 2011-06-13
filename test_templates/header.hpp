#ifndef test_{{test.name}}_header_hpp
#define test_{{test.name}}_header_hpp

#include <boost/hierarchical_enum/hierarchical_enum.hpp>

BOOST_HIERARCHICAL_ENUM(
    {{test.name}},
    {% for n in test.nodes %}({{n.name}}){% endfor %},
    {% for f in test.filiations %}(({{f.parent}}, {{f.child}})){% endfor %}
)

#endif // Include guard
