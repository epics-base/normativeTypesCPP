/* ntaggregate.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTAGGREGATE_H
#define NTAGGREGATE_H

#ifdef epicsExportSharedSymbols
#   define ntaggregateEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef ntaggregateEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntaggregateEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTAggregate;
typedef std::tr1::shared_ptr<NTAggregate> NTAggregatePtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTAggregate.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTAggregateBuilder :
        public std::tr1::enable_shared_from_this<NTAggregateBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTAggregateBuilder);

        /**
         * Add dispersion field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addDispersion();

        /**
         * Add first field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addFirst();

        /**
         * Add firstTimeStamp field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addFirstTimeStamp();

        /**
         * Add last field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addLast();

        /**
         * Add lastTimeStamp field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addLastTimeStamp();

        /**
         * Add max field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addMax();

        /**
         * Add min field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addMin();

        /**
         * Add descriptor field to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTAggregate.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a <b>Structure</b> that represents NTAggregate.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTAggregate.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTAggregate</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>NTAggregate</b>.
         */
        NTAggregatePtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTAggregateBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTAggregateBuilder();

        void reset();

        bool dispersion;
        bool first;
        bool firstTimeStamp;
        bool last;
        bool lastTimeStamp;
        bool max; 
        bool min;

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTAggregate;
    };

}

typedef std::tr1::shared_ptr<detail::NTAggregateBuilder> NTAggregateBuilderPtr;


/**
 * @brief Convenience Class for NTAggregate
 *
 * @author dgh
 */
class epicsShareClass NTAggregate
{
public:
    POINTER_DEFINITIONS(NTAggregate);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTAggregate.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTAggregate.
     * @return NTAggregate instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTAggregate without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTAggregate.
     * @return NTAggregate instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTAggregate.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTAggregate.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTAggregate.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTAggregate.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTAggregate.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTAggregate.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTAggregate.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTAggregate.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTAggregate.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTAggregate
     * @return (false,true) if (is not, is) a valid NTAggregate.
     */
    bool isValid();

    /**
     * Create a NTAggregate builder instance.
     * @return builder instance.
     */
    static NTAggregateBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTAggregate() {}

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
     * @return The PVDouble for the value.
     */
    epics::pvData::PVDoublePtr getValue() const;

    /**
     * Get the N field.
     * @return The PVLong for the N field.
     */
    epics::pvData::PVLongPtr getN() const;

    /**
     * Get the dispersion field.
     * @return The PVDouble for the dispersion which may be null
     */
    epics::pvData::PVDoublePtr getDispersion() const;

    /**
     * Get the first field.
     * @return The PVDouble for the first field which may be null
     */
    epics::pvData::PVDoublePtr getFirst() const;

    /**
     * Get the firstTimeStamp field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getFirstTimeStamp() const;

    /**
     * Get the last field.
     * @return The PVDouble for the last field which may be null
     */
    epics::pvData::PVDoublePtr getLast() const;

    /**
     * Get the lastTimeStamp field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getLastTimeStamp() const;

    /**
     * Get the max field.
     * @return The PVDouble for the max field which may be null
     */
    epics::pvData::PVDoublePtr getMax() const;

    /**
     * Get the min field.
     * @return The PVDouble for the max field which may be null
     */
    epics::pvData::PVDoublePtr getMin() const;

private:
    NTAggregate(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTAggregate;
    epics::pvData::PVDoublePtr pvValue;

    friend class detail::NTAggregateBuilder;
};

}}
#endif  /* NTAGGREGATE_H */
