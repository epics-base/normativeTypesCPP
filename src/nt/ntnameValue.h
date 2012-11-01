/* ntnameValue.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNAMEVALUE_H
#define NTNAMEVALUE_H

#include <pv/ntfield.h>

namespace epics { namespace pvData { 

/**
 * Convenience Class for NTNameValue
 * @author mrk
 *
 */

class NTNameValue;
typedef std::tr1::shared_ptr<NTNameValue> NTNameValuePtr;

class NTNameValue 
{
public:
    POINTER_DEFINITIONS(NTNameValue);
    /**
     * Is the pvStructure an NTNameValue.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTNameValue.
     */
    static bool isNTNameValue(PVStructurePtr const & pvStructure);
    /**
     * Create an  NTNameValue pvStructure.
     * @param hasFunction Create a PVString field named function.
     * @param hasTimeStamp Create a timeStamp structure field.
     * @param hasAlarm Create an alarm structure field.
     * @return  NTNameValuePtr
     */
    static NTNameValuePtr create(
        bool hasFunction,bool hasTimeStamp, bool hasAlarm);
    static NTNameValuePtr create(
        PVStructurePtr const & pvStructure);
    /**
     * Destructor
     */
    ~NTNameValue() {}
    /**
     * Get the function field.
     * @return The pvString or null if no function field.
     */
    PVStringPtr & getFunction() {return pvFunction;}
    /**
     * Attach a pvTimeStamp.
     * @param pvTimeStamp The pvTimeStamp that will be attached.
     * Does nothing if no timeStamp
     */
    void attachTimeStamp(PVTimeStamp &pvTimeStamp);
    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm
     */
    void attachAlarm(PVAlarm &pvAlarm);
    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    PVStructurePtr & getPVStructure(){return pvNTNameValue;}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr & getTimeStamp(){return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr & getAlarm() {return pvAlarm;}
    /**
     * Get the string array on names.
     * @return The array of names.
     */
    PVStringArrayPtr & getNames() {return pvNames;}
    /**
     * Get the string array on values.
     * @return The array of values.
     */
    PVStringArrayPtr & getValues() {return pvValues;}
private:
    NTNameValue(PVStructurePtr const & pvStructure);
    PVStructurePtr pvNTNameValue;
    PVStringPtr pvFunction;
    PVStructurePtr pvTimeStamp;
    PVStructurePtr pvAlarm;
    PVStringArrayPtr pvNames;
    PVStringArrayPtr pvValues;
};

}}
#endif  /* NTNAMEVALUE_H */
