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

class NTNameValue 
{
public:
    POINTER_DEFINITIONS(NTNameValue);
    /**
     * Is the pvStructure an NTNameValue.
     * @param pvStructure The pvStructure to test.
     * @return (false,true) if (is not, is) an NTNameValue.
     */
    static bool isNTNameValue(PVStructurePtr pvStructure);
    /**
     * Create an  NTNameValue pvStructure.
     * @param hasFunction Create a PVString field named function.
     * @param hasTimeStamp Create a timeStamp structure field.
     * @param hasAlarm Create an alarm structure field.
     * @return a NTNameValue pvStructure.
     */
    static PVStructure::shared_pointer create(
        bool hasFunction,bool hasTimeStamp, bool hasAlarm);
    /**
     * Constructor
     * @param pvStructure The pvStructure to which to attach.
     * @return A NTNameValue that is attached to the pvStructure
     */
    NTNameValue(PVStructure::shared_pointer const & pvStructure);
    /**
     * Destructor
     */
    ~NTNameValue();
    /**
     * Get the function field.
     * @return The pvString or null if no function field.
     */
    PVString* getFunction();
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
    PVStructurePtr getPVStructure(){return pvNTNameValue.get();}
    /**
     * Get the timeStamp.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr getTimeStamp(){return pvTimeStamp;}
    /**
     * Get the alarm.
     * @return PVStructurePtr which may be null.
     */
    PVStructurePtr getAlarm() {return pvAlarm;}
    /**
     * Get the string array on names.
     * @return The array of names.
     */
    PVStringArray *getNames();
    /**
     * Get the string array on values.
     * @return The array of values.
     */
    PVStringArray *getValues();
private:
    PVStructure::shared_pointer pvNTNameValue;
    PVString * pvFunction;
    PVStructurePtr pvTimeStamp;
    PVStructurePtr pvAlarm;
    PVStringArray *pvNames;
    PVStringArray *pvValues;
};

}}
#endif  /* NTNAMEVALUE_H */
