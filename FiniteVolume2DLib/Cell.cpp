#include "Cell.h"

#include <algorithm>


Cell::Cell(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces)
    : cell_id_(cell_id), mesh_id_(mesh_id), faces_(faces) {


    // this is necessary since the compiler expects the references passed to the lambda expression
    // in the current scope. VS2010 compiler bug? See http://connect.microsoft.com/VisualStudio/feedback/details/560907/capturing-variables-in-nested-lambdas
    EntityCollection<Vertex> & v = vertices_;

    // extract vertices
    std::for_each(faces.begin(), faces.end(), [&v](Face::Ptr const & face) {
        // Extract and check all vertices before inserting into v
        EntityCollection<Vertex> const & vertices = face->getVertices();

        for (EntityCollection<Vertex>::size_type i = 0; i < vertices.size(); ++i) {
            Vertex::Ptr vert = vertices.getEntity(i);
            if (!v.find(vert->id()))
                v.insert(vert);
        }
    });
}

IGeometricEntity::Id_t
Cell::id() const {
    return cell_id_;
}

IGeometricEntity::Id_t
Cell::meshId() const {
    return mesh_id_;
}

EntityCollection<Vertex> const &
Cell::getVertices() const {
    return vertices_;
}

EntityCollection<Face> const &
Cell::getFaces() const {
    return faces_;
}

Cell::Ptr
Cell::create(IGeometricEntity::Id_t cell_id, IGeometricEntity::Id_t mesh_id, EntityCollection<Face> const & faces) {
    Cell::Ptr cell = Cell::Ptr(new Cell(cell_id, mesh_id, faces));
    return cell;
}
