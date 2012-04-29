#include "ComputationalVariableManagerTest.h"

#include "FiniteVolume2D/ComputationalVariableManager.h"
#include "FiniteVolume2D/ComputationalCell.h"
#include "FiniteVolume2D/ComputationalFace.h"

#include "FiniteVolume2DLib/CellManager.h"
#include "FiniteVolume2DLib/Cell.h"
#include "FiniteVolume2DLib/Face.h"


void
ComputationalVariableManagerTest::setUp() {
}

void
ComputationalVariableManagerTest::tearDown() {
}

namespace {
    bool dummy(ComputationalGridAccessor const & cgrid, ComputationalCell::Ptr const & cell, ComputationalFace::Ptr & face) {
        return false;
    }

}

void
ComputationalVariableManagerTest::basicUsageTest() {
    ComputationalVariableManager cvar_mgr;

    CellManager::Ptr geometric_cell_mgr = CellManager::create();
    Cell::Ptr cell = geometric_cell_mgr->createCell(0, EntityCollection<Face>());

    ComputationalCell::Ptr ccell(new ComputationalCell(cell, EntityCollection<ComputationalFace>()));

    bool success = cvar_mgr.registerVariable("Temperature", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);    

    success = cvar_mgr.registerVariable("Pressure", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);

    ComputationalVariable::Ptr cvar = cvar_mgr.create(ccell, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Creating comp. variable failed", bool(cvar) == true);
}

void
ComputationalVariableManagerTest::createBeforeRegisterTest() {
    ComputationalVariableManager cvar_mgr;

    CellManager::Ptr geometric_cell_mgr = CellManager::create();
    Cell::Ptr cell = geometric_cell_mgr->createCell(0, EntityCollection<Face>());

    ComputationalCell::Ptr ccell(new ComputationalCell(cell, EntityCollection<ComputationalFace>()));

    ComputationalVariable::Ptr cvar = cvar_mgr.create(ccell, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Creating a comp. variable without prior registration should fail", bool(cvar) == false);
}

void
ComputationalVariableManagerTest::registerAfterCreateTest() {
    ComputationalVariableManager cvar_mgr;

    CellManager::Ptr geometric_cell_mgr = CellManager::create();
    Cell::Ptr cell = geometric_cell_mgr->createCell(0, EntityCollection<Face>());

    ComputationalCell::Ptr ccell(new ComputationalCell(cell, EntityCollection<ComputationalFace>()));

    bool success = cvar_mgr.registerVariable("Temperature", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);    

    ComputationalVariable::Ptr cvar = cvar_mgr.create(ccell, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Creating comp. variable failed", bool(cvar) == true);


    success = cvar_mgr.registerVariable("Pressure", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable should have failed", false, success);    
}

void
ComputationalVariableManagerTest::cellHasComputationalVariableTest() {
    ComputationalVariableManager cvar_mgr;

    CellManager::Ptr geometric_cell_mgr = CellManager::create();
    Cell::Ptr cell = geometric_cell_mgr->createCell(0, EntityCollection<Face>());

    ComputationalCell::Ptr ccell(new ComputationalCell(cell, EntityCollection<ComputationalFace>()));

    bool success = cvar_mgr.registerVariable("Temperature", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);    

    success = cvar_mgr.registerVariable("Pressure", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);

    ComputationalVariable::Ptr cvar = cvar_mgr.create(ccell, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Creating comp. variable failed", bool(cvar) == true);

    CPPUNIT_ASSERT_MESSAGE("Computational variable not found in cell", cvar == ccell->getComputationalVariable("Temperature"));   
}

void
ComputationalVariableManagerTest::iteratorTest() {
    /* In order to implement the iterator, I used TDD in the following sense.
     * Before I even started coding the iterator class, I used to iterator
     * as if it were there. This way, I hope to extract the public interface
     * in a natural way. This helps me in doing the OOA and OOD.
     */
    ComputationalVariableManager cvar_mgr;

    CellManager::Ptr geometric_cell_mgr = CellManager::create();
    Cell::Ptr cell = geometric_cell_mgr->createCell(0, EntityCollection<Face>());

    ComputationalCell::Ptr ccell(new ComputationalCell(cell, EntityCollection<ComputationalFace>()));

    bool success = cvar_mgr.registerVariable("Temperature", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);    

    success = cvar_mgr.registerVariable("Pressure", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);

    success = cvar_mgr.registerVariable("Momentum_X", dummy);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Registering comp. variable failed", true, success);

    ComputationalVariable::Ptr cvar = cvar_mgr.create(ccell, "Temperature");
    CPPUNIT_ASSERT_MESSAGE("Creating comp. variable failed", bool(cvar) == true);


    // create an iterator to iterate over all elements
    ComputationalVariableManager::Iterator_t it(cvar_mgr.begin());

    std::map<std::string, short> var_map;
    ComputationalVariableManager::size_type cnt = 0;

    while (it != cvar_mgr.end()) {
        std::string const & cvar_name = it->name;
        ComputationalVariableManager::FluxEvaluator_t const & flux_eval = it->flux_eval;

        ComputationalVariableManager::Iterator_t::Item_t const & item = *it;

        var_map[cvar_name] = 1;

        ++it;
        cnt++;
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of elements", cvar_mgr.size(), cnt);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Wrong number of elements", var_map.size(), 3u);
}
