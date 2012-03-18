/*
 * Name  : MeshConnectivityTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "FiniteVolume2DLib/MeshBuilder.h"
#include "FiniteVolume2DLib/FaceManager.h"
#include "FiniteVolume2DLib/CellManager.h"
#include "FiniteVolume2DLib/EntityCreatorManager.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshConnectivityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshConnectivityTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST_SUITE_END();

private:
    /* If we were to use MeshBuilder directly, we would need a default constructor
     * for MeshBuilder, because we need to declare it a static data member.
     * There is no easy way to define a default constructor because of the
     * EntityManager. Hence, this embedded mesh builder.
     */
    class MeshBuilderMock : public IMeshBuilder {
    public:
        MeshBuilderMock() {
            EntityCreatorManager::Ptr emgr = EntityCreatorManager::create(VertexManager::create(), FaceManager::create(), CellManager::create());
            mesh_builder_ = std::shared_ptr<MeshBuilder>(new MeshBuilder(emgr));
        }

        bool buildVertex(IGeometricEntity::Id_t vertex_id, bool on_boundary, double x, double y) {
            return mesh_builder_->buildVertex(vertex_id, on_boundary, x, y);
        }

        bool buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {
            return mesh_builder_->buildFace(face_id, on_boundary, vertex_ids);
        }

        bool buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
            return mesh_builder_->buildCell(cell_id, face_ids);
        }

        boost::optional<Mesh::Ptr> getMesh() const {
            return mesh_builder_->getMesh();
        }

    private:
        MeshBuilder::Ptr mesh_builder_;
    };

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();

private:
    void initMeshConnectivityTest();

private:
    typedef Mesh::Ptr MeshPtr;
private:
    std::string mesh_filename_;
    static MeshBuilderMock mesh_builder_;
    static MeshPtr mesh_;
};
