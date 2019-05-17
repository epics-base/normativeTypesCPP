/* ntvalidator.cpp */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

#include <sstream>
#include <algorithm>
#include <deque>

#include "validator.h"

#define epicsExportSharedSymbols
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
using std::set;

struct Validator::Helper {
    Validator const & validator;
    std::deque<string> path;
    Validator::Result result;

    Helper(Validator const & validator);
    bool isOptional(Field const & field) const;
    string getCurrentPath(void) const;

    void appendError(Validator::ErrorType type);
    void appendError(Validator::ErrorType type, string const & field_name);
    void appendError(Validator::ErrorType type, string const & ref_field_name,
        string const & field_name);

    bool validate(Union const & ref, Union const & un);
    bool validate(Structure const & ref, Structure const & struc);
    bool validate(Field const & reference, Field const & field);
    bool validate(Field const & field);
};

Validator::Helper::Helper(Validator const & validator) : validator(validator) {}

bool Validator::Helper::isOptional(Field const & field) const
{
    return validator.optional.find(&field) != validator.optional.end();
}

string Validator::Helper::getCurrentPath(void) const
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

void Validator::Helper::appendError(Validator::ErrorType type)
{
     result.errors.push_back(Validator::Error(getCurrentPath(), type));
}

void Validator::Helper::appendError(Validator::ErrorType type, string const & field_name)
{
    path.push_back(field_name);
    appendError(type);
    path.pop_back();
}

void Validator::Helper::appendError(Validator::ErrorType type, string const & ref_field_name,
    string const & field_name)
{
    path.push_back(ref_field_name);
    string ref_field_path = getCurrentPath();
    path.pop_back();

    path.push_back(field_name);
    string field_path(getCurrentPath());
    path.pop_back();

    result.errors.push_back(Validator::Error(ref_field_path, field_path, type));
}

bool Validator::Helper::validate(Union const & ref, Union const & un)
{
    if (&un == &ref)
        return true;

    if (ref.isVariant() != un.isVariant()) {
        appendError(Validator::ErrorType::INCORRECT_TYPE);
        return false;
    }

    if (ref.isVariant())
        return true;

    StringArray const & rnames = ref.getFieldNames();
    StringArray const & unames = un.getFieldNames();

    FieldConstPtrArray const & rfields = ref.getFields();
    FieldConstPtrArray const & ufields = un.getFields();

    size_t numRefFields = ref.getNumberFields();
    size_t numUnFields = un.getNumberFields();

    // Extra fields are OK
    bool ok = numRefFields <= numUnFields;

    size_t i = 0;
    size_t N = std::min(numRefFields, numUnFields);
    for (; i < N; ++i) {
        string const & rname(rnames[i]);
        string const & uname(unames[i]);

        if (rname != uname) {
            appendError(Validator::ErrorType::MISMATCHED_NAME, rname, uname);
            ok = false;
            continue;
        }

        path.push_back(rname);
        ok = ok && validate(*rfields[i], *ufields[i]);
        path.pop_back();
    }

    for (; i < numRefFields; ++i)
        appendError(Validator::ErrorType::MISSING_FIELD, rnames[i]);

    return ok;
}

bool Validator::Helper::validate(Structure const &ref, Structure const &struc)
{
    if (&struc == &ref)
        return true;

    StringArray const &rnames = ref.getFieldNames();
    StringArray const &snames = struc.getFieldNames();

    // TODO: This is naive O(N^2), replace with better algorithm (cache?)
    // TODO: This assumes getField doesn't fail (there's no Field::getFieldT (yet?))
    bool ok = true;
    StringArray::const_iterator ri;
    for (ri = rnames.cbegin(); ri != rnames.cend(); ++ri) {
        FieldConstPtr rfield(ref.getField(*ri));

        if (std::find(snames.cbegin(), snames.cend(), *ri) == snames.end()) {
            if (isOptional(*rfield))
                continue;

            appendError(Validator::ErrorType::MISSING_FIELD, *ri);
            ok = false;
        } else {
            path.push_back(*ri);
            if (!validate(*rfield, *struc.getField(*ri)))
            {
                appendError(Validator::ErrorType::INCORRECT_TYPE, *ri);
                ok = false;
            }
            path.pop_back();
        }
    }

    return ok;
}

bool Validator::Helper::validate(Field const &reference, Field const &field)
{
    Type referenceType = reference.getType();
    if (referenceType != field.getType())
        return false;

    switch (referenceType) {
    case Type::scalar:
    case Type::scalarArray:
        return true;

    case Type::structure:
        return validate(
            static_cast<Structure const &>(reference),
            static_cast<Structure const &>(field));

    case Type::structureArray:
        return validate(
            *static_cast<StructureArray const &>(reference).getStructure(),
            *static_cast<StructureArray const &>(field).getStructure());

    case Type::union_:
        return validate(
            static_cast<Union const &>(reference),
            static_cast<Union const &>(field));

    case Type::unionArray:
        return validate(
            *static_cast<UnionArray const &>(reference).getUnion(),
            *static_cast<UnionArray const &>(field).getUnion());

    default:
        throw std::logic_error("Unknown Field type");
        return false;
    }
}

bool Validator::Helper::validate(Field const &field)
{
    return validate(*validator.reference, field);
}

std::ostream& Validator::Error::dump(std::ostream& o) const {
    switch (type) {
    case ErrorType::MISSING_FIELD:
        return o << "Missing field '" << ref_path << "'";
    case ErrorType::INCORRECT_TYPE:
        return o << "Field '" << ref_path << "' has incorrect type";
    case ErrorType::MISMATCHED_NAME:
        return o << "Expected field '" << ref_path
                 << "' in Union, got '" << path << "'";
    default:
        return o << "Unknown error " << type << " in field '"
                 << ref_path << "'";
    }
}

Validator::Validator(FieldConstPtr const & reference)
: reference(reference) {
    if (!reference)
        throw std::logic_error("reference structure must not be NULL");
}

Validator::Validator(FieldConstPtr const & reference,
    set<Field const *> const & optional)
: reference(reference), optional(optional) {
    if (!reference)
        throw std::logic_error("reference structure must not be NULL");
}


Validator::Result Validator::validate(Field const & field) const
{
    Helper helper(*this);
    helper.validate(field);
    return helper.result;
}

bool Validator::isCompatible(Field const & field) const
{
    return Helper(*this).validate(field);
}

std::ostream& operator<<(std::ostream& o, const Validator::Error& error)
{
    return error.dump(o);
}

}}
