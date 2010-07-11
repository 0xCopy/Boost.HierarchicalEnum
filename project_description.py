from ViDE.Project.Description import *

import glob
import os.path

import TestGenerator

from ViDE.Project.Artefact import AtomicArtefact
from ViDE.Project.Action import PythonAction

class GeneratedSource( AtomicArtefact ):
    def __init__( self, test, file, headers ):
        self.__sourceFileName = TestGenerator.generatedFileName( test, file )

        realHeaders = []
        for h in headers:
            realHeaders.append( GeneratedSource.getOrCreate( test, h, [] ) )

        AtomicArtefact.__init__( self, name = self.__sourceFileName,
            files = [ self.__sourceFileName ],
            dependencies = [],
            orderOnlyDependencies = realHeaders,
            automatic = False
        )
        self.__test = test
        self.__file = file

    @staticmethod
    def computeName( test, file, headers ):
        return TestGenerator.generatedFileName( test, file )

    def doGetProductionAction( self, predecessors ):
        return PythonAction(
            lambda : TestGenerator.generatedFiles[ self.__file ]( self.__test ),
            dump = "Generate " + TestGenerator.generatedFileName( self.__test, self.__file ),
            predecessors = predecessors,
        )

    def fileName( self ):
        return TestGenerator.generatedFileName( self.__test, self.__file )

###############################################################################

HeaderLibrary( "bhe",
    publicHeaders = AllHppIn( "src/boost" ),
    stripFromHeaders = "src/",
    localLibraries = []
)

Executable(
    name = "example_no_ns",
    sources = [ "src/example/no_ns.cpp" ],
    localLibraries = [ "bhe" ],
    externalLibraries = [ "boost_unit_test_framework" ],
)

Executable(
    name = "example_ns",
    sources = [ "src/example/ns.cpp" ],
    localLibraries = [ "bhe" ],
    externalLibraries = [ "boost_unit_test_framework" ],
)

for test in TestGenerator.tests:
    generatedSources = [ f for f in TestGenerator.generatedFiles if f.endswith( ".cpp" ) ]
    generatedHeaders = [ f for f in TestGenerator.generatedFiles if f.endswith( ".hpp" ) ]

    for file in generatedHeaders:
        GeneratedSource(
            test = test,
            file = file,
            headers = []
        )

    for file in generatedSources:
        GeneratedSource(
            test = test,
            file = file,
            headers = generatedHeaders
        )

    Executable(
        name = "test_" + test.name,
        sources = [ TestGenerator.generatedFileName( test, f ) for f in generatedSources ],
        localLibraries = [ "bhe" ],
        externalLibraries = [ "boost_unit_test_framework" ],
    )
