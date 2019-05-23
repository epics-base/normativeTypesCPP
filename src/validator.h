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

struct Result {
    struct Error {
        std::string path;
        enum Type {
            MissingField,
            IncorrectType,
            IncorrectId,
        } type;

        Error(std::string const & ref_path, Type type)
        : path(), type(type) {}

        bool operator==(const Error& other) const {
            return type == other.type && path == other.path;
        }

        std::ostream& dump(std::ostream& os) const {
            os << "Error(path=" << (path.empty() ? "<root>" : path) << ": ";

            switch(type) {
                case MissingField:  os << "missing";        break;
                case IncorrectType: os << "incorrect type"; break;
                case IncorrectId:   os << "incorrect ID";   break;
            }
            os << ")";
            return os;
        }
    };

    const epics::pvData::FieldConstPtr f;
    const std::string path;
    std::vector<Error> errors;

    enum result_t {
        Pass,
        Fail,
    } result;

    explicit Result(const epics::pvData::FieldConstPtr& f, const std::string& path = std::string())
    : f(f), path(path), errors(), result(Pass) {}

    Result& operator|=(const Result& other) {
        result = std::max(result, other.result);
        errors.insert(errors.end(), other.errors.begin(), other.errors.end());
        return *this;
    }

    bool valid(void) const {
        return result == Pass;
    }

    template<typename T>
    Result& is(void) {
        if (!dynamic_cast<T const *>(f.get())) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectType));
        }
        return *this;
    }

    template<typename T>
    Result& is(const std::string& id) {
        T const *s = dynamic_cast<T const *>(f.get());
        if (!s) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectType));
        } else if (s->getID() != id) {
            result = Fail;
            errors.push_back(Error(path, Error::IncorrectId));
        }
        return *this;
    }

    template<Result& (*fn)(Result&)>
    Result& has(const std::string& name) {
        return has<epics::pvData::Field>(name, false, fn);
    }

    template<Result& (*fn)(Result&)>
    Result& maybeHas(const std::string& name) {
        return has<epics::pvData::Field>(name, true, fn);
    }

    template<typename T>
    Result& has(const std::string& name) {
        return has<T>(name, false, NULL);
    }

    template<typename T>
    Result& maybeHas(const std::string& name) {
        return has<T>(name, true, NULL);
    }

    std::ostream& dump(std::ostream& os) const {
        os << "Result(valid=" << valid() << ", errors=[ ";
        
        std::vector<Error>::const_iterator it;
        for (it = errors.cbegin(); it != errors.cend(); ++it) {
            (*it).dump(os);
            os << " ";
        }
        os << "])";
        return os;
    }

private:
    template<typename T>
    Result& has(const std::string& name, bool optional, Result& (*check)(Result&) = NULL) {
        epics::pvData::FieldConstPtr field;

        switch(f->getType()) {
            case epics::pvData::Type::structure:
                field = static_cast<epics::pvData::Structure const *>(f.get())->getField(name);
                break;
            case epics::pvData::Type::structureArray:
                field = static_cast<epics::pvData::StructureArray const *>(f.get())->getStructure()->getField(name);
                break;
            case epics::pvData::Type::union_:
                field = static_cast<epics::pvData::Union const *>(f.get())->getField(name);
                break;
            case epics::pvData::Type::unionArray:
                field = static_cast<epics::pvData::UnionArray const *>(f.get())->getUnion()->getField(name);
                break;
            default:
                // Expected a structure-like Field
                result = Fail;
                errors.push_back(Error(path, Error::Type::IncorrectType));
                return *this;
        }

        if (!field) {
            if (!optional) {
                result = Fail;
                errors.push_back(Error(path + "." + name, Error::Type::MissingField));
            }
        } else if (!dynamic_cast<T const *>(field.get())) {
            result = Fail;
            errors.push_back(Error(path + "." + name, Error::Type::IncorrectType));
        } else if (check) {
            Result r(field, path + "." + name);
            *this |= check(r);
        }

        return *this;
    }
};
}}

#endif