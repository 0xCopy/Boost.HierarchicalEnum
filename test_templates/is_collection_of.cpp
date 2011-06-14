#include "header.hpp"

#include <boost/hierarchical_enum/{{plural}}.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

namespace bhe = boost::hierarchical_enum;

BOOST_AUTO_TEST_CASE( is_{{singular}}_of ) {
{% for resultNode in test.nodes %}
    {% for nodeList in test.nodeLists %}
        {% if resultNode in nodeList.collection %}
    BOOST_MPL_ASSERT    (( bhe::is_{{singular}}_of< {{resultNode.name}}_c{% for node in nodeList.nodes %}, {{node.name}}_c{% endfor %} > ));
        {% else %}
    BOOST_MPL_ASSERT_NOT(( bhe::is_{{singular}}_of< {{resultNode.name}}_c{% for node in nodeList.nodes %}, {{node.name}}_c{% endfor %} > ));
        {% endif %}
    {% endfor %}
{% endfor %}
}
