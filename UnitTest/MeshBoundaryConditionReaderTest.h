/*
 * Name  : MeshBoundaryConditionReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "MeshBuilderMock.h"

#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshBoundaryConditionReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshBoundaryConditionReaderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(testNumberOfDirichletBoundaryConditions);
    CPPUNIT_TEST(testNumberOfNeumannBoundaryConditions);
    CPPUNIT_TEST(testDirichlet);
    CPPUNIT_TEST(testNeumann);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void testNumberOfDirichletBoundaryConditions();
    void testNumberOfNeumannBoundaryConditions();
    void testDirichlet();
    void testNeumann();

private:
    void initMesh();

private:
    typedef Mesh::Ptr MeshPtr;

private:
    std::string                        mesh_filename_;
    static MeshBuilderMock             mock_builder_;
    static BoundaryConditionCollection bc_;
};
