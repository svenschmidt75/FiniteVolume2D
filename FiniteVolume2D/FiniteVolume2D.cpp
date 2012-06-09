----------------------------------------------------

Why the MeshConnectivity object?

Each entity like face and cell knows about the "smaller" entities
it is made up of. A face is made up of vertices, a cell is made up
of faces etc. So, the direction top-down is covered like this.
However, the direction bottom-up must be handled separately. A
vertex cannot know which faces and cells it is attached to. These
questions answers the Mesh Connectivity object, in.e. it covers the
path bottom-up.

----------------------------------------------------

- monitor skewness
- monitor aspect ratio

----------------------------------------------------
All comp. entities inherit from ICompEntity,
with
addComputationalMolecule
getComputationalMolecule

----------------------------------------------------

Read data directly in state, if something found that
is not recognized, return to general state.
Deal with errors there?

----------------------------------------------------
TO-DO

ICell: Node/ComputationalNode => covariant return types

----------------------------------------------------

Face flux according to the one cell or the other negates sign

----------------------------------------------------
Operators for CompMolecule

----------------------------------------------------
Own iterator:

Insteam of Item, just wrap a REFERENCE in Item and
have a "smaller" interface.

This way, FluxEvaluator can have state?


----------------------------------------------------
Check that every b.f. has a b.c.!!

----------------------------------------------------




std::vector<std::shared_ptr<Type>> coll;

typename Entity::Ptr & operator[](int index) {
    return coll[index];
}

coll[index] = std::shared_ptr<Type>;

Now:
Entity* ptr = *(coll[index]);

std::shared_ptr<Entity const> sp = std::shared_ptr<Entity const>().reset(ptr);






















----------------------------------------------------


void
pressure_vertex_derivative(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Node::Ptr vertex) {
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

        Node cell_centroid = current_cell->centroid();

        double y = cell_centroid.y();

        cfx.insert(next_cell->id(),   factor * y);
        cfx.insert(prev_cell->id(), - factor * y);


        double x = cell_centroid.x();

        cfy.insert(next_cell->id(), - factor * x);
        cfy.insert(prev_cell->id(),   factor * x);
    }
}

bool
pressure_flux_evaluator_for_later(IComputationalGridAccessor & cgrid, Cell::Ptr cell, Face::Ptr face)
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

bool
phi_flux_evaluator(IComputationalGridAccessor const & cgrid, Cell::Ptr const & cell, Face::Ptr const & face)
{
    /* Compute flux through a cell face. The cell face may be a boundary face
     * needing special treatment depending on whether Dirichlet or von Neumann
     * boundary conditions have been prescribed.
     * 
     * Flux through an internal face is evaluated by the usual gradient approximation.
     * Note that this is not 2nd order, not even is there is no cell skewness as we
     * omit the cross-diffusion term (Comp. Fluid Dynamics, Versteeg, p. 319).
     */

    // Get flux molecules at face
    FluxComputationalMolecule & flux_molecule = face->getComputationalMolecule("phi_flux");

    // Flux through face already computed?
    if (!flux_molecule_x.empty())
        return true;

    /* We need to know the cell the flux was calculated with. This is because
     * once the neighboring cell accesses the flux, the need to invert the sign.
     */
    flux_molecule->setCell(cell);

    if (face->isBoundaryFace()) {
        BoundaryCondition const & bc = face->getBoundaryCondition();

        // get cell node variable
        ComputationalMolecule & cell_molecule = cell->getComputationalMolecule("phi");

        boost::optional<double> value = bc.getBoundaryValue();
        if (value) {
            /* The boundary condition is of type Dirichlet. Compute the
             * face flux using the half-cell approximation.
             * Versteeg, p. 331
             */
            SourceTerm & face_source = flux_mulecule.getSourceTerm();

            // compute face mid point
            Vertex midpoint = (face->start() + face->end()) / 2.0;
            
            // distance from face midpoint to the cell centroid
            double dist = Math::dist(cell->centroid(), midpoint);

            // the boundary value contributes as a source term
            face_source += face->area() / dist * value;

            // get comp. variable to solve for
            CompVar const & cvar = cell->getComputationalVariable("phi");
            flux_molecule.add(cvar, -face->area() / dist);

            // contribution to the cell node
            flux_molecule.add(cell->getComputationalVariable(), -face->area() / dist);
        }
        else {
            // Face b.c. given as von Neumann
            SourceTerm & face_source = flux_mulecule.getSourceTerm();

            // the boundary flux contributes as a source term
            face_source += bc.getFluxValue();
        }
        return true;
    }

    // internal face


    // Compute distance to face midpoint to monitor accuracy

    
    /* We have to approximate the term
     * <\hat{n}, \gamma \grad \phi f_{area}>
     * compute the usual gradient approximation,
     * \grad \phi \approx \frac{phi_{N} - phi_{P}}{\dist N - P}
     */
    Vertex cell_centroid = cell->centroid();

    Cell::Ptr cell_nbr = cgrid.getOtherCell(face, cell);
    Vertex cell_nbr_centroid = cell_nbr->centroid();

    // distance from face midpoint to the cell centroid
    double dist = Math::dist(cell_centroid, cell_nbr_centroid);

    /* The weight for the computational molecule is
     * \gamma f_{area} / dist(N - P).
     */
    double weight = face->area() / dist;
    
    // get comp. variable to solve for
    CompVar const & cvar = cell->getComputationalVariable("Temp");

    CompVar const & cvar = cell->getComputationalVariable("phi");
    CompVar const & cvar_nbr = cell_nbr->getComputationalVariable("phi");

    flux_molecule.add(cvar, -weight);
    flux_molecule.add(cvar_nbr, weight);

    return true;
}

bool
phi_computational_molecule(IComputationalGridAccessor const & cgrid, Cell::Ptr const & cell)
{
    /* Get the final computational molecule for cell 'cell'.
     * This is essentially a row in the linear equation.
     */
    FaceCollection fc(cell->faces());

    ComputationalMolecule & cell_cm = cell->getComputationalMolecule("phi");
    cell_cm->clear();

    SourceTerm & cell_source = cell_cm.GetSource();

    for (FaceCollection::Iterator face_it(fc.begin()); face_it != fc.end(); ++face_it) {
        Face::Ptr const & face = *face_it;

        // get the face flux
        ComputationalMolecule const & flux_molecule = face->getComputationalMolecule("phi_flux");

        if (flux_molecule.empty())
            throw std::exception("Empty face flux");

        // if the flux was computed from the neighbor cell
        // to this cell, we must take the nagative
        if (flux_molecule->getCell() == cell)
            cell_cm += flux_molecule;
        else {
          if (flux_molecule->getCell() == cgrid.getOtherCell(face, cell))
            cell_cm -= flux_molecule;
            
          else
            ERROR
        }
        cell_source += flux_molecule.getSource();
    }
}


typedef std::function<void (IComputationalGridAccessor &, Cell::Ptr)> CompMoleculeCallBack;
typedef std::function<void (IComputationalGridAccessor &, Face::Ptr)> FluxCallBack;

int main(int argc, char* argv[])
{
    Mesh mesh;
    MeshBuilder mesh_builder;
    SimpleMeshReader reader(mesh_filename, mesh_builder);
    mesh_builder.build();

    mesh = mesh_builder.grid();

    ComputationalGrid cgrid(mesh);
    cgrid.nodeVariable(NodeVariable("phi", CompMoleculeCallBack(pressure_computational_molecule)));
    cgrid.fluxEvaluator(FluxEvaluator("phi_flux", FluxCallBack(phi_flux_evaluator)));
    cgrid.solve();
    
    sa = cgrid.getSolutionAccessor();
    double value = sa.getValue("phi", cell_id);
    
    return 0;
}
