#!/usr/bin/env python

import os

class Node:
    def __init__( self, name, value ):
        self.name = name
        self.value = value
        self.__father = None
        self.__children = set()

    def setFather( self, father ): self.__father = father
    def addChild( self, child ): self.__children.add( child )

    def father( self ): return self.__father
    def children( self ): return self.__children
    def ascendants( self ): return set( [ self ] ) | self.strictAscendants()
    def strictAscendants( self ): return set() if self.father() is None else self.father().ascendants()
    def descendants( self ): return set( [ self ] ) | self.strictDescendants()
    def strictDescendants( self ):
        d = set()
        for c in self.children():
            d.update( c.descendants() )
        return d

class Test:
    def __init__( self, name, nodes, namespace = None ):
        self.name = name
        self.nodes = sorted( [ Node( name, value ) for ( name, value ) in nodes.iteritems() ], key = lambda n : n.name )
        self.namespace = namespace
        self.initNodeChildren()

    def initNodeChildren( self ):
        for father in self.nodes:
            for son in self.nodes:
                if len( son.name ) == len( father.name ) + 1 and son.name.startswith( father.name ):
                    father.addChild( son );
                    son.setFather( father );

tests = [
    Test( "basic",
        {
            "A": None,
            "B": None,
            "AA": "42",
            "AB": None,
            "BA": None,
            "BB": "AA + 56",
            "BAA": None,
        }
    ),
    Test( "one_node",
        {
            "A": None,
        },
        namespace = "One::Node"
    ),
    Test( "long_branch",
        {
            "A": None,
            "AA": None,
            "AAA": None,
            "AAAA": None,
            "AAAAA": None,
            "AAAAAA": None,
            "AAAAAAA": None,
            "AAAAAAAA": None,
            "AAAAAAAAA": None,
            "AAAAAAAAAA": None,
        }
    ),
    Test( "wide_tree",
        {
            "A": None,
            "B": None,
            "C": None,
            "D": None,
            "E": None,
            "F": None,
            "G": None,
            "GA": None,
        }
    ),
]

def generatedFileName( test, file ):
    return "build/gen/test/%s/%s" % ( test.name, file )

def reallyOpen( f ):
    d = os.path.dirname( f )
    try:
        os.makedirs( d )
    except OSError:
        pass
    return open( f, "w" )

def genHeaderHpp( test ):
    f = reallyOpen( generatedFileName( test, "header.hpp" ) )
    f.write( "#ifndef test_%s_header_hpp\n" % test.name )
    f.write( "#define test_%s_header_hpp\n" % test.name )
    f.write( "\n" )
    f.write( "#include <boost/hierarchical_enum/hierarchical_enum.hpp>\n" )
    f.write( "\n" )

    if test.namespace is None:
        f.write( "BOOST_HIERARCHICAL_ENUM(\n" )
    else:
        f.write( "BOOST_HIERARCHICAL_ENUM_NS(\n" )
        f.write( "    (" + ")(".join( test.namespace.split( "::" ) ) + "),\n" )
    f.write( "    HierarchicalEnum,\n" )
    for father in test.nodes:
        f.write( "    (( %s, " % father.name )
        if father.value is None:
            f.write( "DEF()" )
        else:
            f.write( "VAL( %s )" % father.value )
        f.write( ", " )
        if len( father.children() ) == 0:
            f.write( "BOOST_PP_SEQ_NIL" )
        else:
            f.write( "(" + ")(".join( [ child.name for child in father.children() ] ) + ")" )
        f.write( " ))\n" )
    f.write( ");\n" )

    f.write( "\n" )
    f.write( "#endif // Include guard\n" )

def genMainCpp( test ):
    f = reallyOpen( generatedFileName( test, "main.cpp" ) )
    f.write( "#define BOOST_TEST_DYN_LINK\n" )
    f.write( "#define BOOST_TEST_MAIN\n" )
    f.write( "#include <boost/test/unit_test.hpp>\n" )

def openAndWriteTestHeader( test, functionName, header ):
    f = reallyOpen( generatedFileName( test, functionName + ".cpp" ) )
    f.write( "#include \"header.hpp\"\n" )
    f.write( "\n" )
    if header:
        f.write( "#include <boost/hierarchical_enum/%s.hpp>\n" % header )
        f.write( "\n" )
    f.write( "#include <boost/mpl/equal_to.hpp>\n" )
    f.write( "#include <boost/mpl/equal.hpp>\n" )
    f.write( "#include <boost/mpl/assert.hpp>\n" )
    f.write( "#include <boost/test/unit_test.hpp>\n" )
    f.write( "#include <boost/assign/list_of.hpp>\n" )
    f.write( "\n" )
    f.write( "namespace bhe = boost::hierarchical_enum;\n" )
    ns = ""
    if test.namespace:
        ns = "tst::"
        f.write( "namespace tst = %s;\n" % test.namespace )
    f.write( "\n" )
    f.write( "BOOST_AUTO_TEST_CASE( %s ) {\n" % functionName )
    return f, ns

def writeTestFooter( f ):
    f.write( "}\n" )

def genFatherCpp( test ):
    f, ns = openAndWriteTestHeader( test, "father", None )

    for node in test.nodes:
        if node.father():
            f.write( "    BOOST_MPL_ASSERT(( boost::mpl::equal_to< bhe::mp::father< %s%s_c >, %s%s_c > ));\n" % ( ns, node.name, ns, node.father().name ) )
            f.write( "    BOOST_CHECK_EQUAL( bhe::father( %s%s ), %s%s );\n" % ( ns, node.name, ns, node.father().name ) )
        else:
            f.write( "    BOOST_CHECK_THROW( bhe::father( %s%s ), int );\n" % ( ns, node.name ) )

    writeTestFooter( f )

def genUnaryCollection( test, functionName, collection, header ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for node in test.nodes:
        elements = sorted( collection( node ), key = lambda n : n.name )
        f.write( "    BOOST_MPL_ASSERT(( boost::mpl::equal< bhe::mp::%s< %s%s_c >, boost::mpl::vector< " % ( functionName, ns, node.name ) )
        f.write( ", ".join( [ ns + e.name + "_c" for e in elements ] ) )
        if elements: f.write( " " )
        f.write( "> > ));\n" )
        if len( elements ) != 0:
            f.write( "    BOOST_CHECK( bhe::%s( %s%s ) == boost::assign::list_of(" % ( functionName, ns, node.name ) )
            f.write( ")(".join( [ ns + e.name for e in elements ] ) )
            f.write( ") );\n" )
        else:
            f.write( "    BOOST_CHECK( bhe::%s( %s%s ).empty() );\n" % ( functionName, ns, node.name ) )

    writeTestFooter( f )

def genBinaryCollection( test, functionName, collection, header ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for n1 in test.nodes:
        for n2 in test.nodes:
            elements = sorted( collection( n1 ) & collection( n2 ), key = lambda n : n.name )
            f.write( "    BOOST_MPL_ASSERT(( boost::mpl::equal< bhe::mp::%s< %s%s_c, %s%s_c >, boost::mpl::vector< " % ( functionName, ns, n1.name, ns, n2.name ) )
            f.write( ", ".join( [ ns + e.name + "_c" for e in elements ] ) )
            if elements: f.write( " " )
            f.write( "> > ));\n" )
            if len( elements ) != 0:
                f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ) == boost::assign::list_of(" % ( functionName, ns, n1.name, ns, n2.name ) )
                f.write( ")(".join( [ ns + e.name for e in elements ] ) )
                f.write( ") );\n" )
            else:
                f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ).empty() );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
        f.write( "\n" )

    writeTestFooter( f )

def genNonEmptyUnaryPredicate( test, functionName, collection, header ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for node in test.nodes:
        if collection( node ):
            f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c > ));\n" % ( functionName, ns, node.name ) )
            f.write( "    BOOST_CHECK( bhe::%s( %s%s ) );\n" % ( functionName, ns, node.name ) )
        else:
            f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c > ));\n" % ( functionName, ns, node.name ) )
            f.write( "    BOOST_CHECK( !bhe::%s( %s%s ) );\n" % ( functionName, ns, node.name ) )

    writeTestFooter( f )

def genNonEmptyBinaryPredicate( test, functionName, collection, header ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for n1 in test.nodes:
        for n2 in test.nodes:
            if collection( n1 ) & collection( n2 ):
                f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
            else:
                f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
        f.write( "\n" )

    writeTestFooter( f )

def genMembershipBinaryPredicate( test, functionName, collection, header = None ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for n1 in test.nodes:
        for n2 in test.nodes:
            if n1 in collection( n2 ):
                f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
            else:
                f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
        f.write( "\n" )

    writeTestFooter( f )

def genMembershipTernaryPredicate( test, functionName, collection, header ):
    f, ns = openAndWriteTestHeader( test, functionName, header )

    for n1 in test.nodes:
        for n2 in test.nodes:
            for n3 in test.nodes:
                if n1 in collection( n2 ) & collection( n3 ):
                    f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                    f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                else:
                    f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                    f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
            f.write( "\n" )

    writeTestFooter( f )

def fatherAsSet( n ):
    if n.father() is None:
        return set()
    else:
        return set( [ n.father() ] )

generatedFiles = {
    "header.hpp": genHeaderHpp,
    "main.cpp": genMainCpp,

    "has_father.cpp": lambda test: genNonEmptyUnaryPredicate( test, "has_father", fatherAsSet, None ),
    "father.cpp": genFatherCpp,
    "is_father_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_father_of", fatherAsSet, None ),
    "strict_ascendants.cpp": lambda test: genUnaryCollection( test, "strict_ascendants", Node.strictAscendants, "ascendants" ),
    "is_strict_ascendant_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_strict_ascendant_of", Node.strictAscendants, "ascendants" ),
    "ascendants.cpp": lambda test: genUnaryCollection( test, "ascendants", Node.ascendants, "ascendants" ),
    "is_ascendant_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_ascendant_of", Node.ascendants, "ascendants" ),

    "have_common_ascendant.cpp": lambda test: genNonEmptyBinaryPredicate( test, "have_common_ascendant", Node.ascendants, "common_ascendants" ),
    "common_ascendants.cpp": lambda test: genBinaryCollection( test, "common_ascendants", Node.ascendants, "common_ascendants" ),
    "is_common_ascendant_of.cpp": lambda test: genMembershipTernaryPredicate( test, "is_common_ascendant_of", Node.ascendants, "common_ascendants" ),
    "have_common_strict_ascendant.cpp": lambda test: genNonEmptyBinaryPredicate( test, "have_common_strict_ascendant", Node.strictAscendants, "common_ascendants" ),
    "common_strict_ascendants.cpp": lambda test: genBinaryCollection( test, "common_strict_ascendants", Node.strictAscendants, "common_ascendants" ),
    "is_common_strict_ascendant_of.cpp": lambda test: genMembershipTernaryPredicate( test, "is_common_strict_ascendant_of", Node.strictAscendants, "common_ascendants" ),

    "has_child.cpp": lambda test: genNonEmptyUnaryPredicate( test, "has_child", Node.children, "children" ),
    "children.cpp": lambda test: genUnaryCollection( test, "children", Node.children, "children" ),
    "is_child_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_child_of", Node.children, "children" ),
    "strict_descendants.cpp": lambda test: genUnaryCollection( test, "strict_descendants", Node.strictDescendants, "descendants" ),
    "is_strict_descendant_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_strict_descendant_of", Node.strictDescendants, "descendants" ),
    "descendants.cpp": lambda test: genUnaryCollection( test, "descendants", Node.descendants, "descendants" ),
    "is_descendant_of.cpp": lambda test: genMembershipBinaryPredicate( test, "is_descendant_of", Node.descendants, "descendants" ),
}

if __name__ == "__main__":
    for test in tests:
        for file in generatedFiles:
            generatedFiles[ file ]( test )
    exit()
