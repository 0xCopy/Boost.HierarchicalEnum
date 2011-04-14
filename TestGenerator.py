#!/usr/bin/env python

import os
import sys

class Node:
    def __init__( self, name, value ):
        self.name = name
        self.value = value
        self.__father = None
        self.__children = set()

    def setFather( self, father ): self.__father = father
    def addChild( self, child ): self.__children.add( child )

    def father( self ):
        if self.__father is None:
            return set()
        else:
            return set( [ self.__father ] )
    def children( self ): return self.__children
    def ascendants( self ): return set( [ self ] ) | self.strictAscendants()
    def strictAscendants( self ): return set() if self.__father is None else self.__father.ascendants()
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

class TestsGenerator:
    def __init__( self, path ):
        self.__path = path
        self.__tests = [
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


    def generate( self ):
        for test in self.__tests:
            TestGenerator( self.__path, test ).generate()

class TestGenerator:
    def __init__( self, path, test ):
        self.__path = path
        self.__test = test
    
    def generate( self ):
        self.__genHeaderHpp()
        self.__genMainCpp()

        self.__genNonEmptyUnaryPredicate( "has_father", Node.father, None )
        self.__genFatherCpp()
        self.__genMembershipBinaryPredicate( "is_father_of", Node.father, None )
        self.__genUnaryCollection( "strict_ascendants", Node.strictAscendants, "ascendants" )
        self.__genMembershipBinaryPredicate( "is_strict_ascendant_of", Node.strictAscendants, "ascendants" )
        self.__genUnaryCollection( "ascendants", Node.ascendants, "ascendants" )
        self.__genMembershipBinaryPredicate( "is_ascendant_of", Node.ascendants, "ascendants" )

        self.__genNonEmptyBinaryPredicate( "have_common_ascendant", Node.ascendants, "common_ascendants" )
        self.__genBinaryCollection( "common_ascendants", Node.ascendants, "common_ascendants" )
        self.__genMembershipTernaryPredicate( "is_common_ascendant_of", Node.ascendants, "common_ascendants" )
        self.__genNonEmptyBinaryPredicate( "have_common_strict_ascendant", Node.strictAscendants, "common_ascendants" )
        self.__genBinaryCollection( "common_strict_ascendants", Node.strictAscendants, "common_ascendants" )
        self.__genMembershipTernaryPredicate( "is_common_strict_ascendant_of", Node.strictAscendants, "common_ascendants" )

        self.__genNonEmptyUnaryPredicate( "has_child", Node.children, "children" )
        self.__genUnaryCollection( "children", Node.children, "children" )
        self.__genMembershipBinaryPredicate( "is_child_of", Node.children, "children" )
        self.__genUnaryCollection( "strict_descendants", Node.strictDescendants, "descendants" )
        self.__genMembershipBinaryPredicate( "is_strict_descendant_of", Node.strictDescendants, "descendants" )
        self.__genUnaryCollection( "descendants", Node.descendants, "descendants" )
        self.__genMembershipBinaryPredicate( "is_descendant_of", Node.descendants, "descendants" )
    
    def __genHeaderHpp( self ):
        f = self.__reallyOpen( self.__generatedFileName( "header.hpp" ) )
        f.write( "#ifndef test_%s_header_hpp\n" % self.__test.name )
        f.write( "#define test_%s_header_hpp\n" % self.__test.name )
        f.write( "\n" )
        f.write( "#include <boost/hierarchical_enum/hierarchical_enum.hpp>\n" )
        f.write( "\n" )

        if self.__test.namespace is None:
            f.write( "BOOST_HIERARCHICAL_ENUM(\n" )
        else:
            f.write( "BOOST_HIERARCHICAL_ENUM_NS(\n" )
            f.write( "    (" + ")(".join( self.__test.namespace.split( "::" ) ) + "),\n" )
        f.write( "    HierarchicalEnum,\n" )
        for father in self.__test.nodes:
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

    def __genMainCpp( self ):
        f = self.__reallyOpen( self.__generatedFileName( "main.cpp" ) )
        f.write( "#define BOOST_TEST_DYN_LINK\n" )
        f.write( "#define BOOST_TEST_MAIN\n" )
        f.write( "#include <boost/test/unit_test.hpp>\n" )

    def __openAndWriteTestHeader( self, functionName, header ):
        f = self.__reallyOpen( self.__generatedFileName( functionName + ".cpp" ) )
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
        if self.__test.namespace:
            ns = "tst::"
            f.write( "namespace tst = %s;\n" % self.__test.namespace )
        f.write( "\n" )
        f.write( "BOOST_AUTO_TEST_CASE( %s ) {\n" % functionName )
        return f, ns

    def __writeTestFooter( self, f ):
        f.write( "}\n" )

    def __genFatherCpp( self ):
        f, ns = self.__openAndWriteTestHeader( "father", None )

        for node in self.__test.nodes:
            if node.father():
                f.write( "    BOOST_MPL_ASSERT(( boost::mpl::equal_to< bhe::mp::father< %s%s_c >, %s%s_c > ));\n" % ( ns, node.name, ns, node.father().pop().name ) )
                f.write( "    BOOST_CHECK_EQUAL( bhe::father( %s%s ), %s%s );\n" % ( ns, node.name, ns, node.father().pop().name ) )
            else:
                f.write( "    BOOST_CHECK_THROW( bhe::father( %s%s ), int );\n" % ( ns, node.name ) )

        self.__writeTestFooter( f )

    def __genUnaryCollection( self, functionName, collection, header ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for node in self.__test.nodes:
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

        self.__writeTestFooter( f )

    def __genBinaryCollection( self, functionName, collection, header ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for n1 in self.__test.nodes:
            for n2 in self.__test.nodes:
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

        self.__writeTestFooter( f )

    def __genNonEmptyUnaryPredicate( self, functionName, collection, header ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for node in self.__test.nodes:
            if collection( node ):
                f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c > ));\n" % ( functionName, ns, node.name ) )
                f.write( "    BOOST_CHECK( bhe::%s( %s%s ) );\n" % ( functionName, ns, node.name ) )
            else:
                f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c > ));\n" % ( functionName, ns, node.name ) )
                f.write( "    BOOST_CHECK( !bhe::%s( %s%s ) );\n" % ( functionName, ns, node.name ) )

        self.__writeTestFooter( f )

    def __genNonEmptyBinaryPredicate( self, functionName, collection, header ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for n1 in self.__test.nodes:
            for n2 in self.__test.nodes:
                if collection( n1 ) & collection( n2 ):
                    f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                    f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                else:
                    f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                    f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
            f.write( "\n" )

        self.__writeTestFooter( f )

    def __genMembershipBinaryPredicate( self, functionName, collection, header = None ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for n1 in self.__test.nodes:
            for n2 in self.__test.nodes:
                if n1 in collection( n2 ):
                    f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                    f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                else:
                    f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name ) )
                    f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name ) )
            f.write( "\n" )

        self.__writeTestFooter( f )

    def __genMembershipTernaryPredicate( self, functionName, collection, header ):
        f, ns = self.__openAndWriteTestHeader( functionName, header )

        for n1 in self.__test.nodes:
            for n2 in self.__test.nodes:
                for n3 in self.__test.nodes:
                    if n1 in collection( n2 ) & collection( n3 ):
                        f.write( "    BOOST_MPL_ASSERT    (( bhe::mp::%s< %s%s_c, %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                        f.write( "    BOOST_CHECK( bhe::%s( %s%s, %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                    else:
                        f.write( "    BOOST_MPL_ASSERT_NOT(( bhe::mp::%s< %s%s_c, %s%s_c, %s%s_c > ));\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                        f.write( "    BOOST_CHECK( !bhe::%s( %s%s, %s%s, %s%s ) );\n" % ( functionName, ns, n1.name, ns, n2.name, ns, n3.name ) )
                f.write( "\n" )

        self.__writeTestFooter( f )

    def __generatedFileName( self, file ):
        return os.path.join( self.__path, "gen", "test", self.__test.name, file )

    def __reallyOpen( self, f ):
        d = os.path.dirname( f )
        try:
            os.makedirs( d )
        except OSError:
            pass
        return open( f, "w" )

if __name__ == "__main__":
    path = ""
    if len( sys.argv ) > 1:
        path = sys.argv[ 1 ]
    TestsGenerator( path ).generate()
