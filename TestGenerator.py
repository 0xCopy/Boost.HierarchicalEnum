#!/usr/bin/env python

import os
import sys

import django.template.loader
import django.conf
django.conf.settings.configure( TEMPLATE_DIRS = ( "test_templates", ) )

class Node:
    def __init__( self, name, value = None ):
        self.name = name
        self.value = value
        self.__parents = set()
        self.__children = set()

    def addParent( self, parent ): self.__parents.add( parent )
    def addChild( self, child ): self.__children.add( child )

    def parents( self ): return sorted( self.__parents, key = lambda node: node.name )
    def children( self ): return sorted( self.__children, key = lambda node: node.name )
    def ascendants( self ): return sorted( set( [ self ] ) | self.strictAscendants(), key = lambda node: node.name )
    def descendants( self ): return sorted( set( [ self ] ) | self.strictDescendants(), key = lambda node: node.name )
    def strictAscendants( self ):
        a = set()
        for p in self.parents():
            a.update( p.ascendants() )
        return sorted( a, key = lambda node: node.name )
    def strictDescendants( self ):
        d = set()
        for c in self.children():
            d.update( c.descendants() )
        return sorted( d, key = lambda node: node.name )

class Test:
    def __init__( self, name, nodes, filiations ):
        self.name = name
        self.__filiations = filiations
        self.__nodes = { n.name: n for n in nodes }
        self.initNodeChildren( filiations )

    def initNodeChildren( self, filiations ):
        for ( parent, child ) in filiations:
            self.__nodes[parent].addChild( self.__nodes[child] );
            self.__nodes[child].addParent( self.__nodes[parent] );

    def filiations( self ):
        return [ { "parent": a, "child": b } for ( a, b ) in self.__filiations ]
        
    def nodes( self ):
        return [ self.__nodes[ name ] for name in sorted( self.__nodes ) ]

class TestGenerator:
    files = [
        "main.cpp",
        "is_child_of.cpp",
        "children.cpp"
    ]
    
    def __init__( self, path, test ):
        self.__path = path
        self.__test = test
    
    def generate( self ):
        for f in [ "header.hpp" ] + TestGenerator.files:
            self.__open( f ).write( django.template.loader.render_to_string( f, self.__djangoContext() ) )

    def __djangoContext( self ):
        return {
            "test": test
        }

    def __genMainCpp( self ):
        f = self.__open( "main.cpp" )
        f.write( "#define BOOST_TEST_DYN_LINK\n" )
        f.write( "#define BOOST_TEST_MAIN\n" )
        f.write( "#include <boost/test/unit_test.hpp>\n" )

    def __open( self, f ):
        f = self.__computeFileName( f )
        d = os.path.dirname( f )
        try:
            os.makedirs( d )
        except OSError:
            pass
        return open( f, "w" )

    def __computeFileName( self, file ):
        return os.path.join( self.__path, self.__test.name, file )

tests = [
    Test( "OneNode", [ Node( "A" ) ], [] ),
    Test( "TwoNode", [ Node( "A" ), Node( "B" ) ], [ ( "A", "B" ) ] ),
    Test( "ThreeNode", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "B" ), ( "B", "C" ) ] ),
    Test( "TwoChildren", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "B" ), ( "A", "C" ) ] ),
    Test( "TwoParents", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "C" ), ( "B", "C" ) ] ),
    Test( "Diamond",
        [
            Node( "A" ),
            Node( "B" ),
            Node( "C" ),
            Node( "D" ),
        ],
        [
            ( "A", "B" ),
            ( "A", "C" ),
            ( "B", "D" ),
            ( "C", "D" ),
        ]
    ),
]

files = TestGenerator.files

if __name__ == "__main__":
    path = ""
    if len( sys.argv ) > 1:
        path = sys.argv[ 1 ]
    for test in tests:
        TestGenerator( path, test ).generate()

