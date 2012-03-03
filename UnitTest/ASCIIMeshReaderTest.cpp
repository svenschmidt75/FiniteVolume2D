#include "ASCIIMeshReaderTest.h"

#include "FiniteVolume2DLib/ASCIIMeshReader.h"

#include <boost/filesystem.hpp>

namespace FS = boost::filesystem;


void
ASCIIMeshReaderTest::setUp() {
    mesh_filename_ = "Data\\grid.mesh";

    initMesh();
}

void
ASCIIMeshReaderTest::tearDown() {
}

void
ASCIIMeshReaderTest::testMeshFileExists() {
    bool exists = FS::exists(mesh_filename_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Mesh file not found!", true, exists);
}

void
ASCIIMeshReaderTest::testNumberOfVertices() {
}

void
ASCIIMeshReaderTest::initMesh() {
    static bool init = false;
    if (!init)
    {
        init = true;
        ASCIIMeshReader reader(mesh_filename_, mock_builder_);
        reader.read();
    }
}


/* setup reader with mock bilder,
 * the mock bilder just stores the items without actually building the mesh
 * 
 * 
 * 
 * */
