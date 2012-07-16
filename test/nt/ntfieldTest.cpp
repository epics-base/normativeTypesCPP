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
#include <cstring>
#include <cstdio>
#include <ctime>
#include <list>

#include <epicsAssert.h>

#include <pv/nt.h>

#include <epicsExit.h>
#include <pv/CDRMonitor.h>

using namespace epics::pvData;

static FieldCreate * fieldCreate = 0;
static PVDataCreate * pvDataCreate = 0;
static StandardField *standardField = 0;
static StandardPVField *standardPVField = 0;
static NTField *ntField = 0;
static PVNTField *pvntField = 0;
static String builder("");

static void testNTField(FILE * fd)
{
    StructureConstPtr structureConstPtr = ntField->createEnumerated("value");
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isEnumerated(structureConstPtr));

    structureConstPtr = ntField->createTimeStamp();
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isTimeStamp(structureConstPtr));

    structureConstPtr = ntField->createAlarm();
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarm(structureConstPtr));

    structureConstPtr = ntField->createDisplay();
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isDisplay(structureConstPtr));

    structureConstPtr = ntField->createAlarmLimit();
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarmLimit(structureConstPtr));

    structureConstPtr = ntField->createControl();
    builder.clear();
    structureConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isControl(structureConstPtr));

    StructureArrayConstPtr structureArrayConstPtr
        = ntField->createEnumeratedArray("value");
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    structureArrayConstPtr = ntField->createTimeStampArray("value");
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    structureArrayConstPtr = ntField->createAlarmArray("value");
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
}

static void testPVNTField(FILE * fd)
{
    String choices[] = {"one","two","three"};
    PVStructure::shared_pointer pvStructure = PVStructure::shared_pointer(
        pvntField->createEnumerated(0,"enumerated",choices, 3));
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isEnumerated(pvStructure->getStructure()));

    pvStructure = PVStructure::shared_pointer(pvntField->createTimeStamp(0));
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isTimeStamp(pvStructure->getStructure()));

    pvStructure = PVStructure::shared_pointer(pvntField->createAlarm(0));
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarm(pvStructure->getStructure()));

    pvStructure = PVStructure::shared_pointer(pvntField->createDisplay(0));
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isDisplay(pvStructure->getStructure()));

    pvStructure = PVStructure::shared_pointer(pvntField->createAlarmLimit(0));
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarmLimit(pvStructure->getStructure()));

    PVStructureArray::shared_pointer pvStructureArray = PVStructureArray::shared_pointer(
        pvntField->createEnumeratedArray(0,"enumArray"));
    builder.clear();
    pvStructureArray->toString(&builder);
    fprintf(fd,"\n%s\n",builder.c_str());
    builder.clear();
    pvStructureArray->getStructureArray()->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    pvStructureArray = PVStructureArray::shared_pointer(
        pvntField->createTimeStampArray(0,"timeStampArray"));
    builder.clear();
    pvStructureArray->toString(&builder);
    fprintf(fd,"\n%s\n",builder.c_str());
    builder.clear();
    pvStructureArray->getStructureArray()->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    pvStructureArray = PVStructureArray::shared_pointer(
        pvntField->createAlarmArray(0,"alarmArray"));
    builder.clear();
    pvStructureArray->toString(&builder);
    fprintf(fd,"\n%s\n",builder.c_str());
    builder.clear();
    pvStructureArray->getStructureArray()->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
}

int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    FILE * fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    fieldCreate = getFieldCreate();
    pvDataCreate = getPVDataCreate();
    standardField = getStandardField();
    standardPVField = getStandardPVField();
    ntField = NTField::get();
    pvntField = PVNTField::get();
    testNTField(fd);
    testPVNTField(fd);
    epicsExitCallAtExits();
    CDRMonitor::get().show(fd,true);
    return(0);
}

