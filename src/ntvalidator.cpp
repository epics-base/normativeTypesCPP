/* ntvalidator.cpp */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

#include <sstream>
#include <algorithm>
#include <deque>

#define epicsExportSharedSymbols
#include <pv/ntvalidator.h>
#include <pv/bitSet.h>

namespace epics { namespace nt {

using epics::pvData::Type;
using epics::pvData::Field;
using epics::pvData::Union;
using epics::pvData::UnionArray;
using epics::pvData::Structure;
using epics::pvData::StructureConstPtr;
using epics::pvData::StructureArray;
using epics::pvData::StringArray;
using epics::pvData::FieldConstPtr;
using epics::pvData::FieldConstPtrArray;
using epics::pvData::BitSet;

using std::string;

struct Helper
{
    Validator::Definition const & definition;
    std::deque<string> path;
    Validator::Result result;

    Helper(Validator::Definition const & definition) : definition(definition) {}

    bool isOptional(Field const & field) const
    {
        return definition.optional.find(&field) != definition.optional.end();
    }

    string getCurrentPath(void) const
    {
        std::ostringstream os;

        // TODO: is there a better string join?
        std::deque<string>::const_iterator it;
        for (it = path.cbegin(); it != path.cend(); ++it) {
            if (it != path.cbegin())
                os << ".";
            os << *it;
        }

        return os.str();
    }

    void appendError(Validator::ErrorType type)
    {
        result.errors.push_back(Validator::Error(getCurrentPath(), type));
    }

    void appendError(Validator::ErrorType type, string const & field_name)
    {
        path.push_back(field_name);
        appendError(type);
        path.pop_back();
    }

    bool validate(Union const & ref, Union const & un)
    {
        // TODO: Validator Errors for unions could be more granular
        if (&un == &ref)
            return true;

        if (ref.isVariant() != un.isVariant()) {
            appendError(Validator::ErrorType::INCORRECT_TYPE);
            return false;
        }

        if (ref.isVariant())
            return true;

        size_t numfields = ref.getNumberFields();

        if (un.getNumberFields() != numfields) {
            appendError(Validator::ErrorType::INCORRECT_TYPE);
            return false;
        }

        StringArray const & rnames = ref.getFieldNames();
        StringArray const & unames = un.getFieldNames();

        if (!std::equal(rnames.cbegin(), rnames.cend(), unames.cend())) {
            appendError(Validator::ErrorType::INCORRECT_TYPE);
            return false;
        }

        FieldConstPtrArray const & rfields = ref.getFields();
        FieldConstPtrArray const & ufields = un.getFields();

        bool fields_ok = true;
        for (size_t i = 0; i < numfields; ++i) {
            path.push_back(rnames[i]);
            fields_ok = fields_ok && validate(*rfields[i], *ufields[i]);
            path.pop_back();
        }

        return fields_ok;
    }

    bool validate(Structure const & ref, Structure const & struc)
    {
        if (&struc == &ref)
            return true;

        StringArray const & rnames = ref.getFieldNames();
        StringArray const & snames = struc.getFieldNames();

        // TODO: This is naive O(N^2), replace with better algorithm
        // TODO: This assumes getField doesn't fail (there's no Field::getFieldT)
        bool fields_ok = true;
        StringArray::const_iterator ri;
        for (ri = rnames.cbegin(); ri != rnames.cend(); ++ri) {

            FieldConstPtr rfield(ref.getField(*ri));

            if (std::find(snames.cbegin(), snames.cend(), *ri) == snames.end()) {
                if (isOptional(*rfield))
                    continue;

                appendError(Validator::ErrorType::MISSING_FIELD, *ri);
                fields_ok = false;
            } else {
                path.push_back(*ri);

                if (!validate(*rfield, *struc.getField(*ri))) {
                    appendError(Validator::ErrorType::INCORRECT_TYPE, *ri);
                    fields_ok = false;
                }

                path.pop_back();
            }
        }

        return fields_ok;
    }

    bool validate(Field const & reference, Field const & field)
    {
        Type referenceType = reference.getType();
        if (referenceType != field.getType())
            return false;

        switch(referenceType) {
        case Type::scalar:
        case Type::scalarArray:
            return true;

        case Type::structure:
            return validate(
                static_cast<Structure const &>(reference),
                static_cast<Structure const &>(field)
            );

        case Type::structureArray:
            return validate(
                *static_cast<StructureArray const &>(reference).getStructure(),
                *static_cast<StructureArray const &>(field).getStructure()
            );

        case Type::union_:
            return validate(
                static_cast<Union const &>(reference),
                static_cast<Union const &>(field)
            );

        case Type::unionArray:
            return validate(
                *static_cast<UnionArray const &>(reference).getUnion(),
                *static_cast<UnionArray const &>(field).getUnion()
            );

        default:
            throw std::logic_error("Unknown Field type");
            return false;
            }
    }

    bool validate(Field const & field)
    {
        return validate(*definition.structure, field);
    }
};

Validator::Result Validator::validate(Validator::Definition const & definition,
        Field const & field)
{
    Helper helper(definition);
    helper.validate(field);
    return helper.result;
}

bool Validator::isCompatible(Validator::Definition const & definition,
        Field const & field)
{
    return Helper(definition).validate(field);
}

bool Validator::isCompatible(Validator::Definition const & definition,
        StructureConstPtr const & structure)
{
    FieldConstPtr field(std::static_pointer_cast<const Field>(structure));
    return Validator::isCompatible(definition, *field);
}

std::ostream& operator<<(std::ostream& o, const Validator::Error& error)
{
    switch (error.type) {
    case Validator::ErrorType::MISSING_FIELD:
        return o << "Missing field '" << error.path << "'";
    case Validator::ErrorType::INCORRECT_TYPE:
        return o << "Field '" << error.path << "' has incorrect type";
    default:
        return o << "Unknown error " << error.type << " in field '"
                 << error.path << "'";
    }
}

}}
