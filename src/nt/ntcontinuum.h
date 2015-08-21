/* ntcontinuum.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTCONTINUUM_H
#define NTCONTINUUM_H

#ifdef epicsExportSharedSymbols
#   define ntcontinuumEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef ntcontinuumEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntcontinuumEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>


namespace epics { namespace nt {

class NTContinuum;
typedef std::tr1::shared_ptr<NTContinuum> NTContinuumPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTContinuum.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTContinuumBuilder :
        public std::tr1::enable_shared_from_this<NTContinuumBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTContinuumBuilder);

        /**
         * Add descriptor field to the NTContinuum.
         * @return this instance of <b>NTContinuumBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTContinuum.
         * @return this instance of <b>NTContinuumBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTContinuum.
         * @return this instance of <b>NTContinuumBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a <b>Structure</b> that represents NTContinuum.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTContinuum.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTContinuum</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>NTContinuum</b>.
         */
        NTContinuumPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTContinuumBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTContinuumBuilder();

        void reset();

        bool dim;
        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTContinuum;
    };

}

typedef std::tr1::shared_ptr<detail::NTContinuumBuilder> NTContinuumBuilderPtr;



/**
 * @brief Convenience Class for NTContinuum
 *
 * @author dgh
 */
class epicsShareClass NTContinuum
{
public:
    POINTER_DEFINITIONS(NTContinuum);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTContinuum.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTContinuum.
     * @return NTContinuum instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTContinuum without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTContinuum.
     * @return NTContinuum instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTContinuum.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTContinuum.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTContinuum.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTContinuum.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTContinuum.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTContinuum.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTContinuum.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTContinuum.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTContinuum.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTContinuum
     * @return (false,true) if (is not, is) a valid NTContinuum.
     */
    bool isValid();

    /**
     * Create a NTContinuum builder instance.
     * @return builder instance.
     */
    static NTContinuumBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTContinuum() {}

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
     * Get the base field.
     * @return The PVDoubleArray for the base.
     */
    epics::pvData::PVDoubleArrayPtr getBase() const;

    /**
     * Get the value field.
     * @return The PVDoubleArray for the values.
     */
    epics::pvData::PVDoubleArrayPtr getValue() const;

    /**
     * Get the units field.
     * @return The PVStringArray for the units.
     */
    epics::pvData::PVStringArrayPtr getUnits() const;   

private:
    NTContinuum(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTContinuum;
    epics::pvData::PVDoubleArrayPtr pvValue;

    friend class detail::NTContinuumBuilder;
};

}}
#endif  /* NTCONTINUUM_H */
