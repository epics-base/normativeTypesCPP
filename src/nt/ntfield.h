/* ntfield.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTFIELD_H
#define NTFIELD_H

#include <cstdarg>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

namespace epics { namespace pvData { 
/**
 * Convenience Class for introspection fields of a Normative Type
 * @author mrk
 * 
 */
class NTField: NoDefaultMethods {
public:
    /**
     * get the single implementation of this class.
     * @return the implementation
     */
    static NTField * get();
    /**
     * destructor
     */
    ~NTField() {}
    /**
     * Is field an enumerated structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an enumerated structure.
     */
    bool isEnumerated(FieldConstPtr field);
    /**
     * Is field a timeStamp structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a timeStamp structure.
     */
    bool isTimeStamp(FieldConstPtr field);
    /**
     * Is field an alarm structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarm structure.
     */
    bool isAlarm(FieldConstPtr field);
    /**
     * Is field a display structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a display structure.
     */
    bool isDisplay(FieldConstPtr field);
    /**
     * Is field an alarmLimit structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) an alarmLimit structure.
     */
    bool isAlarmLimit(FieldConstPtr field);
    /**
     * Is field a control structure.
     * @param field The field to test.
     * @return (false,true) if field (is not,is) a control structure.
     */
    bool isControl(FieldConstPtr field);

    /**
     * Create an enumerated structure.
     * @param fieldName The fieldName for the structure.
     * @return an enumerated structure.
     */
    StructureConstPtr createEnumerated(String fieldName);
    /**
     * Create a timeStamp structure.
     * @return a timeStamp structure.
     */
    StructureConstPtr createTimeStamp();
    /**
     * Create an alarm structure.
     * @return an alarm structure.
     */
    StructureConstPtr createAlarm();
    /**
     * Create a display structure.
     * @return a displayalarm structure.
     */
    StructureConstPtr createDisplay();
    /**
     * Create an alarmLimit structure.
     * @return an alarmLimit structure.
     */
    StructureConstPtr createAlarmLimit();
    /**
     * Create a control structure.
     * @return a control structure.
     */
    StructureConstPtr createControl();

    /**
     * Create an array of enumerated structures.
     * @param fieldName The fieldName for the array.
     * @return an array of enumerated structures.
     */
    StructureArrayConstPtr createEnumeratedArray(String fieldName);
    /**
     * Create an array of timeStamp structures.
     * @param fieldName The fieldName for the array.
     * @return an array of timeStamp structures.
     */
    StructureArrayConstPtr createTimeStampArray(String fieldName);
    /**
     * Create an array of alarm structures.
     * @param fieldName The fieldName for the array.
     * @return an array of alarm structures.
     */
    StructureArrayConstPtr createAlarmArray(String fieldName);
private:
    NTField();
    FieldCreate *fieldCreate;
    StandardField *standardField;
};

/**
 * Convenience Class for data fields of a Normative Type
 * @author mrk
 * 
 */
class PVNTField: NoDefaultMethods {
public:
    /**
     * get the single implementation of this class.
     * @return the implementation
     */
    static PVNTField * get();
    /**
     * destructor
     */
    ~PVNTField() {}
    /**
     * Create an enumerated PVStructure.
     * @param parent The parent structure.
     * @param fieldName The fieldName for the structure.
     * @param choices The array of choices.
     * @param numberChoices The number of choices.
     * @return an enumerated PVStructure..
     */
    PVStructurePtr createEnumerated(
        PVStructurePtr parent,
        String fieldName,
        StringArray choices,
        int numberChoices);
    /**
     * Create a timeStamp PVStructure.
     * @param parent The parent structure.
     * @return a timeStamp PVStructure..
     */
    PVStructurePtr createTimeStamp(PVStructurePtr parent);
    /**
     * Create an alarm PVStructure.
     * @param parent The parent structure.
     * @return an alarm PVStructure..
     */
    PVStructurePtr createAlarm(PVStructurePtr parent);
    /**
     * Create a display PVStructure.
     * @param parent The parent structure.
     * @return a display PVStructure..
     */
    PVStructurePtr createDisplay(PVStructurePtr parent);
    /**
     * Create an alarmLimit PVStructure.
     * @param parent The parent structure.
     * @return an alarmLimit PVStructure..
     */
    PVStructurePtr createAlarmLimit(PVStructurePtr parent);
    /**
     * Create a control PVStructure.
     * @param parent The parent structure.
     * @return a control PVStructure..
     */
    PVStructurePtr createControl(PVStructurePtr parent);

    /**
     * Create an enumerated PVStructureArray.
     * @param parent The parent structure.
     * @param fieldName The fieldName for the structure.
     * @return an enumerated PVStructureArray..
     */
    PVStructureArray * createEnumeratedArray(
        PVStructurePtr parent,String fieldName);
    /**
     * Create a timeStamp PVStructureArray.
     * @param parent The parent structure.
     * @param fieldName The fieldName for the structure.
     * @return a timeStamp PVStructureArray
     */
    PVStructureArray * createTimeStampArray(
        PVStructurePtr parent,String fieldName);
    /**
     * Create an alarm PVStructureArray.
     * @param parent The parent structure.
     * @param fieldName The fieldName for the structure.
     * @return an alarm PVStructureArray..
     */
    PVStructureArray * createAlarmArray(
        PVStructurePtr parent,String fieldName);
private:
    PVNTField();
    PVDataCreate *pvDataCreate;
    StandardPVField *standardPVField;
    NTField *ntstructureField;
};

}}
#endif  /* NTFIELD_H */
