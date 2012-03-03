#include "GridConectivityTest.h"


// CPPUNIT_ASSERT_EQUAL_MESSAGE("Convex polytope not closed, maybe face orientation wrong?", true, check);
// CPPUNIT_ASSERT_MESSAGE("Min x of polytope wrong", Utility::isAlmostEqual(-1.0, box.minX()));
// #define CPPUNIT_ASSERT_DOUBLES_EQUAL(expected,actual,delta)        \
// #define CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message,expected,actual,delta)  \

void
GridConectivityTest::setUp() {
    // read test grid file
//     std::string mesh_filename = "UnitTest/Data/simple_mesh.mesh";
//     MeshBuilder mesh_builder;
//     ASCIIMeshReader reader(mesh_filename, mesh_builder);
//     reader.read();
// 
//     Mesh const & mesh = mesh_builder.getMesh();


}

void
GridConectivityTest::tearDown() {
}

void
GridConectivityTest::testInit() {
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Error", true, false);
}
