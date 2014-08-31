/* ntmultiChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTMULTICHANNEL_H
#define NTMULTICHANNEL_H

#include <pv/ntfield.h>
#include <vector>
#include <string>


namespace epics { namespace nt { 

/**
 * Convenience Class for NTMultiChannel
 * @author mrk
 *
 */

class NTMultiChannel;
typedef std::tr1::shared_ptr<NTMultiChannel> NTMultiChannelPtr;

namespace detail {

    /**
     * Interface for in-line creating of NTMultiChannel.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTMultiChannelBuilder :
        public std::tr1::enable_shared_from_this<NTMultiChannelBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTMultiChannelBuilder);
        /**
         * specify the union for the value field.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addValue(epics::pvData::UnionConstPtr valuePtr);
        /**
         * Add descriptor field to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addDescriptor();
        /**
         * Add alarm structure to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addAlarm();
        /**
         * Add timeStamp structure to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addTimeStamp();
        /**
         * Add severity array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addSeverity();
        /**
         * Add status array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addStatus();
        /**
         * Add message array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addMessage();
        /**
         * Add secondsPastEpoch array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addSecondsPastEpoch();
        /**
         * Add nanoseconds array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addNanoseconds();
        /**
         * Add userTag array to the NTMultiChannel.
         * @return this instance of a {@code NTMultiChannelBuilder}.
         */
        shared_pointer addUserTag();
        /**
         * Create a {@code Structure} that represents NTMultiChannel.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();
        /**
         * Create a {@code PVStructure} that represents NTMultiChannel.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();
        /**
         * Create a {@code NTMultiChannel} instance.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a {@code NTMultiChannel}
         */
        NTMultiChannelPtr create();
    private:
        NTMultiChannelBuilder();

        void reset();

        epics::pvData::UnionConstPtr valueUnion;
        bool value;
        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool severity;
        bool status;
        bool message;
        bool secondsPastEpoch;
        bool nanoseconds;
        bool userTag;

        friend class ::epics::nt::NTMultiChannel;
    };

}

typedef std::tr1::shared_ptr<detail::NTMultiChannelBuilder> NTMultiChannelBuilderPtr;


class NTMultiChannel
{
public:
    POINTER_DEFINITIONS(NTMultiChannel);

    static const std::string URI;

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTMultiChannel.
     * First the structure ID is checked against NTMultiChannel::URI.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTMultiChannel.
     * @return NTMultiChannel instance on success, nullptr otherwise.
     */
    static shared_pointer narrow(epics::pvData::PVStructurePtr const & structure);

    /**
     * Narrow (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for null-ness or its ID.
     * @param structure The structure to narrow-ed (dynamic cast, wrapped) to NTMultiChannel.
     * @return NTMultiChannel instance.
     */
    static shared_pointer narrow_unsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the pvStructure an NTMultiChannel.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool is_a(
        epics::pvData::StructureConstPtr const &structure);
    /**
     * Create a NTMultiChannelBuilder instance
     * @return builder instance.
     */
    static  NTMultiChannelBuilderPtr createBuilder();

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
    /**
     * Get the value of each channel.
     * @return PVUnionArrayPtr
     */
    epics::pvData::PVUnionArrayPtr getValue() {return pvValue;}
    /**
     * Get the channelName of each channel.
     * @return PVStringArrayPtr
     */
    epics::pvData::PVStringArrayPtr getChannelName() { return pvChannelName;};
    /**
     * Get the connection state of each channel.
     * @return PVBooleanArrayPtr
     */
    epics::pvData::PVBooleanArrayPtr getIsConnected() { return pvIsConnected;};
    /**
     * Get the severity of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getSeverity() {return pvSeverity;}
    /**
     * Get the status of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getStatus() {return pvStatus;}
    /**
     * Get the message of each chnnel.
     * @return PVStringArrayPtr which may be null.
     */
    epics::pvData::PVStringArrayPtr getMessage() {return pvMessage;}
    /**
     * Get the secondsPastEpoch of each channel.
     * @return PVLongArrayPtr which may be null.
     */
    epics::pvData::PVLongArrayPtr getSecondsPastEpoch() {return pvSecondsPastEpoch;}
    /**
     * Get the nanoseconds of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getNanoseconds() {return pvNanoseconds;}
    /**
     * Get the userTag of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getUserTag() {return pvUserTag;}
    /**
     * Get the descriptor.
     * @return PVStringPtr which may be null.
     */
    epics::pvData::PVStringPtr getDescriptor() {return pvDescriptor;}
private:
    NTMultiChannel(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTMultiChannel;
    epics::pvData::PVStructurePtr pvTimeStamp;
    epics::pvData::PVStructurePtr pvAlarm;
    epics::pvData::PVUnionArrayPtr pvValue;
    epics::pvData::PVStringArrayPtr pvChannelName;
    epics::pvData::PVBooleanArrayPtr pvIsConnected;
    epics::pvData::PVIntArrayPtr pvSeverity;
    epics::pvData::PVIntArrayPtr pvStatus;
    epics::pvData::PVStringArrayPtr pvMessage;
    epics::pvData::PVLongArrayPtr pvSecondsPastEpoch;
    epics::pvData::PVIntArrayPtr pvNanoseconds;
    epics::pvData::PVIntArrayPtr pvUserTag;
    epics::pvData::PVStringPtr pvDescriptor;
    friend class detail::NTMultiChannelBuilder;
};

}}
#endif  /* NTMULTICHANNEL_H */
