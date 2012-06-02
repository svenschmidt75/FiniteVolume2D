/*
 * Name  : ComputationalMeshBuilderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

#include "internal/MeshBuilderMock.h"

#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include <cppunit/extensions/HelperMacros.h>


class ComputationalMeshBuilderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalMeshBuilderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(evaluateFluxesDummyTest);
    CPPUNIT_TEST(noActiveVarsTest);
    CPPUNIT_TEST(addPassiveVarSameAsActiveVarTest);
    CPPUNIT_TEST(addPassiveVarTwiceTest);
    CPPUNIT_TEST(addPassiveVarForSeveralDifferentEntitiesTest);
    CPPUNIT_TEST(addUserDefinedNodeVarsTest);
    CPPUNIT_TEST(addUserDefinedFaceVarsTest);
    CPPUNIT_TEST(addUserDefinedCellVarsTest);
    CPPUNIT_TEST(addCellVarsTest);
    CPPUNIT_TEST(evaluateFluxesTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void evaluateFluxesDummyTest();
    void noActiveVarsTest();
    void addPassiveVarSameAsActiveVarTest();
    void addPassiveVarTwiceTest();
    void addPassiveVarForSeveralDifferentEntitiesTest();
    void addUserDefinedNodeVarsTest();
    void addUserDefinedFaceVarsTest();
    void addUserDefinedCellVarsTest();
    void addCellVarsTest();
    void evaluateFluxesTest();

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
    static MeshBuilderMock             mesh_builder_;

    static BoundaryConditionCollection bc_;
};
