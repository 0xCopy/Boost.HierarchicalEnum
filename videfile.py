from ViDE.Project.Description import *
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

for test in TestGenerator.tests:
    generatedSources = [ f for f in TestGenerator.generatedFiles if f.endswith( ".cpp" ) ]
    generatedHeaders = [ f for f in TestGenerator.generatedFiles if f.endswith( ".hpp" ) ]

    #for file in generatedHeaders:
        #GeneratedSource(
            #test = test,
            #file = file,
            #headers = []
        #)

    #for file in generatedSources:
        #GeneratedSource(
            #test = test,
            #file = file,
            #headers = generatedHeaders
        #)

    CppExecutable(
        name = "test_" + test.name,
        sources = [ TestGenerator.generatedFileName( test, f ) for f in generatedSources ],
        localLibraries = [ bhe ],
        externalLibraries = [ BoostUnitTestFramework ],
    )
