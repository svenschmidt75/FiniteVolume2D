/*
 * Name  : MeshBoundaryConditionReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "MeshBuilderMock.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshBoundaryConditionReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshBoundaryConditionReaderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();

private:
    void initMesh();

private:
    typedef Mesh::Ptr MeshPtr;

private:
    std::string            mesh_filename_;
    static MeshPtr         mesh_;

    /* If we were to use MeshBuilder directly, we would need a default constructor
     * for MeshBuilder, because we need to declare it a static data member.
     * There is no easy way to define a default constructor because of the
     * EntityManager. Hence, this embedded mesh builder.
     */
    static MeshBuilderMock mesh_builder_;
};
