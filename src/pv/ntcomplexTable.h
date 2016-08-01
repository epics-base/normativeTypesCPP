/* ntcomplexTable.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTCOMPLEXTABLE_H
#define NTCOMPLEXTABLE_H

#include <vector>
#include <string>

#ifdef epicsExportSharedSymbols
#   define ntcomplexTableEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDisplay.h>
#include <pv/pvControl.h>

#ifdef ntcomplexTableEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef ntcomplexTableEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTComplexTable;
typedef std::tr1::shared_ptr<NTComplexTable> NTComplexTablePtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTComplexTable.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTComplexTableBuilder :
        public std::tr1::enable_shared_from_this<NTComplexTableBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTComplexTableBuilder);

        /**
         * Adds a column of given <b>Union</b> type.
         * @param name name of the column.
         * @param elementType column type, a Union.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer addColumn(std::string const & name, epics::pvData::UnionConstPtr elementType);

        /**
         * Adds a column of variant union type.
         * @param name name of the column.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer addColumn(std::string const & name);

        /**
         * Adds descriptor field to the NTComplexTable.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer addDescriptor();

        /**
         * Adds alarm field to the NTComplexTable.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer addAlarm();

        /**
         * Adds timeStamp field to the NTComplexTable.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer addTimeStamp();

        /**
         * Creates a <b>Structure</b> that represents NTComplexTable.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Creates a <b>PVStructure</b> that represents NTComplexTable.
         * The returned PVStructure will have labels equal to the column names.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Creates a <b>NTComplexTable</b> instance.
         * The returned NTComplexTable will wrap a PVStructure which will have
         * labels equal to the column names.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>NTComplexTable</b>.
         */
        NTComplexTablePtr create();

        /**
         * Adds extra <b>Field</b> to the type.
         * @param name the name of the field.
         * @param field the field to be added.
         * @return this instance of <b>NTComplexTableBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTComplexTableBuilder();

        void reset();

        std::vector<std::string> columnNames;
        std::vector<epics::pvData::UnionConstPtr> types;

        bool descriptor;
        bool alarm;
        bool timeStamp;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTComplexTable;
    };

}

typedef std::tr1::shared_ptr<detail::NTComplexTableBuilder> NTComplexTableBuilderPtr;



/**
 * @brief Convenience Class for NTComplexTable
 *
 * @author dgh
 */
class epicsShareClass NTComplexTable
{
public:
    POINTER_DEFINITIONS(NTComplexTable);

    static const std::string URI;

    /**
     * Creates an NTComplexTable wrapping the specified PVStructure if the latter is compatible.
     * <p>
     * Checks the supplied PVStructure is compatible with NTComplexTable
     * and if so returns an NTComplexTable which wraps it.
     * This method will return null if the structure is is not compatible
     * or is null.
     *
     * @param pvStructure the PVStructure to be wrapped
     * @return NTComplexTable instance wrapping pvStructure on success, null otherwise
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Creates an NTComplexTable wrapping the specified PVStructure, regardless of the latter's compatibility.
     * <p>
     * No checks are made as to whether the specified PVStructure
     * is compatible with NTComplexTable or is non-null.
     *
     * @param pvStructure the PVStructure to be wrapped
     * @return NTComplexTable instance wrapping pvStructure
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Returns whether the specified Structure reports to be a compatible NTComplexTable.
     * <p>
     * Checks if the specified Structure reports compatibility with this
     * version of NTComplexTable through its type ID, including checking version numbers.
     * The return value does not depend on whether the structure is actually
     * compatible in terms of its introspection type.
     *
     * @param structure the Structure to test
     * @return (false,true) if the specified Structure (is not, is) a compatible NTComplexTable
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Returns whether the specified PVStructure reports to be a compatible NTComplexTable.
     * <p>
     * Checks if the specified PVStructure reports compatibility with this
     * version of NTComplexTable through its type ID, including checking version numbers.
     * The return value does not depend on whether the structure is actually
     * compatible in terms of its introspection type.
     *
     * @param pvStructure the PVStructure to test
     * @return (false,true) if the specified PVStructure (is not, is) a compatible NTComplexTable
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Returns whether the specified Structure is compatible with NTComplexTable.
     * <p>
     * Checks if the specified Structure is compatible with this version
     * of NTComplexTable through the introspection interface.
     *
     * @param structure the Structure to test
     * @return (false,true) if the specified Structure (is not, is) a compatible NTComplexTable
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Returns whether the specified PVStructure is compatible with NTComplexTable.
     *
     * Checks if the specified PVStructure is compatible with this version
     * of NTComplexTable through the introspection interface.
     *
     * @param pvStructure the PVStructure to test
     * @return (false,true) if the specified PVStructure (is not, is) a compatible NTComplexTable
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Returns whether the specified structure is a valid NTComplexTable.
     * <p>
     * Unlike isCompatible(), isValid() may perform checks on the value
     * data as well as the introspection data.
     *
     * @return (false,true) if wrapped PVStructure (is not, is) a valid NTComplexTable
     */
    bool isValid();

    /**
     * Creates an NTComplexTable builder instance.
     * @return builder instance.
     */
    static NTComplexTableBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTComplexTable() {}

     /**
      * Attaches a PVTimeStamp to the wrapped PVStructure.
      * Does nothing if no timeStamp field.
      * @param pvTimeStamp the PVTimeStamp that will be attached.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attaches a PVAlarm to the wrapped PVStructure.
     * Does nothing if no alarm field.
     * @param pvAlarm the PVAlarm that will be attached.
     * @return true if the operation was successfull (i.e. this instance has an alarm field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Returns the PVStructure wrapped by this instance.
     * @return the PVStructure wrapped by this instance.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Returns the descriptor field.
     * @return the descriptor field or null if no descriptor field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Returns the timeStamp field.
     * @return the timStamp field or null if no such field.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Returns the alarm field.
     * @return the alarm field or null if no such field.
     */
    epics::pvData::PVStructurePtr getAlarm() const;

    /**
     * Returns the labels field.
     * @return the labels field.
     */
    epics::pvData::PVStringArrayPtr getLabels() const;

    /**
     * Returns the column names for the table.
     * For each name, calling getColumn should return the column, which should not be null.
     * @return the column names.
     */
    epics::pvData::StringArray const & getColumnNames() const;

    /**
     * Returns the PVUnion for the column with the specified colum name.
     * @param columnName the name of the column.
     * @return the field for the column or null if column does not exist.
     */
    epics::pvData::PVUnionArrayPtr getColumn(std::string const & columnName) const;

private:
    NTComplexTable(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTComplexTable;
    epics::pvData::PVStructurePtr pvValue;
    friend class detail::NTComplexTableBuilder;
};

}}
#endif  /* NTCOMPLEXTABLE_H */
