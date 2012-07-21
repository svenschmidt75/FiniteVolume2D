#include <iostream>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>


#include "ComputationalMeshSolverHelperTest.h"
#include "ASCIIMeshReaderTest.h"
#include "MeshConnectivityTest.h"
#include "EntityTest.h"
#include "MeshBoundaryConditionReaderTest.h"
#include "ComputationalMeshBuilderTest.h"
#include "ComputationalVariableTest.h"
#include "ComputationalVariableManagerTest.h"
#include "MeshCheckerTest.h"
#include "VersteegMalalasekeraTest.h"
#include "VersteegMalalasekeraMeshDistortedTest.h"
#include "GeometricHelperTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ComputationalMeshSolverHelperTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ASCIIMeshReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MeshConnectivityTest);
CPPUNIT_TEST_SUITE_REGISTRATION(EntityTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MeshBoundaryConditionReaderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ComputationalMeshBuilderTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ComputationalVariableTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ComputationalVariableManagerTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MeshCheckerTest);
CPPUNIT_TEST_SUITE_REGISTRATION(VersteegMalalasekeraTest);
CPPUNIT_TEST_SUITE_REGISTRATION(VersteegMalalasekeraMeshDistortedTest);
CPPUNIT_TEST_SUITE_REGISTRATION(GeometricHelperTest);


int main(int /*argc*/, char ** /*argv*/) {
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
