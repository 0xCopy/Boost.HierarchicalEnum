from ViDE.Project.Description import *

import glob
import os.path

import TestGenerator

from ViDE.Project.Description import *
from ViDE.Project.Artefact import AtomicArtefact
from ViDE.Project.Action import PythonAction
from ViDE.Project.RealArtefacts import realHeaders

class GeneratedSource( AtomicArtefact ):
    def __init__( self, project, test, file, headers ):
        self.__sourceFileName = TestGenerator.generatedFileName( test, file )
        AtomicArtefact.__init__( self, project, name = self.__sourceFileName,
            files = [ self.__sourceFileName ],
            dependencies = [],
            orderOnlyDependencies = headers
        )
        self.__test = test
        self.__file = file

    def doGetProductionAction( self, predecessors ):
        return PythonAction(
            lambda : TestGenerator.generatedFiles[ self.__file ]( self.__test ),
            dump = "Generate " + self.__sourceFileName,
            predecessors = predecessors,
        )

class GeneratedHeader( GeneratedSource ):
    pass

class GeneratedSourceDescription:
    def __init__( self, test, file, headers = [] ):
        self.__test = test
        self.__file = file
        self.__headers = headers

    def create( self, project ):
        cls = GeneratedSource
        if self.__file.endswith( ".hpp" ):
            cls = GeneratedHeader
        return cls(
            project,
            self.__test,
            self.__file,
            realHeaders( project, self.__headers )
        )

def testArtefacts():
    return [
        GeneratedSourceDescription( test, file )
        for file in TestGenerator.generatedFiles if file.endswith( ".hpp" )
        for test in TestGenerator.tests
    ] + [
        GeneratedSourceDescription( test, file, headers = [ TestGenerator.generatedFileName( test, f ) for f in TestGenerator.generatedFiles if f.endswith( ".hpp" ) ] )
        for file in TestGenerator.generatedFiles if file.endswith( ".cpp" )
        for test in TestGenerator.tests
    ] + [
        Executable( "test_" + test.name,
            sources = [ TestGenerator.generatedFileName( test, file ) for file in TestGenerator.generatedFiles if file.endswith( ".cpp" ) ],
            localLibraries = [ "bhe" ],
            externalLibraries = [ "boost_unit_test_framework" ],
        )
        for test in TestGenerator.tests
    ]

project = Project(
    artefacts = [
        HeaderLibrary( "bhe",
            headers = AllHppIn( "src/boost" )
        ),
        Executable( "example_no_ns",
            sources = [ "src/example/no_ns.cpp" ],
            localLibraries = [ "bhe" ],
            externalLibraries = [ "boost_unit_test_framework" ],
        ),
        Executable( "example_ns",
            sources = [ "src/example/ns.cpp" ],
            localLibraries = [ "bhe" ],
            externalLibraries = [ "boost_unit_test_framework" ],
        )
    ]
    +
    testArtefacts()
)
