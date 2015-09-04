/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTID_H
#define NTID_H

#include <string>

namespace epics { 

namespace nt {

/**
 * Utility class for parsing an ID following the NT type ID conventions
 *
 * An NT type ID will be of the from epics:nt/<type-name>:<Major>.<Minor>,
 * e.g. epics:nt/NTNDArray:1.2
 * @author dgh
 */
class NTID
{
public:
    /**
     * Creates an NTID from the specified type ID
     *
     * @param id The the id to be parsed.
     * @return NTNDArray instance on success, null otherwise.
     */
    NTID(const std::string &id);
    /**
     * Get the full name of the id, i.e. the original ID
     *
     * For example above returns "epics:nt/NTNDArray:1.2"
     * @return the full name
     */
    std::string getFullName();

    /**
     * Get the fully qualified name including namespaces, but excluding version numbers
     *
     * For example above return "epics:nt/NTNDArray"
     * @return the fully qualified name
     */ 
    std::string getQualifiedName();

    /**
     * Get the namespace
     *
     * For example above return "epics:nt"
     * @return the namespace
     */
    std::string getNamespace();

    /**
     * Get the unqualified name, without namespace or version
     *
     * For example above return "NTNDArray"
     * @return the unqualified name
     */
    std::string getName();

    /**
     * Get the unqualified name, without namespace or version
     *
     * For example above return "NTNDArray"
     * @return the unqualified name
     */
    std::string getVersion();

    /**
     * Get the Major version as a string
     *
     * For example above return "1"
     * @return the Major string
     */
    std::string getMajorVersionString();

    /**
     * Does the ID contain a major version and is it a number
     *
     * @return true if it contains a major version number
     */
    bool hasMajorVersion();

    /**
     * Get the Major version as an integer
     *
     * For example above return 1
     * @return the Major string
     */
    int getMajorVersion();

    /**
     * Get the Major version as a string
     *
     * For example above return "1"
     * @return the Major string
     */
    std::string getMinorVersionString();

    /**
     * Does the ID contain a minor version and is it a number
     *
     * @return true if it contains a minor version number
     */
    bool hasMinorVersion();

    /**
     * Get the Minor version as an integer
     *
     * For example above return 1
     * @return the Minor string
     */
    int getMinorVersion();

private:
    std::string fullName;
    std::string qualifiedName;
    std::string namespaceStr;
    std::string name;
    std::string version;

    size_t nsSepIndex;
    size_t versionSepIndex;
    bool nsQualified;
    bool hasVersion;

    size_t endMajorIndex;
    std::string majorVersionStr;
    bool majorVersionParsed;
    bool hasMajor;
    int majorVersion;

    size_t endMinorIndex;
    std::string minorVersionStr;
    bool minorVersionParsed;
    bool hasMinor;
    int minorVersion;

};

}}

#endif

