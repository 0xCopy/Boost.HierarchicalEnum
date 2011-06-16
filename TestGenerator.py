#!/usr/bin/env python

import os
import sys
import itertools

import django.template.loader
import django.conf
django.conf.settings.configure( TEMPLATE_DIRS = ( "test_templates", ) )

class Node:
    def __init__( self, name, value = None ):
        self.name = name
        self.value = value
        self.__parents = set()
        self.__children = set()
        
    @staticmethod
    def sort( nodes ):
        return sorted( nodes, key = lambda node: node.name )

    def addParent( self, parent ): self.__parents.add( parent )
    def addChild( self, child ): self.__children.add( child )

    def parents( self ): return self.__parents
    def children( self ): return self.__children
    def ascendants( self ): return set( [ self ] ) | self.strictAscendants()
    def descendants( self ): return set( [ self ] ) | self.strictDescendants()
    def strictAscendants( self ):
        a = set()
        for p in self.parents():
            a.update( p.ascendants() )
        return a
    def strictDescendants( self ):
        d = set()
        for c in self.children():
            d.update( c.descendants() )
        return d

class NodeList:
    currentCollection = Node.children

    def __init__( self, nodes ):
        self.__nodes = nodes
        
    def collection( self ):
        c = set( NodeList.currentCollection( self.__nodes[ 0 ] ) )
        for node in self.__nodes:
            c.intersection_update( NodeList.currentCollection( node ) )
        return Node.sort( c )
        
    def nodes( self ):
        return self.__nodes
        
class Test:
    def __init__( self, name, nodes, filiations ):
        self.name = name
        self.__filiations = filiations
        self.__nodes = Node.sort( nodes )
        self.__initNodeChildren( filiations )

    def __initNodeChildren( self, filiations ):
        nodeDict = dict( [ ( n.name, n ) for n in self.__nodes ] )
        for ( parent, child ) in filiations:
            nodeDict[parent].addChild( nodeDict[child] );
            nodeDict[child].addParent( nodeDict[parent] );

    def filiations( self ):
        return [ { "parent": a, "child": b } for ( a, b ) in self.__filiations ]
        
    def nodes( self ):
        return self.__nodes
        
    def nodeLists( self ):
        sets = []
        #for i in range( len( self.__nodes ) ):
        for i in [ 0, 1 ]:
            sets += [ NodeList( nodes ) for nodes in itertools.product( self.__nodes, repeat = i + 1 ) ]
        return sets

class TestGenerator:
    __staticCppFiles = [ "main.cpp" ]
    __staticHppFiles = [ "header.hpp" ]
    __collections = {
        "child" : ( "children", Node.children ),
        "parent" : ( "parents", Node.parents ),
        #"ascendant" : ( "ascendants", Node.ascendants ),
        #"descendant" : ( "descendant", Node.descendants ),
        #"strict_ascendant" : ( "strict_ascendants", Node.strictAscendants ),
        #"strict_descendant" : ( "strict_descendants", Node.strictDescendants ),
    }

    @staticmethod
    def cppFiles():
        files = list( TestGenerator.__staticCppFiles )
        for singular, ( plural, collection ) in TestGenerator.__collections.items():
            files.append( "is_%s_of.cpp" % singular )
            files.append( "%s.cpp" % plural )
            files.append( "nb_%s.cpp" % plural )
        return files

    def __init__( self, path, test ):
        self.__path = path
        self.__test = test

    def generate( self ):
        for f in TestGenerator.__staticHppFiles + TestGenerator.__staticCppFiles:
            #print test.name, f
            self.__render( f, f, { "test" : self.__test } )
        for singular, ( plural, collection ) in TestGenerator.__collections.items():
            NodeList.currentCollection = collection
            self.__render( "is_%s_of.cpp" % singular, "is_collection_of.cpp", { "test" : self.__test, "singular" : singular, "plural" : plural } )
            self.__render( "%s.cpp" % plural, "collection.cpp", { "test" : self.__test, "singular" : singular, "plural" : plural } )
            self.__render( "nb_%s.cpp" % plural, "nb_collection.cpp", { "test" : self.__test, "singular" : singular, "plural" : plural } )

    def __render( self, file, template, context ):
        self.__open( file ).write( self.__removeMultipleBlankLines( 
            django.template.loader.render_to_string(
                template,
                context
            )
        ) )
        
    def __removeMultipleBlankLines( self, text ):
        newText = ""
        emptyLines = 0
        for line in text.split( "\n" ):
            if line.strip() == "":
                emptyLines += 1
            else:
                if emptyLines == 1:
                    newText += "\n"
                newText += line + "\n"
                emptyLines = 0
        return newText

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
    Test( "TwoNodes", [ Node( "A" ), Node( "B" ) ], [ ( "A", "B" ) ] ),
    Test( "ThreeNodes", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "B" ), ( "B", "C" ) ] ),
    Test( "TwoChildren", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "B" ), ( "A", "C" ) ] ),
    Test( "TwoParents", [ Node( "A" ), Node( "B" ), Node( "C" ) ], [ ( "A", "C" ), ( "B", "C" ) ] ),
    Test( "Diamond",
        [ Node( "A" ), Node( "B" ), Node( "C" ), Node( "D" ), ],
        [ ( "A", "B" ), ( "A", "C" ), ( "B", "D" ), ( "C", "D" ), ]
    ),
]

cppFiles = TestGenerator.cppFiles()

if __name__ == "__main__":
    path = ""
    if len( sys.argv ) > 1:
        path = sys.argv[ 1 ]
    for test in tests:
        TestGenerator( path, test ).generate()
