/* ntmultiChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntmultiChannel.h>

namespace epics { namespace nt { 

using namespace epics::pvData;
using std::tr1::static_pointer_cast;
using std::string;
using std::cout;
using std::endl;

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

bool NTMultiChannel::isNTMultiChannel(PVStructurePtr const & pvStructure)
{
    PVUnionArrayPtr pvValue = pvStructure->getSubField<PVUnionArray>("value");
    if(!pvValue) return false;
    PVStringArrayPtr pvChannelName =
        pvStructure->getSubField<PVStringArray>("channelName");
    return true;
}

NTMultiChannelPtr NTMultiChannel::create(
    std::vector<std::string> const & optionNames)
{
     return NTMultiChannel::create(optionNames,fieldCreate->createVariantUnion());
}

NTMultiChannelPtr NTMultiChannel::create(
    std::vector<std::string> const & optionNames,
    epics::pvData::UnionConstPtr const & unionPtr)
{
    shared_vector<const std::string> channelNames;
    return NTMultiChannel::create(optionNames,unionPtr,channelNames);
}

NTMultiChannelPtr NTMultiChannel::create(
    std::vector<std::string> const & optionNames,
    UnionConstPtr const & unionPtr,
    shared_vector<const std::string> channelNames)
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 2;
    bool hasAlarm = false;
    bool hasTimeStamp = false;
    bool hasSeverity = false;
    bool hasStatus = false;
    bool hasMessage = false;
    bool hasSecondsPastEpoch = false;
    bool hasNanoseconds = false;
    bool hasDescriptor = false;
    for(size_t i=0; i<optionNames.size(); ++i) {
        string name = optionNames[i];
        if(name.compare("alarm")==0) {hasAlarm = true; ++nfields;}
        if(name.compare("timeStamp")==0) {hasTimeStamp = true; ++nfields;}
        if(name.compare("severity")==0) {hasSeverity = true; ++nfields;}
        if(name.compare("status")==0) {hasStatus = true; ++nfields;}
        if(name.compare("message")==0) {hasMessage = true; ++nfields;}
        if(name.compare("secondsPastEpoch")==0) {hasSecondsPastEpoch = true; ++nfields;}
        if(name.compare("nanoseconds`")==0) {hasNanoseconds = true; ++nfields;}
        if(name.compare("descriptor")==0) {hasDescriptor = true; ++nfields;}
    }
    FieldConstPtrArray fields(nfields);
    StringArray names(nfields);
    size_t ind = 0;
    names[ind] = "value";
    fields[ind++] =  fieldCreate->createUnionArray(unionPtr);
    names[ind] = "channelName";
    fields[ind++] =  fieldCreate->createScalarArray(pvString);
    if(hasTimeStamp) {
        names[ind] = "timeStamp";
        fields[ind++] = standardField->timeStamp();
    }
    if(hasAlarm) {
        names[ind] = "alarm";
        fields[ind++] = standardField->alarm();
    }
    if(hasDescriptor) {
        names[ind] = "descriptor";
        fields[ind++] = fieldCreate->createScalar(pvString);
    }
    if(hasSeverity) {
        names[ind] = "severity";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(hasStatus) {
        names[ind] = "status";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    if(hasMessage) {
        names[ind] = "message";
        fields[ind++] = fieldCreate->createScalarArray(pvString);
    }
    if(hasSecondsPastEpoch) {
        names[ind] = "secondsPastEpoch";
        fields[ind++] = fieldCreate->createScalarArray(pvLong);
    }
    if(hasNanoseconds) {
        names[ind] = "nanoseconds";
        fields[ind++] = fieldCreate->createScalarArray(pvInt);
    }
    StructureConstPtr st = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(st);
    if(channelNames.size()>0) {
        PVStringArrayPtr pvName = pvStructure->getSubField<PVStringArray>("channelName");
        pvName->replace(channelNames);
    }
    return NTMultiChannelPtr(new NTMultiChannel(pvStructure));
}

NTMultiChannelPtr NTMultiChannel::clone(PVStructurePtr const & pv)
{
    PVStructurePtr pvStructure = getPVDataCreate()->createPVStructure(pv);
    return NTMultiChannelPtr(new NTMultiChannel(pvStructure));
}

NTMultiChannel::NTMultiChannel(PVStructurePtr const & pvStructure)
: pvNTMultiChannel(pvStructure),
  pvTimeStamp(pvStructure->getSubField<PVStructure>("timeStamp")),
  pvAlarm(pvStructure->getSubField<PVStructure>("alarm")),
  pvValue(pvStructure->getSubField<PVUnionArray>("value")),
  pvChannelName(pvStructure->getSubField<PVStringArray>("channelName")),
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
