/* ntmatrix.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTMATRIX_H
#define NTMATRIX_H

#ifdef epicsExportSharedSymbols
#   define ntmatrixEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDisplay.h>

#ifdef ntmatrixEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntmatrixEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>


namespace epics { namespace nt {

class NTMatrix;
typedef std::tr1::shared_ptr<NTMatrix> NTMatrixPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTMatrix.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTMatrixBuilder :
        public std::tr1::enable_shared_from_this<NTMatrixBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTMatrixBuilder);

        /**
         * Add dimensionfield to the NTMatrix.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer addDim();

        /**
         * Add descriptor field to the NTMatrix.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTMatrix.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTMatrix.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Add display structure to the NTMatrix.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer addDisplay();

        /**
         * Create a <b>Structure</b> that represents NTMatrix.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTMatrix.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTMatrix</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>NTMatrix</b>.
         */
        NTMatrixPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTMatrixBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTMatrixBuilder();

        void reset();

        bool dim;
        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool display;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTMatrix;
    };

}

typedef std::tr1::shared_ptr<detail::NTMatrixBuilder> NTMatrixBuilderPtr;



/**
 * @brief Convenience Class for NTMatrix
 *
 * @author dgh
 */
class epicsShareClass NTMatrix
{
public:
    POINTER_DEFINITIONS(NTMatrix);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMatrix.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTMatrix.
     * @return NTMatrix instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMatrix without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTMatrix.
     * @return NTMatrix instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTMatrix.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTMatrix.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTMatrix.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTMatrix.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTMatrix.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTMatrix.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTMatrix.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTMatrix.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTMatrix.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTMatrix
     * @return (false,true) if (is not, is) a valid NTMatrix.
     */
    bool isValid();

    /**
     * Create a NTMatrix builder instance.
     * @return builder instance.
     */
    static NTMatrixBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTMatrix() {}

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
     * Attach an pvDisplay.
     * @param pvDisplay The pvDisplay that will be attached.
     * Does nothing if no display.
      * @return true if the operation was successfull (i.e. this instance has a display field), otherwise false.
     */
    bool attachDisplay(epics::pvData::PVDisplay &pvDisplay) const;

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
     * Get the display.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getDisplay() const;

    /**
     * Get the value field.
     * @return The PVDoubleArray for the values.
     */
    epics::pvData::PVDoubleArrayPtr getValue() const;

    /**
     * Get the dim field.
     * @return The PVIntArray for the dim which may be null.
     */
    epics::pvData::PVIntArrayPtr getDim() const;   

private:
    NTMatrix(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTMatrix;
    epics::pvData::PVDoubleArrayPtr pvValue;

    friend class detail::NTMatrixBuilder;
};

}}
#endif  /* NTMATRIX_H */
