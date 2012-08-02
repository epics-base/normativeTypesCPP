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

using namespace epics::pvData;

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardFieldPtr standardField = getStandardField();
static StandardPVFieldPtr standardPVField = getStandardPVField();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();
static String builder;

static void testNTField(FILE * fd)
{
    StructureConstPtr structureConstPtr = ntField->createEnumerated();
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
        = ntField->createEnumeratedArray();
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    structureArrayConstPtr = ntField->createTimeStampArray();
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    structureArrayConstPtr = ntField->createAlarmArray();
    builder.clear();
    structureArrayConstPtr->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
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
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isEnumerated(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createTimeStamp());
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isTimeStamp(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createAlarm());
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarm(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createDisplay());
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isDisplay(pvStructure->getStructure()));

    pvStructure = PVStructurePtr(pvntField->createAlarmLimit());
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(ntField->isAlarmLimit(pvStructure->getStructure()));

    PVStructureArrayPtr pvStructureArray = PVStructureArrayPtr(
        pvntField->createEnumeratedArray());
    builder.clear();
    pvStructureArray->toString(&builder);
    fprintf(fd,"\n%s\n",builder.c_str());
    builder.clear();
    pvStructureArray->getStructureArray()->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    pvStructureArray = PVStructureArrayPtr(
        pvntField->createTimeStampArray());
    builder.clear();
    pvStructureArray->toString(&builder);
    fprintf(fd,"\n%s\n",builder.c_str());
    builder.clear();
    pvStructureArray->getStructureArray()->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());

    pvStructureArray = PVStructureArrayPtr(
        pvntField->createAlarmArray());
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
    testNTField(fd);
    testPVNTField(fd);
    return(0);
}

