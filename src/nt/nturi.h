/* nturi.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTURI_H
#define NTURI_H

#include <vector>
#include <string>

#ifdef epicsExportSharedSymbols
#   define nturiEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#ifdef nturiEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef nturiEpicsExportSharedSymbols
#endif

#include <pv/ntfield.h>

#include <shareLib.h>

namespace epics { namespace nt {

class NTURI;
typedef std::tr1::shared_ptr<NTURI> NTURIPtr;

namespace detail {

    /**
     * @brief Interface for in-line creating of NTURI.
     *
     * One instance can be used to create multiple instances.
     * An instance of this object must not be used concurrently (an object has a state).
     * @author dgh
     */
    class epicsShareClass NTURIBuilder :
        public std::tr1::enable_shared_from_this<NTURIBuilder>
    {
    public:
        POINTER_DEFINITIONS(NTURIBuilder);

        /**
         * Add authority field to the NTURI.
         * @return this instance of <b>NTURIBuilder</b>.
         */
        shared_pointer addAuthority();

        /**
         * Add extra <b>Scalar</b> of ScalarType pvString
         * to the query field of the type.
         * @param name name of the field.
         * @return this instance of <b>NTURIBuilder</b>.
         */
         shared_pointer addQueryString(std::string const & name);

        /**
         * Add extra <b>Scalar</b> of ScalarType pvDouble
         * to the query field of the type.
         * @param name name of the field.
         * @return this instance of <b>NTURIBuilder</b>.
         */
         shared_pointer addQueryDouble(std::string const & name);

        /**
         * Add extra <b>Scalar</b> of ScalarType pvInt
         * to the query field of the type.
         * @param name name of the field.
         * @return this instance of <b>NTURIBuilder</b>.
         */
         shared_pointer addQueryInt(std::string const & name);

        /**
         * Add a column of given <b>Scalar</b> type.
         * @param name name of the column.
         * @param elementType column type, a scalar array.
         * @return this instance of <b>NTURIBuilder</b>.
         */
        //shared_pointer addColumn(std::string const & name, epics::pvData::ScalarType elememtType);

        /**
         * Create a <b>Structure</b> that represents NTURI.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>Structure</b>.
         */
        epics::pvData::StructureConstPtr createStructure();

        /**
         * Create a <b>PVStructure</b> that represents NTURI.
         * The returned PVStructure will have labels equal to the column names.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>PVStructure</b>.
         */
        epics::pvData::PVStructurePtr createPVStructure();

        /**
         * Create a <b>NTURI</b> instance.
         * The returned NTURI will wrap a PVStructure which will have
         * labels equal to the column names.
         * This resets this instance state and allows new instance to be created.
         * @return a new instance of <b>NTURI</b>.
         */
        NTURIPtr create();
        /**
         * Add extra <b>Field</b> to the type.
         * @param name name of the field.
         * @param field a field to add.
         * @return this instance of <b>NTURIBuilder</b>.
         */
        shared_pointer add(std::string const & name, epics::pvData::FieldConstPtr const & field);

    private:
        NTURIBuilder();

        void reset();

        std::vector<std::string> queryFieldNames;
        std::vector<epics::pvData::ScalarType> queryTypes;

        bool authority;

        // NOTE: this preserves order, however it does not handle duplicates
        epics::pvData::StringArray extraFieldNames;
        epics::pvData::FieldConstPtrArray extraFields;

        friend class ::epics::nt::NTURI;
    };

}

typedef std::tr1::shared_ptr<detail::NTURIBuilder> NTURIBuilderPtr;



/**
 * @brief Convenience Class for NTURI
 *
 * @author dgh
 */
class epicsShareClass NTURI
{
public:
    POINTER_DEFINITIONS(NTURI);

    static const std::string URI;

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTURI.
     * First isCompatible is called.
     * This method will nullptr if the structure is is not compatible.
     * This method will nullptr if the structure is nullptr.
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTURI.
     * @return NTURI instance on success, nullptr otherwise.
     */
    static shared_pointer wrap(epics::pvData::PVStructurePtr const & structure);

    /**
     * Wrap (aka dynamic cast, or wrap) the structure to NTMultiChannel without checking for isCompatible
     * @param structure The structure to wrap-ed (dynamic cast, wrapped) to NTURI.
     * @return NTURI instance.
     */
    static shared_pointer wrapUnsafe(epics::pvData::PVStructurePtr const & structure);

    /**
     * Is the structure an NTURI.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTURI.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);

    /**
     * Is the structure an NTURI.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTURI.
     */
    static bool is_a(epics::pvData::PVStructurePtr const & pvStructure);

    /**
     * Is the Structure compatible with NTURI.
     * This method introspects the fields to see if they are compatible.
     * @param structure The Structure to test.
     * @return (false,true) if (is not, is) an NTURI.
     */
    static bool isCompatible(
        epics::pvData::StructureConstPtr const &structure);

    /**
     * Is the PVStructure compatible with NTURI.
     * This method introspects the fields to see if they are compatible.
     * @param pvStructure The PVStructure to test.
     * @return (false,true) if (is not, is) an NTURI.
     */
    static bool isCompatible(
        epics::pvData::PVStructurePtr const &pvStructure);

    /**
     * Checks if the specified structure is a valid NTURI.
     *
     * Checks whether the wrapped structure is valid with respect to this
     * version of NTURI
     * @return (false,true) if (is not, is) a valid NTURI.
     */
    bool isValid();

    /**
     * Create a NTURI builder instance.
     * @return builder instance.
     */
    static NTURIBuilderPtr createBuilder();

    /**
     * Destructor.
     */
    ~NTURI() {}

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the scheme field.
     * @return The PVString for the scheme.
     */
    epics::pvData::PVStringPtr getScheme() const;

    /**
     * Get the authority field.
     * @return The PVString for the authority.
     */
    epics::pvData::PVStringPtr getAuthority() const;

    /**
     * Get the path field.
     * @return The PVString for the path.
     */
    epics::pvData::PVStringPtr getPath() const;

    /**
     * Get the query field.
     * @return The PVStructure for the query.
     */
    epics::pvData::PVStructurePtr getQuery() const;

    /**
     * Get the names of the query fields for the URI.
     * For each name, calling getQueryField should return
     * the query field, which should not be null.
     * @return The query field names.
     */
    epics::pvData::StringArray const & getQueryNames() const;

    /**
     * Get the PVField (column) for a field that follows the label field.
     * @param columnName The name of the column.
     * @return The PVFieldPtr for the field.
     */
    epics::pvData::PVFieldPtr getQueryField(std::string const & columnName) const;

    /**
     * Get the PVField (column) for a field that follows the label field of a specified type (e.g. PVDoubleArray).
     * @param columnName The name of the column.
     * @return The <PVT> field.
     */
    template<typename PVT>
    std::tr1::shared_ptr<PVT> getQueryField(std::string const & columnName) const
    {
        epics::pvData::PVFieldPtr pvField = getQueryField(columnName);
        if (pvField.get())
            return std::tr1::dynamic_pointer_cast<PVT>(pvField);
        else
            return std::tr1::shared_ptr<PVT>();
    }

private:
    NTURI(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTURI;
    friend class detail::NTURIBuilder;
};

}}
#endif  /* NTURI_H */
