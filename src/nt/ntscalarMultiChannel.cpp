/* ntscalarMultiChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>

#define epicsExportSharedSymbols
#include <pv/ntscalarMultiChannel.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt { 


static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();

namespace detail {

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::value(ScalarType scalarType)
{
    valueType = scalarType;
    return shared_from_this();
}


NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addSeverity()
{
    severity = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addStatus()
{
    status = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addMessage()
{
    message = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addSecondsPastEpoch()
{
    secondsPastEpoch = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addNanoseconds()
{
    nanoseconds = true;
    return shared_from_this();
}

NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::addUserTag()
{
    userTag = true;
    return shared_from_this();
}

StructureConstPtr NTScalarMultiChannelBuilder::createStructure()
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 3;
    size_t extraCount = extraFieldNames.size();
    nfields += extraCount;
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
    fields[ind++] =  fieldCreate->createScalarArray(valueType);
    names[ind] = "channelName";
    fields[ind++] =  fieldCreate->createScalarArray(pvString);
    names[ind] = "isConnected";
    fields[ind++] =  fieldCreate->createScalarArray(pvBoolean);
    if(descriptor) {
        names[ind] = "descriptor";
        fields[ind++] = fieldCreate->createScalar(pvString);
    }
    if(alarm) {
        names[ind] = "alarm";
        fields[ind++] = standardField->alarm();
    }
    if(timeStamp) {
        names[ind] = "timeStamp";
        fields[ind++] = standardField->timeStamp();
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
    for (size_t i = 0; i< extraCount; i++) {
        names[ind] = extraFieldNames[i];
        fields[ind++] = extraFields[i];
    }

    StructureConstPtr st = fieldCreate->createStructure(NTScalarMultiChannel::URI,names,fields);
    reset();
    return st;
}

PVStructurePtr NTScalarMultiChannelBuilder::createPVStructure()
{
    return pvDataCreate->createPVStructure(createStructure());
}

NTScalarMultiChannelPtr NTScalarMultiChannelBuilder::create()
{
    return NTScalarMultiChannelPtr(new NTScalarMultiChannel(createPVStructure()));
}

NTScalarMultiChannelBuilder::NTScalarMultiChannelBuilder()
: valueType(pvDouble)
{
    reset();
}

void NTScalarMultiChannelBuilder::reset()
{
    extraFieldNames.clear();
    extraFields.clear();
    valueType = pvDouble;
    descriptor = false;
    alarm = false;
    timeStamp = false;
    severity = false;
    status = false;
    message = false;
    secondsPastEpoch = false;
    nanoseconds = false;
    userTag = false;
}


NTScalarMultiChannelBuilder::shared_pointer NTScalarMultiChannelBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTScalarMultiChannel::URI("epics:nt/NTScalarMultiChannel:1.0");

NTScalarMultiChannel::shared_pointer NTScalarMultiChannel::wrap(PVStructurePtr const & structure)
{
    if(!isCompatible(structure)) return shared_pointer();
    return wrapUnsafe(structure);
}

NTScalarMultiChannel::shared_pointer NTScalarMultiChannel::wrapUnsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTScalarMultiChannel(structure));
}

bool NTScalarMultiChannel::is_a(StructureConstPtr const &structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTScalarMultiChannel::isCompatible(PVStructurePtr const &pvStructure)
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
    pvField = pvStructure->getSubField("severity");
    if(pvField && !pvStructure->getSubField<PVIntArray>("severity")) return false;
    pvField = pvStructure->getSubField("status");
    if(pvField && !pvStructure->getSubField<PVIntArray>("status")) return false;
    pvField = pvStructure->getSubField("message");
    if(pvField && !pvStructure->getSubField<PVStringArray>("message")) return false;
    pvField = pvStructure->getSubField("secondsPastEpoch");
    if(pvField && !pvStructure->getSubField<PVLongArray>("secondsPastEpoch")) return false;
    pvField = pvStructure->getSubField("nanoseconds");
    if(pvField && !pvStructure->getSubField<PVIntArray>("nanoseconds")) return false;
    pvField = pvStructure->getSubField("userTag");
    if(pvField && !pvStructure->getSubField<PVIntArray>("userTag")) return false;
    return true;
}

NTScalarMultiChannelBuilderPtr NTScalarMultiChannel::createBuilder()
{
    return NTScalarMultiChannelBuilderPtr(new detail::NTScalarMultiChannelBuilder());
}


NTScalarMultiChannel::NTScalarMultiChannel(PVStructurePtr const & pvStructure)
: pvNTScalarMultiChannel(pvStructure),
  pvTimeStamp(pvStructure->getSubField<PVStructure>("timeStamp")),
  pvAlarm(pvStructure->getSubField<PVStructure>("alarm")),
  pvValue(pvStructure->getSubField<PVScalarArray>("value")),
  pvChannelName(pvStructure->getSubField<PVStringArray>("channelName")),
  pvIsConnected(pvStructure->getSubField<PVBooleanArray>("isConnected")),
  pvSeverity(pvStructure->getSubField<PVIntArray>("severity")),
  pvStatus(pvStructure->getSubField<PVIntArray>("status")),
  pvMessage(pvStructure->getSubField<PVStringArray>("message")),
  pvSecondsPastEpoch(pvStructure->getSubField<PVLongArray>("secondsPastEpoch")),
  pvNanoseconds(pvStructure->getSubField<PVIntArray>("nanoseconds")),
  pvUserTag(pvStructure->getSubField<PVIntArray>("userTag")),
  pvDescriptor(pvStructure->getSubField<PVString>("descriptor"))
{
}


void  NTScalarMultiChannel::attachTimeStamp(PVTimeStamp &pv) const
{
    if(!pvTimeStamp) return;
    pv.attach(pvTimeStamp);
}

void  NTScalarMultiChannel::attachAlarm(PVAlarm &pv) const
{
    if(!pvAlarm) return;
    pv.attach(pvAlarm);
}

}}
