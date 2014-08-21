/* ntnameValue.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntnameValue.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

namespace detail {

static NTFieldPtr ntField = NTField::get();

NTNameValueBuilder::shared_pointer NTNameValueBuilder::value(
        epics::pvData::ScalarType scalarType
        )
{
    valueType = scalarType;
    valueTypeSet = true;

    return shared_from_this();
}

StructureConstPtr NTNameValueBuilder::createStructure()
{
    if (!valueTypeSet)
        throw std::runtime_error("value type not set");

    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTNameValue::URI)->
               addArray("names", pvString)->
               addArray("values", valueType);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTNameValueBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTNameValuePtr NTNameValueBuilder::create()
{
    return NTNameValuePtr(new NTNameValue(createPVStructure()));
}

NTNameValueBuilder::NTNameValueBuilder()
{
    reset();
}

void NTNameValueBuilder::reset()
{
    valueTypeSet = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
}

}

const std::string NTNameValue::URI("uri:ev4:nt/2012/pwd:NTNameValue");

bool NTNameValue::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
}

NTNameValueBuilderPtr NTNameValue::createBuilder()
{
    return NTNameValueBuilderPtr(new detail::NTNameValueBuilder());
}

bool NTNameValue::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNameValue::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTNameValue::getPVStructure() const
{
    return pvNTNameValue;
}

PVStringPtr NTNameValue::getDescriptor() const
{
    return pvNTNameValue->getSubField<PVString>("descriptor");
}

PVStructurePtr NTNameValue::getTimeStamp() const
{
    return pvNTNameValue->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTNameValue::getAlarm() const
{
    return pvNTNameValue->getSubField<PVStructure>("alarm");
}

PVStringArrayPtr NTNameValue::getNames() const
{
    return pvNTNameValue->getSubField<PVStringArray>("names");
}

PVFieldPtr NTNameValue::getValues() const
{
    return pvNTNameValue->getSubField("values");
}

NTNameValue::NTNameValue(PVStructurePtr const & pvStructure) :
    pvNTNameValue(pvStructure)
{}


}}
