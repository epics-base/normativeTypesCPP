/* ntscalarMultiChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTSCALARMULTICHANNEL_H
#define NTSCALARMULTICHANNEL_H

#include <vector>
#include <string>

#ifdef epicsExportSharedSymbols
#   define ntscalarMultiChannelEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

#ifdef ntscalarMultiChannelEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntscalarMultiChannelEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>


namespace epics { namespace nt { 


class NTScalarMultiChannel;
typedef std::tr1::shared_ptr<NTScalarMultiChannel> NTScalarMultiChannelPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTScalarMultiChannel.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     * @author mse
     */
    class epicsShareClass NTScalarMultiChannelBuilder :
        public std::tr1::enable_shared_from_this<NTScalarMultiChannelBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTScalarMultiChannelBuilder);
        /**
         * specify the scalar type for the value field.
         * If this is not called then pvDouble is the default.
         * @return this instance of  <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer value(epics::pvData::ScalarType scalarType);
        /**
         * Add descriptor field to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addDescriptor();
        /**
         * Add alarm structure to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addAlarm();
        /**
         * Add timeStamp structure to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addTimeStamp();
        /**
         * Add severity array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addSeverity();
        /**
         * Add status array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addStatus();
        /**
         * Add message array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addMessage();
        /**
         * Add secondsPastEpoch array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addSecondsPastEpoch();
        /**
         * Add nanoseconds array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addNanoseconds();
        /**
         * Add userTag array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addUserTag();
        /**
         * Add isConnected array to the NTScalarMultiChannel.
         * @return this instance of <b>NTScalarMultiChannelBuilder</b>.
         */
        shared_pointer addIsConnected();
        /**
         * Create a <b>Structure</b> that represents NTScalarMultiChannel.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();
        /**
         * Create a <b>PVStructure</b> that represents NTScalarMultiChannel.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a <b>PVStructure</b>
         */
        epics::pvData::PVStructurePtr createPVStructure();
        /**
         * Create a <b>NTScalarMultiChannel</b> instance.
         * This resets this instance state and allows new {@code instance to be created.}
         * @return a new instance of a <b>NTScalarMultiChannel</b>
         */
        NTScalarMultiChannelPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of a <b>NTScalarMultiChannelBuilder</b>
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);
    private:
        NTScalarMultiChannelBuilder();

        void reset();

        epics::pvData::ScalarType valueType;
        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool severity;
        bool status;
        bool message;
        bool secondsPastEpoch;
        bool nanoseconds;
        bool userTag;
        bool isConnected;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTScalarMultiChannel;
    };

}

typedef std::tr1::shared_ptr<detail::NTScalarMultiChannelBuilder> NTScalarMultiChannelBuilderPtr;


/**
 * @brief Convenience Class for NTScalarMultiChannel
 * @author dgh
 * @author mrk
 *
 */
class epicsShareClass NTScalarMultiChannel
{
public:
    POINTER_DEFINITIONS(NTScalarMultiChannel);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTScalarMultiChannel.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTScalarMultiChannel.
     * @return NTScalarMultiChannel instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTScalarMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTScalarMultiChannel.
     * @return NTScalarMultiChannel instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);
    /**
     * Is the Structure an NTScalarMultiChannel.
     * This method structure->getID() and checks if it is the same as the URI.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTScalarMultiChannel.
     */
    static bool is_a(
        epics::pvData::StructureConstPtr const &structure);
    /**
     * Is the pvStructure compatible with  NTScalarMultiChannel.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTScalarMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTScalarMultiChannelBuilder instance
     * @return builder instance.
     */
    static  NTScalarMultiChannelBuilderPtr createBuilder();

    /**
     * Destructor
     */
    ~NTScalarMultiChannel() {}
     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp
      */
    void attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;
    /**
     * Attach a pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm
     */
    void attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;
    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const
    {return pvNTScalarMultiChannel;}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const
    {return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const
     {return pvAlarm;}
    /**
     * Get the value of each channel.
     * @return PVScalarArrayPtr
     */
    epics::pvData::PVScalarArrayPtr getValue() const 
    {return pvValue;}
    /**
     * Get the value field of a specified type (e.g. PVDoubleArray).
     * @return The <PVT> field for the values.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValue() const
    {
        return std::tr1::dynamic_pointer_cast<PVT>(pvValue);
    }
    /**
     * Get the channelName of each channel.
     * @return PVStringArrayPtr
     */
    epics::pvData::PVStringArrayPtr getChannelName() const 
    { return pvChannelName;};
    /**
     * Get the connection state of each channel.
     * @return PVBooleanArrayPtr
     */
    epics::pvData::PVBooleanArrayPtr getIsConnected() const 
    { return pvIsConnected;};
    /**
     * Get the severity of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getSeverity() const 
    {return pvSeverity;}
    /**
     * Get the status of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getStatus() const 
    {return pvStatus;}
    /**
     * Get the message of each chnnel.
     * @return PVStringArrayPtr which may be null.
     */
    epics::pvData::PVStringArrayPtr getMessage() const 
    {return pvMessage;}
    /**
     * Get the secondsPastEpoch of each channel.
     * @return PVLongArrayPtr which may be null.
     */
    epics::pvData::PVLongArrayPtr getSecondsPastEpoch() const 
    {return pvSecondsPastEpoch;}
    /**
     * Get the nanoseconds of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getNanoseconds() const 
    {return pvNanoseconds;}
    /**
     * Get the userTag of each channel.
     * @return PVIntArrayPtr which may be null.
     */
    epics::pvData::PVIntArrayPtr getUserTag() const 
    {return pvUserTag;}
    /**
     * Get the descriptor.
     * @return PVStringPtr which may be null.
     */
    epics::pvData::PVStringPtr getDescriptor() const 
    {return pvDescriptor;}
private:
    NTScalarMultiChannel(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTScalarMultiChannel;
    epics::pvData::PVStructurePtr pvTimeStamp;
    epics::pvData::PVStructurePtr pvAlarm;
    epics::pvData::PVScalarArrayPtr pvValue;
    epics::pvData::PVStringArrayPtr pvChannelName;
    epics::pvData::PVBooleanArrayPtr pvIsConnected;
    epics::pvData::PVIntArrayPtr pvSeverity;
    epics::pvData::PVIntArrayPtr pvStatus;
    epics::pvData::PVStringArrayPtr pvMessage;
    epics::pvData::PVLongArrayPtr pvSecondsPastEpoch;
    epics::pvData::PVIntArrayPtr pvNanoseconds;
    epics::pvData::PVIntArrayPtr pvUserTag;
    epics::pvData::PVStringPtr pvDescriptor;
    friend class detail::NTScalarMultiChannelBuilder;
};

}}
#endif  /* NTSCALARMULTICHANNEL_H */
