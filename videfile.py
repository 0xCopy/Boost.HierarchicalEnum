from ViDE.Project.Description import *
from ViDE.Project.Artifacts.BasicArtifacts import AtomicArtifact, SubatomicArtifact, MonofileInputArtifact
from ViDE.Core.Actions import SystemAction
from Tools.Boost import BoostUnitTestFramework

import TestGenerator

bhe = CppHeaderLibrary( "bhe",
    headers = AllHppIn( "src/boost" ),
    stripHeaders = lambda h: h[4:]
)

CppExecutable(
    name = "example_no_ns",
    sources = [ "src/example/no_ns.cpp" ],
    localLibraries = [ bhe ]
)

CppExecutable(
    name = "example_ns",
    sources = [ "src/example/ns.cpp" ],
    localLibraries = [ bhe ]
)

class GeneratorScript( MonofileInputArtifact ):
    pass

class GeneratedTestSources( AtomicArtifact ):
    def __init__( self, context, generatorScript, explicit ):
        self.__context = context # Needed because self.context is initialized in AtomicArtifact.__init__, but the context is used in self.__getAllFiles
        AtomicArtifact.__init__(
            self,
            context = context,
            name = "tests",
            files = self.__getAllFiles(),
            strongDependencies = [ generatorScript ],
            orderOnlyDependencies = [],
            automaticDependencies = [],
            explicit = explicit
        )

    def getSources( self, test ):
        return self.getCached( "sources", lambda test: [ Project.inProgress.createArtifact( GeneratedSource, f, self, False ) for f in self.__getFiles( test ) ], test )

    def __getAllFiles( self ):
        files = []
        for test in TestGenerator.tests:
            files += self.__getFiles( test )
        return files

    def __getFiles( self, test ):
        return [ self.__context.buildkit.fileName( "gen", test.name, f ) for f in TestGenerator.files ]
        
    def doGetProductionAction( self ):
        return SystemAction(
            [ "python", "TestGenerator.py" ],
            [ self.__context.buildkit.fileName( "gen" ) ]
        )

class GeneratedSource( SubatomicArtifact ):
    def __init__( self, context, fileName, atomicArtifact, explicit ):
        SubatomicArtifact.__init__(
            self,
            context = context,
            name = fileName,
            atomicArtifact = atomicArtifact,
            files = [ fileName ],
            explicit = explicit
        )
        self.__fileName = fileName

    def getFileName( self ):
        return self.__fileName

generatorScript = Project.inProgress.createArtifact( GeneratorScript, "TestGenerator.py", True )

generatedTestSources = Project.inProgress.createArtifact( GeneratedTestSources, generatorScript, True )

for test in TestGenerator.tests:
    CppExecutable(
        name = "test_" + test.name,
        sources = generatedTestSources.getSources( test ),
        localLibraries = [ bhe ],
        externalLibraries = [ BoostUnitTestFramework ],
    )
