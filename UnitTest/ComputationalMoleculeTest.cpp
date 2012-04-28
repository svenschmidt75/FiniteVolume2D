#include "ComputationalMoleculeTest.h"

void
ComputationalMoleculeTest::addMoleculesTest() {
    ComputationalMolecule m1;
    m1.add(0, 1.0);
    m1.add(1, 2.0);
    m1.add(4, -1.0);
    m1.add(6, -1.0);

    ComputationalMolecule m2;
    m2.add(0, 4.0);
    m2.add(1, 2.2);
    m2.add(3, -1.0);
    m2.add(6, -5.2);

    ComputationalMolecule m = m1 + m2;
    Assert(m.get(0) == 5.0);
    Assert(m.get(1) == 4.2);
    Assert(m.get(3) == -1.0);
    Assert(m.get(6) == -6.2);
    Assert(m.size() == 5);
}

void
ComputationalMoleculeTest::addFluxMoleculesTest() {
    FluxComputationalMolecule m1;
    m1.setCell(cell1);

    m1.add(0, 1.0);
    m1.add(1, 2.0);
    m1.add(4, -1.0);
    m1.add(6, -1.0);

    FluxComputationalMolecule m2;
    m2.setCell(cell2);

    m2.add(0, 4.0);
    m2.add(1, 2.2);
    m2.add(3, -1.0);
    m2.add(6, -5.2);

    ComputationalMolecule m = m1 + m2;
    Assert(m.get(0) == 5.0);
    Assert(m.get(1) == 4.2);
    Assert(m.get(3) == -1.0);
    Assert(m.get(6) == -6.2);
    Assert(m.size() == 5);
}
