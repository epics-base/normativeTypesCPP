/* ntunion.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntunion.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

static NTFieldPtr ntField = NTField::get();


StructureConstPtr NTUnionBuilder::createStructure()
{
    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTUnion::URI)->
               add("value", valueType);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);


    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTUnionBuilder::shared_pointer NTUnionBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTUnionBuilder::shared_pointer NTUnionBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTUnionBuilder::shared_pointer NTUnionBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTUnionBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTUnionPtr NTUnionBuilder::create()
{
    return NTUnionPtr(new NTUnion(createPVStructure()));
}

NTUnionBuilder::NTUnionBuilder()
{
    reset();
}

void NTUnionBuilder::reset()
{
    valueType = getFieldCreate()->createVariantUnion();
    descriptor = false;
    alarm = false;
    timeStamp = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTUnionBuilder::shared_pointer NTUnionBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTUnion::URI("epics:nt/NTUnion:1.0");

NTUnion::shared_pointer NTUnion::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTUnion::shared_pointer NTUnion::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTUnion(structure));
}

bool NTUnion::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTUnion::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;

    PVUnionPtr pvValue = pvStructure->getSubField<PVUnion>("value");
    if(!pvValue) return false;

    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;

    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;

    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;

    return true;
}

NTUnionBuilderPtr NTUnion::createBuilder()
{
    return NTUnionBuilderPtr(new detail::NTUnionBuilder());
}

bool NTUnion::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTUnion::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTUnion::getPVStructure() const
{
    return pvNTUnion;
}

PVStringPtr NTUnion::getDescriptor() const
{
    return pvNTUnion->getSubField<PVString>("descriptor");
}

PVStructurePtr NTUnion::getTimeStamp() const
{
    return pvNTUnion->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTUnion::getAlarm() const
{
    return pvNTUnion->getSubField<PVStructure>("alarm");
}

PVUnionPtr NTUnion::getValue() const
{
    return pvValue;
}

NTUnion::NTUnion(PVStructurePtr const & pvStructure) :
    pvNTUnion(pvStructure), pvValue(pvNTUnion->getSubField<PVUnion>("value"))
{}


}}
