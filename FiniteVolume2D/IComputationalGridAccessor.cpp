#include "IComputationalGridAccessor.h"

#include "FiniteVolume2DLib/IMeshConnectivity.h"

#include "FiniteVolume2D/GeometricalEntityMapper.h"


IComputationalGridAccessor::IComputationalGridAccessor(IMeshConnectivity const & mesh_connectivity, GeometricalEntityMapper const & mapper)
    :
    mesh_connectivity_(mesh_connectivity),
    mapper_(mapper) {}

ComputationalCell::Ptr
IComputationalGridAccessor::getOtherCell(ComputationalFace::Ptr const & cface, ComputationalCell::Ptr const & ccell) const {
    Cell::Ptr other_cell = mesh_connectivity_.getOtherCell(cface->geometricEntity(), ccell->geometricEntity());

    ComputationalCell::Ptr const & other_ccell = mapper_.getComputationalCell(other_cell);
    return other_ccell;
}
