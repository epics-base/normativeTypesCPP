/* validator.h */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <set>

#include <pv/pvIntrospect.h>


namespace epics { namespace nt {

/**
 * @brief Validation methods for NT types.
 *
 * @author bsm
 */

class Validator {
public:
    enum ErrorType { MISSING_FIELD, INCORRECT_TYPE, MISMATCHED_NAME };

    struct Helper;

    struct Error {
        std::string ref_path;
        std::string path;
        ErrorType type;

        Error(std::string const & ref_path, ErrorType type)
        : ref_path(ref_path), type(type) {}

        Error(std::string const & ref_path, std::string const & path, ErrorType type)
        : ref_path(ref_path), path(path), type(type) {}

        bool operator==(const Error& other) const {
            return type == other.type &&
                   ref_path == other.ref_path &&
                   path == other.path;
        }

        std::ostream& dump(std::ostream&) const;
    };

    struct Result {
        std::vector<Error> errors;
        bool valid(void) const { return errors.empty(); }
        std::ostream& dump(std::ostream&) const;
    };

    Validator(epics::pvData::FieldConstPtr const & reference);

    Validator(epics::pvData::FieldConstPtr const & reference,
        std::set<epics::pvData::Field const *> const & optional);

    Result validate(epics::pvData::Field const & field) const;

    /**
     * Checks whether a Field is compatible with a NT definition.
     * @param definition the definition of the reference structure
     * @param field the Field being tested for compatibility
     * @return true if field is compatible, false otherwise
     */
    bool isCompatible(epics::pvData::Field const & field) const;

private:
    friend struct Helper;
    epics::pvData::FieldConstPtr const reference;
    std::set<epics::pvData::Field const *> optional;
};

epicsShareExtern std::ostream& operator<<(std::ostream& o, const Validator::Error& error);

}}

#endif  /* VALIDATOR_H */
