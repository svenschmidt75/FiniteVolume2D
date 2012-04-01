/*
 * Name  : EntityTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/01/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "FiniteVolume2DLib/Mesh.h"
#include "MeshBuilderMock.h"


class EntityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(EntityTest);
    CPPUNIT_TEST(testInternalGetFaceVertices);
    CPPUNIT_TEST(testBoundaryGetFaceVertices);
    CPPUNIT_TEST(testGetCellFaces);
    CPPUNIT_TEST(testGetCellVertices);
    CPPUNIT_TEST(testFaceArea);
    CPPUNIT_TEST(testCellVolume);
    CPPUNIT_TEST(testFaceCentroid);
    CPPUNIT_TEST(testCellCentroid);
    CPPUNIT_TEST(testFaceNormal);
    CPPUNIT_TEST_SUITE_END();

private:
    void initMesh();

public:
    void setUp();
    void tearDown();

protected:
    void testInternalGetFaceVertices();
    void testBoundaryGetFaceVertices();
    void testGetCellFaces();
    void testGetCellVertices();
    void testFaceArea();
    void testCellVolume();
    void testFaceCentroid();
    void testCellCentroid();
    void testFaceNormal();

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
