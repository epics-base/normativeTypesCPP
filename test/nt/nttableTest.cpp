/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/*
 * nttableTest.cpp
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
using std::tr1::static_pointer_cast;

static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();
static String builder;

static void test(FILE * fd)
{
    size_t n = 2;
    FieldConstPtrArray fields(n);
    StringArray names(n);
    names[0] = "position";
    names[1] = "alarms";
    fields[0] = fieldCreate->createScalarArray(pvDouble);
    fields[1] = ntField->createAlarmArray();
    NTTablePtr ntTable = NTTable::create(
        true,true,true,names,fields);
    PVStructurePtr pvStructure = ntTable->getPVStructure();
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    builder.clear();
    pvStructure->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    PVDoubleArrayPtr pvPositions
        = static_pointer_cast<PVDoubleArray>(ntTable->getPVField(0));
    DoubleArray positions(2);
    positions[0] = 1.0;
    positions[1] = 2.0;
    pvPositions->put(0,2,positions,0);
    PVStructureArrayPtr pvAlarms
        = static_pointer_cast<PVStructureArray>(ntTable->getPVField(1));
    PVAlarm pvAlarm;
    Alarm alarm;
    PVStructurePtrArray palarms(n);
    for(size_t i=0; i<n; i++) {
        palarms[i] = pvntField->createAlarm();
        pvAlarm.attach(palarms[i]);
        alarm.setMessage("test");
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        pvAlarm.set(alarm);
    }
    pvAlarms->put(0,n,palarms,0);
    StringArray labels(n);
    labels[0] = pvPositions->getFieldName();
    labels[1] = pvAlarms->getFieldName();
    PVStringArrayPtr label = ntTable->getLabel();
    label->put(0,n,labels,0);
    PVStringPtr function = ntTable->getFunction();
    function->put("test");
    ntTable->attachAlarm(pvAlarm);
    alarm.setMessage("test alarm");
    alarm.setSeverity(majorAlarm);
    alarm.setStatus(clientStatus);
    pvAlarm.set(alarm);
    PVTimeStamp pvTimeStamp;
    ntTable->attachTimeStamp(pvTimeStamp);
    TimeStamp timeStamp(1000,1000,10);
    pvTimeStamp.set(timeStamp);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(NTTable::isNTTable(pvStructure));
}


int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    FILE * fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    test(fd);
    return(0);
}

