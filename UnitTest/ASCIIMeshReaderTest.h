/*
 * Name  : ASCIIMeshReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "FiniteVolume2DLib/IMeshBuilder.h"

#include <cppunit/extensions/HelperMacros.h>


class ASCIIMeshReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ASCIIMeshReaderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(testNumberOfVertices);
    CPPUNIT_TEST_SUITE_END();

private:
    class MockMeshBuilder : public IMeshBuilder {
    public:
        bool
        buildVertex(IGeometricEntity::Id_t vertex_id, double x, double y) {
            VertexMap_t::const_iterator it = vertices_.find(vertex_id);
            if (it != vertices_.end())
                CPPUNIT_FAIL("Duplicate vertex!");
            vertices_[vertex_id] = std::make_pair(x, y);
            return true;
        }

        bool
        buildFace(IGeometricEntity::Id_t face_id, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {
            FaceMap_t::const_iterator it = faces_.find(face_id);
            if (it != faces_.end())
                CPPUNIT_FAIL("Duplicate face!");
            faces_[face_id] = vertex_ids;
            return true;
        }

        bool
        buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
            CellMap_t::const_iterator it = cells_.find(cell_id);
            if (it != cells_.end())
                CPPUNIT_FAIL("Duplicate cell!");
            cells_[cell_id] = face_ids;
            return true;
        }

        boost::optional<Mesh::Ptr>
        getMesh() const {
            return boost::optional<Mesh::Ptr>();
        }

    private:
        typedef std::map<IGeometricEntity::Id_t, std::pair<double, double>> VertexMap_t;
        VertexMap_t vertices_;

        typedef std::map<IGeometricEntity::Id_t, std::vector<IGeometricEntity::Id_t>> FaceMap_t;
        FaceMap_t faces_;

        typedef std::map<IGeometricEntity::Id_t, std::vector<IGeometricEntity::Id_t>> CellMap_t;
        CellMap_t cells_;
    };

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void testNumberOfVertices();

private:
    void initMesh();

private:
    std::string mesh_filename_;
    MockMeshBuilder mock_builder_;
};
