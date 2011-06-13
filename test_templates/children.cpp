#include "header.hpp"

#include <boost/hierarchical_enum/children.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

namespace bhe = boost::hierarchical_enum;

BOOST_AUTO_TEST_CASE( children ) {
{% for parent in test.nodes %}
    BOOST_MPL_ASSERT(( boost::mpl::equal< bhe::children< {{parent.name}}_c >, boost::mpl::vector< {%for child in parent.children %}{{child.name}}_c{% if not forloop.last %}, {% endif %}{% endfor %} > > ));
{% endfor %}
}
