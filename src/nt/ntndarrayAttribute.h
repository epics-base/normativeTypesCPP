/* ntndarrayAttribute.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNDARRAYATTRIBUTE_H
#define NTNDARRAYATTRIBUTE_H

#ifdef epicsExportSharedSymbols
#   define ntndarrayAttributeEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef ntndarrayAttributeEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntndarrayAttributeEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTNDArrayAttribute;
typedef std::tr1::shared_ptr<NTNDArrayAttribute> NTNDArrayAttributePtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTAttribute extended as required by NTNDArray 
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTNDArrayAttributeBuilder :
        public std::tr1::enable_shared_from_this<NTNDArrayAttributeBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTNDArrayAttributeBuilder);

        /**
         * Add tags field to the NTNDArrayAttribute.
         * @return this instance of <b>NTNDArrayAttributeBuilder</b>.
         */
        virtual shared_pointer addTags();

        /**
         * Add descriptor field to the NTNDArrayAttribute.
         * @return this instance of <b>NTNDArrayAttributeBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Add alarm structure to the NTNDArrayAttribute.
         * @return this instance of <b>NTNDArrayAttributeBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Add timeStamp structure to the NTNDArrayAttribute.
         * @return this instance of <b>NTNDArrayAttributeBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Create a <b>Structure</b> that represents NTNDArrayAttribute.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTNDArrayAttribute.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTNDArrayAttribute</b> instance.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of a <b>NTNDArrayAttribute</b>.
         */
        NTNDArrayAttributePtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTNDArrayAttributeBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    protected://private:
        NTNDArrayAttributeBuilder();

        void reset();

        bool tags;
        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTNDArrayAttribute;
    };

}

typedef std::tr1::shared_ptr<detail::NTNDArrayAttributeBuilder> NTNDArrayAttributeBuilderPtr;



/**
 * @brief Convenience Class for NTNDArrayAttribute
 *
 * @author dgh
 */
class epicsShareClass NTNDArrayAttribute
{
public:
    POINTER_DEFINITIONS(NTNDArrayAttribute);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTNDArrayAttribute.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNDArrayAttribute.
     * @return NTNDArrayAttribute instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTNDArrayAttribute without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTNDArrayAttribute.
     * @return NTNDArrayAttribute instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTNDArrayAttribute.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTNDArrayAttribute.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTNDArrayAttribute.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTNDArrayAttribute.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTNDArrayAttribute.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTNDArrayAttribute.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTNDArrayAttribute.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTNDArrayAttribute.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTNDArrayAttribute.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTNDArrayAttribute
     * @return (false,true) if (is not, is) a valid NTNDArrayAttribute.
     */
    bool isValid();

    /**
     * Create a NTNDArrayAttribute builder instance.
     * @return builder instance.
     */
    static NTNDArrayAttributeBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTNDArrayAttribute() {}

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
     * @return The pvString 
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
     * Get the name field.
     * @return The PVString for the name.
     */
    epics::pvData::PVStringPtr getName() const;

    /**
     * Get the value field.
     * @return The PVUnion for the values.
     */
    epics::pvData::PVUnionPtr getValue() const;

    /**
     * Get the tags field.
     * @return The PVStringArray for the tags, which may be null.
     */
    epics::pvData::PVStringArrayPtr getTags() const;

    /**
     * Get the sourceType field.
     * @return The PVInt for the sourceType field.
     */
    epics::pvData::PVIntPtr getSourceType() const;

    /**
     * Get the source field.
     * @return The PVString for the source field, which may be null.
     */
    epics::pvData::PVStringPtr getSource() const;

private:
    NTNDArrayAttribute(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTNDArrayAttribute;
    epics::pvData::PVUnionPtr pvValue;

    friend class detail::NTNDArrayAttributeBuilder;
};

}}
#endif  /* NTNDARRAYATTRIBUTE_H */
