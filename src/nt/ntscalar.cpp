/* ntscalar.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntscalar.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

namespace detail {

static NTFieldPtr ntField = NTField::get();

NTScalarBuilder::shared_pointer NTScalarBuilder::value(
        epics::pvData::ScalarType scalarType
        )
{
    isArray = false;
    valueType = scalarType;
    valueTypeSet = true;

    return shared_from_this();
}

NTScalarBuilder::shared_pointer NTScalarBuilder::arrayValue(
        epics::pvData::ScalarType scalarType
        )
{
    isArray = true;
    valueType = scalarType;
    valueTypeSet = true;

    return shared_from_this();
}

StructureConstPtr NTScalarBuilder::createStructure()
{
    if (!valueTypeSet)
        throw std::runtime_error("value type not set");

    FieldBuilderPtr builder =
            getFieldCreate()->createFieldBuilder()->
               setId(NTScalar::URI)->
               add("value", valueType);

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    if (display)
        builder->add("display", ntField->createDisplay());

    if (control)
        builder->add("control", ntField->createControl());

    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTScalarBuilder::shared_pointer NTScalarBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTScalarBuilder::shared_pointer NTScalarBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTScalarBuilder::shared_pointer NTScalarBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTScalarBuilder::shared_pointer NTScalarBuilder::addDisplay()
{
    display = true;
    return shared_from_this();
}

NTScalarBuilder::shared_pointer NTScalarBuilder::addControl()
{
    control = true;
    return shared_from_this();
}

PVStructurePtr NTScalarBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTScalarPtr NTScalarBuilder::create()
{
    return NTScalarPtr(new NTScalar(createPVStructure()));
}

NTScalarBuilder::NTScalarBuilder()
{
    reset();
}

void NTScalarBuilder::reset()
{
    valueTypeSet = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    display = false;
    control = false;
}

}

const std::string NTScalar::URI("uri:ev4:nt/2012/pwd:NTScalar");

bool NTScalar::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
}

NTScalarBuilderPtr NTScalar::createBuilder()
{
    return NTScalarBuilderPtr(new detail::NTScalarBuilder());
}

bool NTScalar::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTScalar::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

bool NTScalar::attachDisplay(PVDisplay &pvDisplay) const
{
    PVStructurePtr dp = getDisplay();
    if (dp)
        return pvDisplay.attach(dp);
    else
        return false;
}

bool NTScalar::attachControl(PVControl &pvControl) const
{
    PVStructurePtr ctrl = getControl();
    if (ctrl)
        return pvControl.attach(ctrl);
    else
        return false;
}

PVStructurePtr NTScalar::getPVStructure() const
{
    return pvNTScalar;
}

PVStringPtr NTScalar::getDescriptor() const
{
    return pvNTScalar->getSubField<PVString>("descriptor");
}

PVStructurePtr NTScalar::getTimeStamp() const
{
    return pvNTScalar->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTScalar::getAlarm() const
{
    return pvNTScalar->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTScalar::getDisplay() const
{
    return pvNTScalar->getSubField<PVStructure>("display");
}

PVStructurePtr NTScalar::getControl() const
{
    return pvNTScalar->getSubField<PVStructure>("control");
}

PVFieldPtr NTScalar::getValue() const
{
    return pvNTScalar->getSubField("value");
}

NTScalar::NTScalar(PVStructurePtr const & pvStructure) :
    pvNTScalar(pvStructure)
{}


}}
