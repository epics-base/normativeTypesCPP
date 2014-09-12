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
static NTFieldPtr ntField = NTField::get();

namespace detail {


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

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);

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
    extraFieldNames.clear();
    extraFields.clear();
}

NTNameValueBuilder::shared_pointer NTNameValueBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTNameValue::URI("uri:ev4:nt/2014/pwd:NTNameValue");

NTNameValue::shared_pointer NTNameValue::narrow(PVStructurePtr const & structure)
{
    if (!structure || !is_a(structure->getStructure()))
        return shared_pointer();

    return narrow_unsafe(structure);
}

NTNameValue::shared_pointer NTNameValue::narrow_unsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTNameValue(structure));
}

bool NTNameValue::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
}

bool NTNameValue::is_compatible(PVStructurePtr const & pvStructure)
{
    PVStringArrayPtr pvNames = pvStructure->getSubField<PVStringArray>("names");
    if(!pvNames) return false;
    PVFieldPtr pvValues = pvStructure->getSubField("values");
    if(!pvValues) return false;
    PVFieldPtr pvField = pvStructure->getSubField("descriptor");
    if(pvField && !pvStructure->getSubField<PVString>("descriptor")) return false;
    pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    return true;
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
