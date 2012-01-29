
class Mesh {

};

class MeshBuilder {

};

class IMeshReader {

};

class SimpleMeshReader : public IMeshReader {

};

class ECLMeshReader : public IMeshReader {

};

class ComputeManager {
    // To separate computational aspects from geometrical
};

class Cell : public GeometricEntity {

    ComputeManager::Ptr cmgr_;
};


class NodeDependency {
public:
    // Id of node this dependency is for
    NodeId id_;
    
    // Target node id
    NodeId other_id_;

    // Weight
    double weight_;
};

class ComputationalMolecule {
public:
    // Id of node this dependency is for
    NodeId id_;

    std::vector<NodeDependency> dependencies_;
};

class INodeVariable {
public:
    ComputationalMolecule::Ptr molecule_;
};

class PressureNodeVariable : public INodeVariable {
public:
    // p for pressure etc.
    std::string variable_text_;
    double value_;
};


void
pressure_cell_derivative(IComputationalGridAccessor & cgrid, Cell::Ptr cell) {
    /* Compute the average derivative of pressure in cell 'cell'.
     * 
     * This is done by evaluating pressure at the cell faces.
     * Here, we apply the trapezoidal rule to evaluate the
     * integrals, a second order approximation. The derivative
     * is approximated by its average value over the entire cell.
     * Source: Hirsch, vol. 1, p. 222, (5.3.21)
     */

    // Passive variables are used to cache already computed variables,
    // they are not solved for.
    ComputationalMolecule & cfx = cell->getPassiveVariable("Ux");
    ComputationalMolecule & cfy = cell->getPassiveVariable("Uy");

    // Derivative already computed?
    if (!cfx.empty())
        return;

    double cell_volume = cell->volume();
    double factor = - 0.5 / cell_volume;

    for (int face_index = 0; face_index < cell->nfaces(); ++face_index) {
        Face::ptr current_face = cell->getFace(face_index);






        int next_face_index = (face_index + 1) % cell->nfaces;

        Face::ptr next_face = cell->getFace(next_face_index);

        Cell::ptr cell_nbr = next_face->getCellNbr(cell);
        Vertex cell_centroid = cell_nbr->centroid();
        double y = centroid * factor;
        CellId cell_nbr_id = cell_nbr->id();

        // Get cell id of neighbor to face prev_face
        other_cell = prev_face->getCellNbr(cell);

        cf.insert<std::plus>(cell_nbr_id, y);
        cf.insert<std::minus>(cell_nbr->id(), y);

        // Same for Uy
    }
}

void
pressure_vertex_derivative(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Vertex::Ptr vertex) {
    /* Compute the pressure derivative at face vertices.
     * 
     * The derivative of pressure is calculated at the cell level.
     * Here, we simply compute the average of the derivative of
     * pressure of the surrounding cells and assign this value
     * to the vertex.
     * Alternatively, we could weight the contribution of each
     * cell by its volume.
     * Source: Hirsch, vol. 1, p. 222, (5.3.21)
     */
    ComputationalMolecule & cfx = vertex->getMolecule("Ux");
    ComputationalMolecule & cfy = vertex->getMolecule("Uy");

    // Already computed?
    if (!cfx.empty())
        return;

    double cell_volume = cell->volume();
    double factor = - 0.5 / cell_volume;

    ComputationalMolecule face_vertex_cm;

    // Get all cells attached to this vertex
    CellCollection cc = cgrid->getAttachedCells(vertex);

    for (CellCollection::Iterator it(cc.begin()); it != cc.end(); ++it) {
        Cell:Ptr cell_nbr = *it;

        face_vertex_cm.insert(cell_nbr->id(), )

        ComputationalMolecule & cm = cell_nbr->getComputationalMolecule("Ux");






        Cell::ptr other_cell = current_face->getOtherCell(cell);
        Vertex cell_centroid = other_cell->centroid();
        double y = centroid * factor;
        CellId current_cell_id = other_cell->id();

    }
}

bool
pressure_flux_evaluator(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Face::Ptr face)
{
    /* Compute flux through a cell face.
     * 
     * We compute the flux through face 'face' by taking
     * the average value of the flux variable at start and
     * end vertex of the face.
     * Source: Hirsch, vol. 1, p. 224, (E5.3.14)
     */

    // Get flux molecule
    ComputationalMolecule & flux_molecule = face->getComputationalMolecule("pFlux");

    // Flux through face already computed?
    if (!flux_molecule.empty())
        return;

    Vertex::ptr start = face->startVertex();
    ComputationalMolecule & start_cm = start->getComputationalMolecule("Ux");

    // Already computed?
    if (!start_cm.empty())
        return false;

    Vertex::ptr end = face->endVertex();
    ComputationalMolecule & end_cm = start->getComputationalMolecule("Ux");

    // Already computed?
    if (!end_cm.empty())
        return false;

    ComputationalMolecule flux_cm = 0.5 * (start_cm + end_cm);
    face->setComputationalMolecule("pFlux", flux_cm);

    return true;
}

void
pressure_computational_molecule(IComputationalGridAccessor & cgrid, Cell::Ptr cell)
{
    /* Get the final computational molecule for cell 'cell'.
     * This is essentially a row in the linear equation.
     */
    FaceCollection fc(cell->faces());

    for (FaceCollection::Iterator face_it(fc.begin()); face_it != fc.end(); ++face_it) {
        Face const & face = *face_it;

        ComputationalMolecule & end_cm = start->getComputationalMolecule("Ux");

        // get the face flux
        ComputationalMolecule & flux_molecule = face->getComputationalMolecule("pFlux");
        if (flux_molecule.empty())
            throw std::exception("Empty face flux");



        WHAT about (F * n) dS?
        This should be decoded in the weight a neighboring cell
        contributes.


    }



    // Compute fluxes through all cell faces
}


typedef std::function<void (IComputationalGridAccessor &, Cell::Ptr)> CompMoleculeCallBack;
typedef std::function<void (IComputationalGridAccessor &, Face::Ptr)> FluxCallBack;

int main(int argc, char* argv[])
{
    Mesh mesh;
    MeshBuilder mesh_builder(mesh);
    SimpleMeshReader reader(mesh_builder);
    reader.read_mesh();

    ComputationalGrid cgrid(mesh);
    cgrid.addVariable(PressureNodeVariable("p", CompMoleculeCallBack(pressure_computational_molecule)));
    cgrid.gradientEvaluator(DerivativeEvaluator("Ux", FluxCallBack(pressure_gradient_evaluator)));
    cgrid.gradientEvaluator(DerivativeEvaluator("Uy", FluxCallBack(pressure_gradient_evaluator)));
    cgrid.fluxEvaluator(PressureFluxEvaluator("p", FluxCallBack(pressure_flux_evaluator)));

    
    return 0;
}

