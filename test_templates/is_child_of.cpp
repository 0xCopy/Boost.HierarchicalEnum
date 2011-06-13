#include "header.hpp"

#include <boost/hierarchical_enum/children.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

namespace bhe = boost::hierarchical_enum;

BOOST_AUTO_TEST_CASE( is_child_of ) {
{% for parent in test.nodes %}
    {% for child in test.nodes %}
        {% if child in parent.children %}
            BOOST_MPL_ASSERT(( bhe::is_child_of< {{child.name}}_c, {{parent.name}}_c > ));
        {% else %}
            BOOST_MPL_ASSERT_NOT(( bhe::is_child_of< {{child.name}}_c, {{parent.name}}_c > ));
        {% endif %}
    {% endfor %}
{% endfor %}
}
