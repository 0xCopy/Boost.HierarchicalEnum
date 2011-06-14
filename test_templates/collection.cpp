#include "header.hpp"

#include <boost/hierarchical_enum/{{plural}}.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

namespace bhe = boost::hierarchical_enum;

BOOST_AUTO_TEST_CASE( {{plural}} ) {
{% for nodeList in test.nodeLists %}
    BOOST_MPL_ASSERT(( boost::mpl::equal< bhe::{{plural}}< {% for node in nodeList.nodes %}{{node.name}}_c{% if not forloop.last %}, {% endif %}{% endfor %} >, boost::mpl::vector< {% for child in nodeList.collection %}{{child.name}}_c{% if not forloop.last %}, {% endif %}{% endfor %} > > ));

{% endfor %}
}
