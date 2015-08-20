/* ntenum.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTENUM_H
#define NTENUM_H

#ifdef epicsExportSharedSymbols
#   define ntenumEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef ntenumEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntenumEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTEnum;
typedef std::tr1::shared_ptr<NTEnum> NTEnumPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTEnum.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTEnumBuilder :
        public std::tr1::enable_shared_from_this<NTEnumBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTEnumBuilder);

        /**
         * Add descriptor field to the NTEnum.
         * @return this instance of <b>NTEnumBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTEnum.
         * @return this instance of <b>NTEnumBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTEnum.
         * @return this instance of <b>NTEnumBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a <b>Structure</b> that represents NTEnum.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTEnum.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTEnum</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>NTEnum</b>.
         */
        NTEnumPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTEnumBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTEnumBuilder();

        void reset();

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTEnum;
    };

}

typedef std::tr1::shared_ptr<detail::NTEnumBuilder> NTEnumBuilderPtr;



/**
 * @brief Convenience Class for NTEnum
 *
 * @author dgh
 */
class epicsShareClass NTEnum
{
public:
    POINTER_DEFINITIONS(NTEnum);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTEnum.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTEnum.
     * @return NTEnum instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTEnum.
     * @return NTEnum instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTEnum.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTEnum.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);
    /**
     * Is the pvStructure compatible with  NTEnum.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTMultiChannel.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);
    /**
     * Create a NTEnum builder instance.
     * @return builder instance.
     */
    static NTEnumBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTEnum() {}

     /**
      * Attach a pvTimeStamp.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Get the value field.
     * @return The PVStructure for the values.
     */
    epics::pvData::PVStructurePtr getValue() const;

private:
    NTEnum(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTEnum;
    epics::pvData::PVStructurePtr pvValue;

    friend class detail::NTEnumBuilder;
};

}}
#endif  /* NTENUM_H */
