/* nthistogram.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTHISTOGRAM_H
#define NTHISTOGRAM_H

#ifdef epicsExportSharedSymbols
#   define nthistogramEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef nthistogramEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef nthistogramEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>


namespace epics { namespace nt {

class NTHistogram;
typedef std::tr1::shared_ptr<NTHistogram> NTHistogramPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTHistogram.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTHistogramBuilder :
        public std::tr1::enable_shared_from_this<NTHistogramBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTHistogramBuilder);

        /**
         * Set a scalar type of value field array.
         * @param scalarType the value type.
         * @return this instance of <b>NTHistogramBuilder</b>.
         */
        shared_pointer value(epics::pvData::ScalarType scalarType);

        /**
         * Add descriptor field to the NTHistogram.
         * @return this instance of <b>NTHistogramBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTHistogram.
         * @return this instance of <b>NTHistogramBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTHistogram.
         * @return this instance of <b>NTHistogramBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a <b>Structure</b> that represents NTHistogram.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTHistogram.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTHistogram</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>NTHistogram</b>.
         */
        NTHistogramPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTHistogramBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTHistogramBuilder();

        void reset();

        bool valueTypeSet;
        epics::pvData::ScalarType valueType;

        bool dim;
        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTHistogram;
    };

}

typedef std::tr1::shared_ptr<detail::NTHistogramBuilder> NTHistogramBuilderPtr;



/**
 * @brief Convenience Class for NTHistogram
 *
 * @author dgh
 */
class epicsShareClass NTHistogram
{
public:
    POINTER_DEFINITIONS(NTHistogram);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTHistogram.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTHistogram.
     * @return NTHistogram instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTHistogram without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTHistogram.
     * @return NTHistogram instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTHistogram.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTHistogram.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTHistogram.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTHistogram.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTHistogram.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTHistogram.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTHistogram.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTHistogram.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTHistogram.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTHistogram
     * @return (false,true) if (is not, is) a valid NTHistogram.
     */
    bool isValid();

    /**
     * Create a NTHistogram builder instance.
     * @return builder instance.
     */
    static NTHistogramBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTHistogram() {}

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
    epics::pvData::PVDoubleArrayPtr getRanges() const;

    /**
     * Get the value field.
     * @return The PVScalarArray for the values.
     */
    epics::pvData::PVScalarArrayPtr getValue() const;

    /**
     * Get the value field of a specified type (e.g. PVIntArray).
     * @return The <PVT> field for the values.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValue() const
    {
        return std::tr1::dynamic_pointer_cast<PVT>(pvValue);
    }
  

private:
    NTHistogram(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTHistogram;
    epics::pvData::PVScalarArrayPtr pvValue;

    friend class detail::NTHistogramBuilder;
};

}}
#endif  /* NTHISTOGRAM_H */
