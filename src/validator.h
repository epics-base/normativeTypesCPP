/* ntvalidator.h */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */
#ifndef NTVALIDATOR_H
#define NTVALIDATOR_H

#include <string>
#include <set>

#include <shareLib.h>
#include <pv/pvIntrospect.h>


namespace epics { namespace nt {

/**
 * @brief Validation methods for NT types.
 *
 * @author bsm
 */

class epicsShareClass Validator {
public:
    struct Definition {
        epics::pvData::FieldConstPtr structure;
        std::set<epics::pvData::Field const *> optional;
    };

    enum ErrorType { MISSING_FIELD, INCORRECT_TYPE };

    struct Error {
        std::string path;
        ErrorType type;

        Error(std::string const & path, ErrorType type) : path(path), type(type) {};
    };

    struct Result {
        std::vector<Error> errors;
        bool valid(void) const { return errors.empty(); }
    };

    /**
     * Checks whether a Field is compatible with a NT definition.
     * @param definition the definition of the reference structure
     * @param field the Field being tested for compatibility
     * @return true if field is compatible, false otherwise
     */
    static Result validate(Definition const & definition,
            epics::pvData::Field const & field);

    static bool isCompatible(Definition const & definition,
            epics::pvData::Field const & field);

    static bool isCompatible(Definition const & definition,
            epics::pvData::StructureConstPtr const & structure);

private:
    Validator();
};

epicsShareExtern std::ostream& operator<<(std::ostream& o, const Validator::Error& error);

}}

#endif  /* NTVALIDATOR_H */
