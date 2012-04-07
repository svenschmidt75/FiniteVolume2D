#include "BoundaryCondition.h"

bool
BoundaryCondition::add(IGeometricEntity::Id_t face_id, BoundaryCondition::Type bc_type, double bc_value) {
    if (find(face_id))
        // already a bc defined for this face
        return false;

    data_[face_id] = Pair(bc_type, bc_value);
    return true;
}

boost::optional<BoundaryCondition::Pair>
BoundaryCondition::find(IGeometricEntity::Id_t face_id) const {
    auto it = data_.find(face_id);
    if (it == data_.end())
        return boost::optional<Pair>();

    return boost::optional<Pair>(it->second);
}
