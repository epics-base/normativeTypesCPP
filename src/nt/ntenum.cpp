/* ntenum.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntenum.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

static NTFieldPtr ntField = NTField::get();


StructureConstPtr NTEnumBuilder::createStructure()
{
    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTEnum::URI)->
               add("value", ntField->createEnumerated());

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

NTEnumBuilder::shared_pointer NTEnumBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTEnumBuilder::shared_pointer NTEnumBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTEnumBuilder::shared_pointer NTEnumBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTEnumBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTEnumPtr NTEnumBuilder::create()
{
    return NTEnumPtr(new NTEnum(createPVStructure()));
}

NTEnumBuilder::NTEnumBuilder()
{
    reset();
}

void NTEnumBuilder::reset()
{
    descriptor = false;
    alarm = false;
    timeStamp = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTEnumBuilder::shared_pointer NTEnumBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTEnum::URI("epics:nt/NTEnum:1.0");

NTEnum::shared_pointer NTEnum::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTEnum::shared_pointer NTEnum::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTEnum(structure));
}

bool NTEnum::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTEnum::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;
    PVStructurePtr pvValue = pvStructure->getSubField<PVStructure>("value");
    if(!pvValue) return false;
    if (!ntField->isEnumerated(pvValue->getField())) return false;

    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;

    return true;
}

NTEnumBuilderPtr NTEnum::createBuilder()
{
    return NTEnumBuilderPtr(new detail::NTEnumBuilder());
}

bool NTEnum::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTEnum::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTEnum::getPVStructure() const
{
    return pvNTEnum;
}

PVStringPtr NTEnum::getDescriptor() const
{
    return pvNTEnum->getSubField<PVString>("descriptor");
}

PVStructurePtr NTEnum::getTimeStamp() const
{
    return pvNTEnum->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTEnum::getAlarm() const
{
    return pvNTEnum->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTEnum::getValue() const
{
    return pvValue;
}

NTEnum::NTEnum(PVStructurePtr const & pvStructure) :
    pvNTEnum(pvStructure), pvValue(pvNTEnum->getSubField<PVStructure>("value"))
{}


}}
