/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

#include <epicsUnitTest.h>
#include <testMain.h>

#include "../src/validator.h"
#include <pv/ntndarray.h>
#include <pv/pvIntrospect.h>

using namespace epics::nt;
using epics::pvData::StructureConstPtr;
using epics::pvData::Field;
using epics::pvData::ScalarType;
using epics::pvData::FieldConstPtr;
using epics::pvData::FieldBuilder;
using epics::pvData::FieldBuilderPtr;

static epics::pvData::FieldCreatePtr FC;

void test_result()
{
    testDiag("test_result");

    Validator::Result result;
    testOk(result.valid(), "Result with no errors means valid");

    result.errors.push_back(Validator::Error("a.b.c", Validator::ErrorType::MISSING_FIELD));
    testOk(!result.valid(), "Result with one error means invalid");
}

void test_scalar()
{
    testDiag("test_scalar");

    {
        FieldConstPtr def(FC->createScalar(ScalarType::pvBoolean));
        FieldConstPtr field(FC->createScalar(ScalarType::pvBoolean));
        testOk(Validator(def).isCompatible(*field), "isCompatible(Scalar<bool>, Scalar<bool>)");
    }

    {
        FieldConstPtr def(FC->createScalar(ScalarType::pvBoolean));
        FieldConstPtr field(FC->createScalar(ScalarType::pvInt));
        testOk(Validator(def).isCompatible(*field), "isCompatible(Scalar<bool>, Scalar<int>)");
    }

    {
        FieldConstPtr def(FC->createScalar(ScalarType::pvString));
        FieldConstPtr field(FC->createScalar(ScalarType::pvFloat));
        testOk(Validator(def).isCompatible(*field), "isCompatible(Scalar<string>, Scalar<float>)");
    }

    {
        FieldConstPtr def(FC->createScalarArray(ScalarType::pvString));
        FieldConstPtr field(FC->createScalarArray(ScalarType::pvFloat));
        testOk(Validator(def).isCompatible(*field), "isCompatible(ScalarArray<string>, ScalarArray<float>)");
    }

    {
        FieldConstPtr def(FC->createScalarArray(ScalarType::pvByte));
        FieldConstPtr field(FC->createScalar(ScalarType::pvByte));
        testOk(!Validator(def).isCompatible(*field), "!isCompatible(ScalarArray<byte>, Scalar<byte>)");
    }

    {
        FieldConstPtr def(FC->createScalar(ScalarType::pvByte));
        FieldConstPtr field(FC->createScalarArray(ScalarType::pvByte));
        testOk(!Validator(def).isCompatible(*field), "!isCompatible(Scalar<byte>, ScalarArray<byte>)");
    }
}

void test_union()
{
    testDiag("test_union");
    FieldBuilderPtr FB(FieldBuilder::begin());

    FieldConstPtr unionVar(FC->createVariantUnion());
    FieldConstPtr unionABC(FB->
        add("A", ScalarType::pvInt)->
        add("B", ScalarType::pvInt)->
        add("C", ScalarType::pvInt)->
        createUnion());

    FieldConstPtr unionABC2(FB->
        add("A", ScalarType::pvFloat)->
        add("B", ScalarType::pvDouble)->
        add("C", ScalarType::pvString)->
        createUnion());

    FieldConstPtr unionBAC(FB->
        add("B", ScalarType::pvInt)->
        add("A", ScalarType::pvInt)->
        add("C", ScalarType::pvInt)->
        createUnion());

    FieldConstPtr unionAB(FB->
        add("A", ScalarType::pvInt)->
        add("B", ScalarType::pvInt)->
        createUnion());

    FieldConstPtr unionNested1(FB->
        add("A", unionABC)->
        add("B", unionABC)->
        createUnion());

    FieldConstPtr unionNested2(FB->
        add("A", unionABC)->
        add("B", unionBAC)->
        createUnion());

    testOk(Validator(unionVar).isCompatible(*unionVar),
        "isCompatible(VarUnion, VarUnion)");
    testOk(!Validator(unionVar).isCompatible(*unionABC),
        "!isCompatible(VarUnion, Union{A:int, B:int, C:int})");
    testOk(!Validator(unionABC).isCompatible(*unionVar),
        "!isCompatible(Union{A:int, B:int, C:int}, VarUnion)");
    testOk(Validator(unionABC).isCompatible(*unionABC),
        "isCompatible(Union{A:int, B:int, C:int}, Union{A:int, B:int, C:int})");

    testOk(Validator(unionABC).isCompatible(*unionABC2),
        "isCompatible(Union{A:int, B:int, C:int}, Union{A:float, B:double, C:string})");
    testOk(Validator(unionABC2).isCompatible(*unionABC),
        "isCompatible(Union{A:float, B:double, C:string}, Union{A:int, B:int, C:int})");

    testOk(!Validator(unionABC).isCompatible(*unionBAC),
        "!isCompatible(Union{A:int, B:int, C:int}, Union{B:int, A:int, C:int})");

    testOk(Validator(unionAB).isCompatible(*unionABC),  "Extra Union field");
    testOk(!Validator(unionABC).isCompatible(*unionAB), "Missing Union field");

    {
        Validator::Result result = Validator(unionABC).validate(*unionAB);
        testOk(result.errors.size() == 1 &&
               result.errors[0] == Validator::Error("C", Validator::ErrorType::MISSING_FIELD),
               "Missing Union field Error");
    }

    testOk(!Validator(unionNested1).isCompatible(*unionNested2), "Nested Unions");
}

void test_isCompatible()
{
    testDiag("test_isCompatible");

    std::set<Field const *> opt;
    StructureConstPtr ref(NTNDArray::createBuilder()->addAlarm()->createStructure());
    opt.insert(ref->getField("alarm").get());

    StructureConstPtr struc(NTNDArray::createBuilder()->createStructure());

    testOk1(Validator(ref, opt).isCompatible(*struc));
}

MAIN(testValidator) {
    testPlan(0);
    FC = epics::pvData::getFieldCreate();
    test_result();
    test_scalar();
    test_union();
    test_isCompatible();
    return testDone();
}


