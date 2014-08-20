/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/*
 * ntfieldTest.cpp
 *
 *  Created on: 2011.11
 *      Author: Marty Kraimer
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <ctime>
#include <list>
#include <iostream>

#include <epicsAssert.h>

#include <pv/nt.h>

using namespace epics::pvData;
using std::string;
using std::cout;
using std::endl;

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardFieldPtr standardField = getStandardField();
static StandardPVFieldPtr standardPVField = getStandardPVField();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();

static void testNTField(FILE * fd)
{
    StructureConstPtr structureConstPtr = ntField->createEnumerated();
    cout << *structureConstPtr << endl;
    assert(ntField->isEnumerated(structureConstPtr));

    structureConstPtr = ntField->createTimeStamp();
    cout << *structureConstPtr << endl;
    assert(ntField->isTimeStamp(structureConstPtr));

    structureConstPtr = ntField->createAlarm();
    cout << *structureConstPtr << endl;
    assert(ntField->isAlarm(structureConstPtr));

    structureConstPtr = ntField->createDisplay();
    cout << *structureConstPtr << endl;
    assert(ntField->isDisplay(structureConstPtr));

    structureConstPtr = ntField->createAlarmLimit();
    cout << *structureConstPtr << endl;
    assert(ntField->isAlarmLimit(structureConstPtr));

    structureConstPtr = ntField->createControl();
    cout << *structureConstPtr << endl;
    assert(ntField->isControl(structureConstPtr));

    StructureArrayConstPtr structureArrayConstPtr
        = ntField->createEnumeratedArray();
    cout << *structureConstPtr << endl;

    structureArrayConstPtr = ntField->createTimeStampArray();
    cout << *structureConstPtr << endl;

    structureArrayConstPtr = ntField->createAlarmArray();
    cout << *structureConstPtr << endl;
}

static void testPVNTField(FILE * fd)
{
    StringArray choices;
    choices.resize(3);
    choices[0] = "one";
    choices[1] = "two";
    choices[2] = "three";
    PVStructurePtr pvStructure = PVStructurePtr(
        pvntField->createEnumerated(choices));
    cout << *pvStructure << endl;
    assert(ntField->isEnumerated(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createTimeStamp());
    cout << *pvStructure << endl;
    assert(ntField->isTimeStamp(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createAlarm());
    cout << *pvStructure << endl;
    assert(ntField->isAlarm(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createDisplay());
    cout << *pvStructure << endl;
    assert(ntField->isDisplay(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createAlarmLimit());
    cout << *pvStructure << endl;
    assert(ntField->isAlarmLimit(pvStructure->getStructure()));

    PVStructureArrayPtr pvStructureArray = PVStructureArrayPtr(
        pvntField->createEnumeratedArray());
    cout << *pvStructure << endl;
    cout << *pvStructureArray->getStructureArray()->getStructure();

    pvStructureArray = PVStructureArrayPtr(
        pvntField->createTimeStampArray());
    cout << *pvStructure << endl;
    cout << *pvStructureArray->getStructureArray()->getStructure();

    pvStructureArray = PVStructureArrayPtr(
        pvntField->createAlarmArray());
    cout << *pvStructure << endl;
    cout << *pvStructureArray->getStructureArray()->getStructure();
}

int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    FILE * fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    testNTField(fd);
    testPVNTField(fd);
    return(0);
}

