/*
 * Name  : ComputationalMeshSolverHelperTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "internal/MeshBuilderMock.h"


class ComputationalMeshSolverHelperTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalMeshSolverHelperTest);
    CPPUNIT_TEST(matrixConstructionTest);
    CPPUNIT_TEST(rhsTest);
    CPPUNIT_TEST(solutionInMeshTest);
    CPPUNIT_TEST(checkFluxBalanceTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void matrixConstructionTest();
    void rhsTest();
    void solutionInMeshTest();
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
