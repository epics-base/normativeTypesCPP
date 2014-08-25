/* ntmultiChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntmultiChannel.h>
#include <algorithm>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt { 


static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

namespace detail {

static NTFieldPtr ntField = NTField::get();

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addValue(UnionConstPtr valuePtr)
{
    value = true;
    valueUnion = valuePtr;
    return shared_from_this();
}


NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addSeverity()
{
    severity = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addStatus()
{
    status = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addMessage()
{
    message = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addSecondsPastEpoch()
{
    secondsPastEpoch = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addNanoseconds()
{
    nanoseconds = true;
    return shared_from_this();
}

NTMultiChannelBuilder::shared_pointer NTMultiChannelBuilder::addUserTag()
{
    userTag = true;
    return shared_from_this();
}

StructureConstPtr NTMultiChannelBuilder::createStructure()
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 3;
    if(descriptor) ++nfields;
    if(alarm) ++nfields;
    if(timeStamp) ++nfields;
    if(severity) ++nfields;
    if(status) ++nfields;
    if(message) ++nfields;
    if(secondsPastEpoch) ++nfields;
    if(nanoseconds) ++nfields;
    if(userTag) ++nfields;
    FieldConstPtrArray fields(nfields);
    StringArray names(nfields);
    size_t ind = 0;
    names[ind] = "value";
    if(value) {
        fields[ind++] =  fieldCreate->createUnionArray(valueUnion);
    } else {
        fields[ind++] =  fieldCreate->createVariantUnion();
    }
    names[ind] = "channelName";
    fields[ind++] =  fieldCreate->createScalarArray(pvString);
    names[ind] = "isConnected";
    fields[ind++] =  fieldCreate->createScalarArray(pvBoolean);
    if(timeStamp) {
        names[ind] = "timeStamp";
        fields[ind++] = standardField->timeStamp();
    }
    if(alarm) {
        names[ind] = "alarm";
        fields[ind++] = standardField->alarm();
    }
    if(descriptor) {
        names[ind] = "descriptor";
        fields[ind++] = fieldCreate->createScalar(pvString);
    }
    if(severity) {
        names[ind] = "severity";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(status) {
        names[ind] = "status";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(message) {
        names[ind] = "message";
        fields[ind++] = fieldCreate->createScalarArray(pvString);
    }
    if(secondsPastEpoch) {
        names[ind] = "secondsPastEpoch";
        fields[ind++] = fieldCreate->createScalarArray(pvLong);
    }
    if(nanoseconds) {
        names[ind] = "nanoseconds";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(userTag) {
        names[ind] = "userTag";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    StructureConstPtr st = fieldCreate->createStructure(NTMultiChannel::URI,names,fields);
    reset();
    return st;
}

PVStructurePtr NTMultiChannelBuilder::NTMultiChannelBuilder::createPVStructure()
{
    return pvDataCreate->createPVStructure(createStructure());
}

NTMultiChannelPtr NTMultiChannelBuilder::create()
{
    return NTMultiChannelPtr(new NTMultiChannel(createPVStructure()));
}

NTMultiChannelBuilder::NTMultiChannelBuilder()
{
    reset();
}

void NTMultiChannelBuilder::reset()
{
    valueUnion.reset();
    value = false;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    severity = false;
    status = false;
    message = false;
    secondsPastEpoch = false;
    nanoseconds = false;
}

}

const std::string NTMultiChannel::URI("uri:ev4:nt/2012/pwd:NTMultiChannel");

bool NTMultiChannel::is_a(StructureConstPtr const &structure)
{
    return structure->getID() == URI;
}

NTMultiChannelBuilderPtr NTMultiChannel::createBuilder()
{
    return NTMultiChannelBuilderPtr(new detail::NTMultiChannelBuilder());
}


NTMultiChannel::NTMultiChannel(PVStructurePtr const & pvStructure)
: pvNTMultiChannel(pvStructure),
  pvTimeStamp(pvStructure->getSubField<PVStructure>("timeStamp")),
  pvAlarm(pvStructure->getSubField<PVStructure>("alarm")),
  pvValue(pvStructure->getSubField<PVUnionArray>("value")),
  pvChannelName(pvStructure->getSubField<PVStringArray>("channelName")),
  pvIsConnected(pvStructure->getSubField<PVBooleanArray>("isConnected")),
  pvSeverity(pvStructure->getSubField<PVIntArray>("severity")),
  pvStatus(pvStructure->getSubField<PVIntArray>("status")),
  pvMessage(pvStructure->getSubField<PVStringArray>("message")),
  pvSecondsPastEpoch(pvStructure->getSubField<PVLongArray>("secondsPastEpoch")),
  pvNanoseconds(pvStructure->getSubField<PVIntArray>("nanoseconds")),
  pvDescriptor(pvStructure->getSubField<PVString>("descriptor"))
{
}


void  NTMultiChannel::attachTimeStamp(PVTimeStamp &pv)
{
    if(!pvTimeStamp) return;
    pv.attach(pvTimeStamp);
}

void  NTMultiChannel::attachAlarm(PVAlarm &pv)
{
    if(!pvAlarm) return;
    pv.attach(pvAlarm);
}

}}
