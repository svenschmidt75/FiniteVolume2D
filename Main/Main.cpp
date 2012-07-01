#include "FiniteVolume2DLib/ASCIIMeshReader.h"
#include "FiniteVolume2DLib/MeshBuilder.h"
#include "FiniteVolume2DLib/NodeManager.h"
#include "FiniteVolume2DLib/FaceManager.h"
#include "FiniteVolume2DLib/CellManager.h"
#include "FiniteVolume2DLib/EntityManager.hpp"
#include "FiniteVolume2DLib/Util.h"
#include "FiniteVolume2DLib/Mesh.h"
#include "FiniteVolume2DLib/BoundaryConditionCollection.h"
#include "FiniteVolume2DLib/MeshChecker.h"
#include "FiniteVolume2DLib/Math.h"
#include "FiniteVolume2DLib/Vertex.h"
#include "FiniteVolume2DLib/EntityCollection.hpp"

#include "FiniteVolume2D/ComputationalMesh.h"
#include "FiniteVolume2D/ComputationalMeshBuilder.h"
#include "FiniteVolume2D/IComputationalGridAccessor.h"
#include "FiniteVolume2D/ComputationalMeshSolverHelper.h"
#include "FiniteVolume2D/ComputationalMolecule.h"
#include "FiniteVolume2D/FluxComputationalMolecule.h"
#include "FiniteVolume2D/SourceTerm.h"
#include "FiniteVolume2D/BoundaryCondition.h"
#include "FiniteVolume2D/ComputationalVariable.h"
#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/ComputationalFace.h"



#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <string>
#include <exception>
#include <algorithm>



namespace {
  
    bool
    flux_evaluator(IComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & ccell, ComputationalFace::Ptr const & cface)
    {
        /* Compute flux through a cell face. The cell face may be a boundary face
         * needing special treatment depending on whether Dirichlet or von Neumann
         * boundary conditions have been prescribed.
         * 
         * Flux through an internal face is evaluated by the usual gradient approximation.
         * Note that this is not 2nd order, not even is there is no cell skewness as we
         * omit the cross-diffusion term (Comp. Fluid Dynamics, Versteeg, p. 319).
         * 
         * Note that we are assuming that the face normal and the vector connecting
         * the two cell centers are aligned, i.e. that the mesh is fully orthogonal.
         */

        // Get face flux molecules for "Temperature"
        FluxComputationalMolecule & flux_molecule = cface->getComputationalMolecule("Temperature");

        // Flux through face already computed?
        if (!flux_molecule.empty())
            return true;

        /* We need to know the cell the flux was calculated with. This is because
         * once the neighboring cell accesses the flux, the need to invert the sign.
         */
        flux_molecule.setCell(ccell);

        BoundaryCondition::Ptr const & bc = cface->getBoundaryCondition();

        if (bc) {
            if (bc->type() == BoundaryConditionCollection::DIRICHLET) {
                /* The boundary condition is of type Dirichlet. Compute the
                 * face flux using the half-cell approximation.
                 * Versteeg, p. 331
                 */
                double value = bc->getValue();

                SourceTerm & face_source = flux_molecule.getSourceTerm();

                // compute face mid point
                Vertex midpoint = (cface->startNode().location() + cface->endNode().location()) / 2.0;

                // distance from face midpoint to the cell centroid
                double dist = Math::dist(ccell->centroid(), midpoint);

                // the boundary value contributes as a source term
                face_source += cface->area() / dist * value;

                // get comp. variable to solve for
                ComputationalVariable::Ptr const & cvar = ccell->getComputationalVariable("Temperature");

                // insert with opposite sign (convention)
                flux_molecule.add(*cvar, cface->area() / dist);
            }
            else {
                // Face b.c. given as von Neumann
                SourceTerm & face_source = flux_molecule.getSourceTerm();

                // the boundary flux contributes as a source term
                face_source += bc->getValue();
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
        Vertex cell_centroid = ccell->centroid();

        ComputationalCell::Ptr const & cell_nbr = cgrid.getOtherCell(cface, ccell);
        Vertex cell_nbr_centroid = cell_nbr->centroid();

        // distance from face midpoint to the cell centroid
        double dist = Math::dist(cell_centroid, cell_nbr_centroid);

        /* The weight for the computational molecule is
         * \gamma f_{area} / dist(N - P).
         */
        double weight = cface->area() / dist;
    
        // get comp. variable to solve for
        ComputationalVariable::Ptr const & cvar = ccell->getComputationalVariable("Temperature");
        ComputationalVariable::Ptr const & cvar_nbr = cell_nbr->getComputationalVariable("Temperature");

        // insert with opposite sign (convention)
        flux_molecule.add(*cvar,      weight);
        flux_molecule.add(*cvar_nbr, -weight);

        return true;
    }

    bool cell_evaluator(ComputationalCell::Ptr const & ccell) {
        EntityCollection<ComputationalFace> const & cface_coll = ccell->getComputationalFaces();

        // Get cell computational molecule for "Temperature"
        ComputationalMolecule & cmolecule = ccell->getComputationalMolecule("Temperature");

        std::for_each(cface_coll.begin(), cface_coll.end(), [&](ComputationalFace::Ptr const & cface) {
            // Get face flux molecules for "Temperature"
            FluxComputationalMolecule & flux_molecule = cface->getComputationalMolecule("Temperature");

            /* negate the weights if they were calculated with the neighboring
             * cell
             */
            ComputationalCell::Ptr const & ccell_ref = flux_molecule.getCell();
            if (ccell_ref != ccell)
                flux_molecule = -flux_molecule;

            cmolecule.addMolecule(flux_molecule);
        });

        // account for r.h.s.
        SourceTerm & st = cmolecule.getSourceTerm();
        st  += 0.0;

        return true;
    }

}



int main(int /*argc*/, char* /*argv[]*/)
{
    std::string mesh_filename = "..\\UnitTest\\Data\\Versteeg_Malalasekera_11_25.mesh";

    NodeManager::Ptr node_mgr = NodeManager::create();
    FaceManager::Ptr face_mgr = FaceManager::create();
    CellManager::Ptr cell_mgr = CellManager::create();
    EntityCreatorManager::Ptr entity_mgr = EntityCreatorManager::create(node_mgr, face_mgr, cell_mgr);

    /* 
     * Build geometrical mesh
     */

    MeshBuilder mesh_builder(entity_mgr);
    ASCIIMeshReader reader(mesh_filename, mesh_builder);
    if (!reader.read()) {
        boost::format format = boost::format("Could not read mesh file %1%!\n") % mesh_filename;
        Util::error(format.str());
        return 1;
    }

    Mesh::Ptr mesh = nullptr;
    boost::optional<Mesh::Ptr> mesh_opt = mesh_builder.getMesh();
    if (mesh_opt)
        mesh = *mesh_opt;

    if (mesh == nullptr) {
        boost::format format = boost::format("Unable to build mesh!\n");
        Util::error(format.str());
        return 1;
    }

    BoundaryConditionCollection const & bc = reader.getBoundaryConditions();

    bool success = MeshChecker::checkMesh(mesh, bc);
    if (!success) {
        boost::format format = boost::format("Mesh check failed!\n");
        Util::error(format.str());
        return 1;
    }


    /* 
     * Build computational mesh
     */

    ComputationalMeshBuilder computational_builder(mesh, bc);

    // Temperature as cell-centered variable, will be solved for
    computational_builder.addComputationalVariable("Temperature", flux_evaluator);
    computational_builder.addEvaluateCellMolecules(cell_evaluator);

    ComputationalMesh::CPtr cmesh = nullptr;

    try {
        cmesh = computational_builder.build();
    }

    catch (std::logic_error const & ex) {
        std::string msg = "Failure building computational mesh!\n";
        msg += ex.what();
        Util::error(msg);
        return 1;
    }

    ComputationalMeshSolverHelper helper(*cmesh);
    helper.solve();

//    cmesh->output("mesh.out");

    return 0;
}

