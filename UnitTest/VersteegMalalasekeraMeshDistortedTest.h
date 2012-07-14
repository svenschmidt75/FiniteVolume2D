/*
 * Name  : VersteegMalalasekeraMeshDistortedTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/14/2012
 */
#pragma once

#include "internal/MeshBuilderMock.h"

#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include <cppunit/extensions/HelperMacros.h>


class VersteegMalalasekeraMeshDistortedTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VersteegMalalasekeraMeshDistortedTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(checkFluxBalanceTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void checkFluxBalanceTest();

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
