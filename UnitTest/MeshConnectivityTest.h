/*
 * Name  : MeshConnectivityTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "internal/MeshBuilderMock.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshConnectivityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshConnectivityTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(testNumberOfVerices);
    CPPUNIT_TEST(testNumberOfFaces);
    CPPUNIT_TEST(testNumberOfCells);
    CPPUNIT_TEST(testBoundaryVertexNeighbors);
    CPPUNIT_TEST(testInternalVertexNeighbors);
    CPPUNIT_TEST(testInternalVertexFaceNeighbors);
    CPPUNIT_TEST(testBoundaryVertexFaceNeighbors);
    CPPUNIT_TEST(testInternalVertexCellNeighbors);
    CPPUNIT_TEST(testBoundaryVertexCellNeighbors);
    CPPUNIT_TEST(testInteriorFaceGetAttachedCells);
    CPPUNIT_TEST(testBoundaryFaceGetAttachedCells);
    CPPUNIT_TEST(testInteriorGetOtherCell);
    CPPUNIT_TEST(testBoundaryGetOtherCell);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void testNumberOfVerices();
    void testNumberOfFaces();
    void testNumberOfCells();
    void testBoundaryVertexNeighbors();
    void testInternalVertexNeighbors();
    void testInternalVertexFaceNeighbors();
    void testBoundaryVertexFaceNeighbors();
    void testInternalVertexCellNeighbors();
    void testBoundaryVertexCellNeighbors();
    void testInteriorFaceGetAttachedCells();
    void testBoundaryFaceGetAttachedCells();
    void testInteriorGetOtherCell();
    void testBoundaryGetOtherCell();
    void testGetOtherCell();

private:
    void initMesh();

private:
    typedef Mesh::CPtr MeshPtr;

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
