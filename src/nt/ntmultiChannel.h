/* ntmultiChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTMULTICHANNEL_H
#define NTMULTICHANNEL_H

#include <pv/ntfield.h>

namespace epics { namespace nt { 

/**
 * Convenience Class for NTMultiChannel
 * @author mrk
 *
 */

class NTMultiChannel;
typedef std::tr1::shared_ptr<NTMultiChannel> NTMultiChannelPtr;

class NTMultiChannel
{
public:
    POINTER_DEFINITIONS(NTMultiChannel);
    /**
     * Is the pvStructure an NTMultiChannel.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isNTMultiChannel(
        epics::pvData::PVStructurePtr const &pvStructure);
    static NTMultiChannelPtr create(
        std::vector<std::string> const & optionNames);
    static NTMultiChannelPtr create(
        std::vector<std::string> const & optionNames,
        epics::pvData::UnionConstPtr const & unionPtr);
    static NTMultiChannelPtr create(
        std::vector<std::string> const & optionNames,
        epics::pvData::UnionConstPtr const & unionPtr,
        epics::pvData::shared_vector<const std::string> channelNames);
    static NTMultiChannelPtr clone(epics::pvData::PVStructurePtr const &);
    /**
     * Destructor
     */
    ~NTMultiChannel() {}
     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp
      */
    void attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp);
    /**
     * Attach a pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm
     */
    void attachAlarm(epics::pvData::PVAlarm &pvAlarm);
    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure(){return pvNTMultiChannel;}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp(){return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() {return pvAlarm;}
    epics::pvData::PVUnionArrayPtr getValue() {return pvValue;}
    epics::pvData::PVStringArrayPtr getChannelName() { return pvChannelName;};
    epics::pvData::PVIntArrayPtr getSeverity() {return pvSeverity;}
    epics::pvData::PVIntArrayPtr getStatus() {return pvStatus;}
    epics::pvData::PVStringArrayPtr getMessage() {return pvMessage;}
    epics::pvData::PVLongArrayPtr getSecondsPastEpoch() {return pvSecondsPastEpoch;}
    epics::pvData::PVIntArrayPtr getNanoseconds() {return pvNanoseconds;}
    epics::pvData::PVStringPtr getDescriptor() {return pvDescriptor;}
private:
    NTMultiChannel(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTMultiChannel;
    epics::pvData::PVStructurePtr pvTimeStamp;
    epics::pvData::PVStructurePtr pvAlarm;
    epics::pvData::PVUnionArrayPtr pvValue;
    epics::pvData::PVStringArrayPtr pvChannelName;
    epics::pvData::PVIntArrayPtr pvSeverity;
    epics::pvData::PVIntArrayPtr pvStatus;
    epics::pvData::PVStringArrayPtr pvMessage;
    epics::pvData::PVLongArrayPtr pvSecondsPastEpoch;
    epics::pvData::PVIntArrayPtr pvNanoseconds;
    epics::pvData::PVStringPtr pvDescriptor;
};

}}
#endif  /* NTMULTICHANNEL_H */
