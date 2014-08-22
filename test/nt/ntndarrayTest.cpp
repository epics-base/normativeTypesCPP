/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/nt.h>

using namespace epics::nt;
using namespace epics::pvData;
using std::tr1::dynamic_pointer_cast;

void test_builder()
{
    testDiag("test_builder");

    NTNDArrayBuilderPtr builder = NTNDArray::createBuilder();
    testOk(builder.get() != 0, "Got builder");

    StructureConstPtr structure = builder->
            addDescriptor()->
            addTimeStamp()->          
            addAlarm()->  
            addDisplay()->  
            createStructure();
    testOk1(structure.get() != 0);
    if (!structure)
        return;

    testOk1(NTNDArray::is_a(structure));
    testOk1(structure->getID() == NTNDArray::URI);
    testOk1(structure->getField("value").get() != 0);
    testOk1(structure->getField("compressedSize").get() != 0);
    testOk1(structure->getField("uncompressedSize").get() != 0);
    testOk1(structure->getField("codec").get() != 0);
    testOk1(structure->getField("dimension").get() != 0);
    testOk1(structure->getField("uniqueId").get() != 0);
    testOk1(structure->getField("dataTimeStamp").get() != 0);
    testOk1(structure->getField("attribute").get() != 0);
    testOk1(structure->getField("descriptor").get() != 0);
    testOk1(structure->getField("alarm").get() != 0);
    testOk1(structure->getField("timeStamp").get() != 0);
    testOk1(structure->getField("display").get() != 0);
    std::cout << *structure << std::endl;

}

MAIN(testNTNDArray) {
    testPlan(16);
    test_builder();
    return testDone();
}


