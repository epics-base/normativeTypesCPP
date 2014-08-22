/* ntscalar.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTSCALAR_H
#define NTSCALAR_H

#include <pv/ntfield.h>
#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

namespace epics { namespace nt {

class NTScalar;
typedef std::tr1::shared_ptr<NTScalar> NTScalarPtr;

namespace detail {

    /**
     * Interface for in-line creating of NTScalar.
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author mse
     */
    class epicsShareClass NTScalarBuilder :
        public std::tr1::enable_shared_from_this<NTScalarBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTScalarBuilder);

        /**
         * Set a value type of a NTScalar.
         * @param scalarType the value type.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer value(epics::pvData::ScalarType scalarType);

        /**
         * Set a value array type of a NTScalar, i.e. it creates an NTScalarArray.
         * @param scalarType the value array element type.
         * @return this instance of a {@code NTTableBuilder}.
         */
        shared_pointer arrayValue(epics::pvData::ScalarType scalarType);

        /**
         * Add descriptor field to the NTScalar.
         * @return this instance of a {@code NTScalarBuilder}.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTScalar.
         * @return this instance of a {@code NTScalarBuilder}.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTScalar.
         * @return this instance of a {@code NTScalarBuilder}.
         */
        shared_pointer addTimeStamp();

        /**
         * Add display structure to the NTScalar.
         * @return this instance of a {@code NTScalarBuilder}.
         */
        shared_pointer addDisplay();

        /**
         * Add control structure to the NTScalar.
         * @return this instance of a {@code NTScalarBuilder}.
         */
        shared_pointer addControl();

        /**
         * Create a {@code Structure} that represents NTScalar.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a {@code Structure}.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a {@code PVStructure} that represents NTScalar.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code PVStructure}
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a {@code NTScalar} instance.
         * This resets this instance state and allows new {@code instance to be created.
         * @return a new instance of a {@code NTScalar}
         */
        NTScalarPtr create();

    private:
        NTScalarBuilder();

        void reset();

        bool isArray;
        bool valueTypeSet;
        epics::pvData::ScalarType valueType;

        bool descriptor;
        bool alarm;
        bool timeStamp;
        bool display;
        bool control;

        friend class ::epics::nt::NTScalar;
    };

}

typedef std::tr1::shared_ptr<detail::NTScalarBuilder> NTScalarBuilderPtr;



/**
 * Convenience Class for NTScalar
 * @author mrk
 */
class NTScalar
{
public:
    POINTER_DEFINITIONS(NTScalar);

    static const std::string URI;

    /**
     * Is the structure an NTScalar.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTScalar.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Create a NTScalar builder instance.
     * @return builder instance.
     */
    static NTScalarBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTScalar() {}

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
     * Attach an pvControl.
     * @param pvControl The pvControl that will be attached.
     * Does nothing if no control.
      * @return true if the operation was successfull (i.e. this instance has a control field), otherwise false.
     */
    bool attachControl(epics::pvData::PVControl &pvControl) const;

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
     * Get the control.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getControl() const;

    /**
     * Get the value field.
     * @return The PVField for the values.
     */
    epics::pvData::PVFieldPtr getValue() const;

    /**
     * Get the value field of a specified type (e.g. PVDouble).
     * @return The <PVT> field for the values.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getValue() const
    {
        return std::tr1::dynamic_pointer_cast<PVT>(pvValue);
    }

private:
    NTScalar(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTScalar;
    epics::pvData::PVFieldPtr pvValue;
    friend class detail::NTScalarBuilder;
};

}}
#endif  /* NTScalar_H */
