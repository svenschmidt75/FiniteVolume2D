
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
pressure_vertex_derivative(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Vertex::Ptr vertex) {
    /* Compute the pressure derivative at face vertices.
     * 
     * We collect all cells that share the vertex. Then
     * we connect the cell centers pairwise s.t. it defines
     * a virtual cell volume. We integrate the pressure over
     * this control volume using the trapezoidal rule.
     * This yields the derivative to 2nd order accuracy.
     * Source: Hirsch, vol. 1, p. 222, (5.3.21)
     */
    ComputationalMolecule & cfx = vertex->getMolecule("Phi_Dx");
    ComputationalMolecule & cfy = vertex->getMolecule("Phi_Dy");

    // Already computed?
    if (!cfx.empty())
        return;

    double cell_volume = cell->volume();
    double factor = - 0.5 / cell_volume;

    ComputationalMolecule face_vertex_cm;

    // Get all cells attached to this vertex
    CellCollection cc = cgrid->getAttachedCells(vertex);

    for (int cell_index = 0; cell_index < cc.size(); ++cell_index) {
        int prev_cell_index = (cell_index - 1) % cc.size();
        if (prev_cell_index < 0)
            prev_cell_index -= prev_cell_index;

        int next_cell_index = (cell_index + 1) % cc.size();

        Cell::ptr prev_cell = cc.getCellAt(prev_cell_index);
        Cell::ptr current_cell = cc.getCellAt(cell_index);
        Cell::ptr next_cell = cc.getCellAt(next_cell_index);

        Vertex cell_centroid = current_cell->centroid();

        double y = cell_centroid.y();

        cfx.insert(next_cell->id(),   factor * y);
        cfx.insert(prev_cell->id(), - factor * y);


        double x = cell_centroid.x();

        cfy.insert(next_cell->id(), - factor * x);
        cfy.insert(prev_cell->id(),   factor * x);
    }
}

bool
pressure_flux_evaluator(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Face::Ptr face)
{
    /* Compute flux through a cell face.
     * 
     * We compute the flux through face 'face' by taking
     * the average value of the flux variable at start and
     * end vertex of the face. This is the trapezoidal rule
     * and this is 2nd order accurate.
     * Source: Hirsch, vol. 1, p. 224, (E5.3.14)
     */

    // Get flux molecules at face
    ComputationalMolecule const & flux_molecule_x = face->getComputationalMolecule("Flux_Phi_x");
    ComputationalMolecule const & flux_molecule_y = face->getComputationalMolecule("Flux_Phi_y");

    // Flux through face already computed?
    if (!flux_molecule_x.empty())
        return true;

    ComputationalMolecule const & start_cm_x = start->getComputationalMolecule("Phi_Dx");
    ComputationalMolecule const & start_cm_y = start->getComputationalMolecule("Phi_Dy");

    // Already computed?
    if (!start_cm_x.empty())
        return true;

    // Already computed?
    if (!end_cm_x.empty())
        return true;


    ComputationalMolecule flux_cm_x = 0.5 * (start_cm_x + end_cm_x);
    face->setComputationalMolecule("Flux_Phi_x", flux_cm_x);

    ComputationalMolecule flux_cm_y = 0.5 * (start_cm_y + end_cm_y);
    face->setComputationalMolecule("Flux_Phi_y", flux_cm_y);

    return true;
}

void
pressure_computational_molecule(IComputationalGridAccessor & cgrid, Cell::Ptr cell)
{
    /* Get the final computational molecule for cell 'cell'.
     * This is essentially a row in the linear equation.
     */
    FaceCollection fc(cell->faces());

    ComputationalMolecule & cell_cm = start->getComputationalMolecule("Phi");
    cell_cm->clear();

    for (FaceCollection::Iterator face_it(fc.begin()); face_it != fc.end(); ++face_it) {
        Face::Ptr const & face = *face_it;

        // get the face fluxes
        ComputationalMolecule const & flux_molecule_x = face->getComputationalMolecule("Phi_Flux_x");
        ComputationalMolecule const & flux_molecule_y = face->getComputationalMolecule("Phi_Flux_y");

        if (flux_molecule_x.empty())
            throw std::exception("Empty face flux");

        Vertex::Ptr const & start = face->startVertex();
        Vertex::Ptr const & end = face->endVertex();

        double dx = end.x() - start.x();
        double dy = end.y() - start.y();

        ComputationalMolecule cm = - 0.5 * dx * flux_molecule_x;
        cm += (0.5 * dy * flux_molecule_y);
        cell_cm += cm;
    }
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
    cgrid.addVariable(PhiNodeVariable("p", CompMoleculeCallBack(pressure_computational_molecule)));
    cgrid.gradientEvaluator(DerivativeEvaluator("Ux", FluxCallBack(pressure_gradient_evaluator)));
    cgrid.gradientEvaluator(DerivativeEvaluator("Uy", FluxCallBack(pressure_gradient_evaluator)));
    cgrid.fluxEvaluator(PressureFluxEvaluator("p", FluxCallBack(pressure_flux_evaluator)));

    
    return 0;
}

