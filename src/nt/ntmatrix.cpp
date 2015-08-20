/* ntmatrix.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntmatrix.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

StructureConstPtr NTMatrixBuilder::createStructure()
{
    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTMatrix::URI)->
               addArray("value", pvDouble);

    if (dim)
        builder->addArray("dim", pvInt);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    if (display)
        builder->add("display", ntField->createDisplay());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);


    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::addDim()
{
    dim = true;
    return shared_from_this();
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::addDisplay()
{
    display = true;
    return shared_from_this();
}

PVStructurePtr NTMatrixBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTMatrixPtr NTMatrixBuilder::create()
{
    return NTMatrixPtr(new NTMatrix(createPVStructure()));
}

NTMatrixBuilder::NTMatrixBuilder()
{
    reset();
}

void NTMatrixBuilder::reset()
{
    dim = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    display = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTMatrixBuilder::shared_pointer NTMatrixBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTMatrix::URI("epics:nt/NTMatrix:1.0");

NTMatrix::shared_pointer NTMatrix::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTMatrix::shared_pointer NTMatrix::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTMatrix(structure));
}

bool NTMatrix::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTMatrix::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;
    PVScalarArrayPtr pvValue = pvStructure->getSubField<PVScalarArray>("value");
    if(!pvValue) return false;
    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    pvField = pvStructure->getSubField("display");
    if(pvField && !ntField->isDisplay(pvField->getField())) return false;
    pvField = pvStructure->getSubField("control");
    if(pvField && !ntField->isControl(pvField->getField())) return false;
    return true;
}


NTMatrixBuilderPtr NTMatrix::createBuilder()
{
    return NTMatrixBuilderPtr(new detail::NTMatrixBuilder());
}

bool NTMatrix::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTMatrix::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

bool NTMatrix::attachDisplay(PVDisplay &pvDisplay) const
{
    PVStructurePtr dp = getDisplay();
    if (dp)
        return pvDisplay.attach(dp);
    else
        return false;
}

PVStructurePtr NTMatrix::getPVStructure() const
{
    return pvNTMatrix;
}

PVStringPtr NTMatrix::getDescriptor() const
{
    return pvNTMatrix->getSubField<PVString>("descriptor");
}

PVStructurePtr NTMatrix::getTimeStamp() const
{
    return pvNTMatrix->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTMatrix::getAlarm() const
{
    return pvNTMatrix->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTMatrix::getDisplay() const
{
    return pvNTMatrix->getSubField<PVStructure>("display");
}

PVDoubleArrayPtr NTMatrix::getValue() const
{
    return pvValue;
}

PVIntArrayPtr NTMatrix::getDim() const
{
    return pvNTMatrix->getSubField<PVIntArray>("dim");
}

NTMatrix::NTMatrix(PVStructurePtr const & pvStructure) :
    pvNTMatrix(pvStructure),
    pvValue(pvNTMatrix->getSubField<PVDoubleArray>("value"))
{}


}}
