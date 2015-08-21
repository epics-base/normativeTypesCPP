/* ntndarrayAttribute.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#define epicsExportSharedSymbols
#include <pv/ntndarrayAttribute.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

static NTFieldPtr ntField = NTField::get();


StructureConstPtr NTNDArrayAttributeBuilder::createStructure()
{
    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTNDArrayAttribute::URI)->
               add("name", pvString)->
               add("value", getFieldCreate()->createVariantUnion());

    if (tags)
        builder->addArray("tags", pvString);

    //if (descriptor)
        builder->add("descriptor", pvString);

    builder->add("sourceType", pvInt);

    builder->add("source", pvString);

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

NTNDArrayAttributeBuilder::shared_pointer NTNDArrayAttributeBuilder::addTags()
{
    tags = true;
    return shared_from_this();
}

NTNDArrayAttributeBuilder::shared_pointer NTNDArrayAttributeBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTNDArrayAttributeBuilder::shared_pointer NTNDArrayAttributeBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTNDArrayAttributeBuilder::shared_pointer NTNDArrayAttributeBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTNDArrayAttributeBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTNDArrayAttributePtr NTNDArrayAttributeBuilder::create()
{
    return NTNDArrayAttributePtr(new NTNDArrayAttribute(createPVStructure()));
}

NTNDArrayAttributeBuilder::NTNDArrayAttributeBuilder()
{
    reset();
}

void NTNDArrayAttributeBuilder::reset()
{
    descriptor = false;
    alarm = false;
    timeStamp = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTNDArrayAttributeBuilder::shared_pointer NTNDArrayAttributeBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTNDArrayAttribute::URI("epics:nt/NTAttribute:1.0");

NTNDArrayAttribute::shared_pointer NTNDArrayAttribute::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTNDArrayAttribute::shared_pointer NTNDArrayAttribute::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTNDArrayAttribute(structure));
}

bool NTNDArrayAttribute::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTNDArrayAttribute::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;

    PVUnionPtr pvValue = pvStructure->getSubField<PVUnion>("value");
    if(!pvValue) return false;

    // TODO tags

    PVStringPtr pvDescriptor = pvStructure->getSubField<PVString>("descriptor");
    if(!pvDescriptor) return false;

    PVStringPtr pvSource = pvStructure->getSubField<PVString>("source");
    if(!pvSource) return false;

    PVIntPtr pvSourceType = pvStructure->getSubField<PVInt>("sourceType");
    if(!pvSourceType) return false;

    PVFieldPtr pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;

    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;

    return true;
}

NTNDArrayAttributeBuilderPtr NTNDArrayAttribute::createBuilder()
{
    return NTNDArrayAttributeBuilderPtr(new detail::NTNDArrayAttributeBuilder());
}

bool NTNDArrayAttribute::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNDArrayAttribute::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTNDArrayAttribute::getPVStructure() const
{
    return pvNTNDArrayAttribute;
}

PVStringPtr NTNDArrayAttribute::getDescriptor() const
{
    return pvNTNDArrayAttribute->getSubField<PVString>("descriptor");
}

PVStructurePtr NTNDArrayAttribute::getTimeStamp() const
{
    return pvNTNDArrayAttribute->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTNDArrayAttribute::getAlarm() const
{
    return pvNTNDArrayAttribute->getSubField<PVStructure>("alarm");
}


PVStringPtr NTNDArrayAttribute::getName() const
{
    return pvNTNDArrayAttribute->getSubField<PVString>("name");
}

PVUnionPtr NTNDArrayAttribute::getValue() const
{
    return pvValue;
}

PVStringArrayPtr NTNDArrayAttribute::getTags() const
{
    return pvNTNDArrayAttribute->getSubField<PVStringArray>("tags");
}

PVIntPtr NTNDArrayAttribute::getSourceType() const
{
    return pvNTNDArrayAttribute->getSubField<PVInt>("sourceType");
}

PVStringPtr NTNDArrayAttribute::getSource() const
{
    return pvNTNDArrayAttribute->getSubField<PVString>("source");
}

NTNDArrayAttribute::NTNDArrayAttribute(PVStructurePtr const & pvStructure) :
    pvNTNDArrayAttribute(pvStructure), pvValue(pvNTNDArrayAttribute->getSubField<PVUnion>("value"))
{
}


}}
