/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/ntvalidator.h>
#include <pv/ntndarray.h>

using namespace epics::nt;
using epics::pvData::StructureConstPtr;
using epics::pvData::Field;

void test_isCompatible()
{
    testDiag("test_isCompatible");

    StructureConstPtr ref(NTNDArray::createBuilder()->addAlarm()->createStructure());

    Validator::Definition def;
    def.structure = std::static_pointer_cast<const Field>(ref);
    def.optional.insert(ref->getField("alarm").get());

    StructureConstPtr struc(NTNDArray::createBuilder()->createStructure());

    testOk1(Validator::isCompatible(def, struc));
}

MAIN(testNTValidator) {
    testPlan(1);
    test_isCompatible();
    return testDone();
}


