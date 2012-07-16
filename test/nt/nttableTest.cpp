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

#include <epicsExit.h>
#include <pv/CDRMonitor.h>

using namespace epics::pvData;


static FieldCreate * fieldCreate = 0;
static PVDataCreate * pvDataCreate = 0;
static NTField *ntField = 0;
static PVNTField *pvntField = 0;
static String builder("");

static void test(FILE * fd)
{
    int n = 2;
    FieldConstPtr fields[2];
    fields[0] = fieldCreate->createScalarArray("position",pvDouble);
    fields[1] = ntField->createAlarmArray("alarms");
    PVStructure::shared_pointer pvStructure = NTTable::create(
        true,true,true,n,fields);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    builder.clear();
    pvStructure->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    NTTable ntTable(pvStructure);
    PVDoubleArray *pvPositions
        = static_cast<PVDoubleArray *>(ntTable.getPVField(0));
    double positions[2];
    positions[0] = 1.0;
    positions[1] = 2.0;
    pvPositions->put(0,n,positions,0);
    PVStructureArray *pvAlarms
        = static_cast<PVStructureArray *>(ntTable.getPVField(1));
    PVAlarm pvAlarm;
    Alarm alarm;
    PVStructurePtr palarms[n];
    for(int i=0; i<n; i++) {
        palarms[i] = pvntField->createAlarm(0);
        pvAlarm.attach(palarms[i]);
        alarm.setMessage("test");
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        pvAlarm.set(alarm);
    }
    pvAlarms->put(0,n,palarms,0);
    String labels[n];
    labels[0] = pvPositions->getField()->getFieldName();
    labels[1] = pvAlarms->getField()->getFieldName();
    PVStringArray *label = ntTable.getLabel();
    label->put(0,n,labels,0);
    
    PVString *function = ntTable.getFunction();
    function->put("test");
    ntTable.attachAlarm(pvAlarm);
    alarm.setMessage("test alarm");
    alarm.setSeverity(majorAlarm);
    alarm.setStatus(clientStatus);
    pvAlarm.set(alarm);
    PVTimeStamp pvTimeStamp;
    ntTable.attachTimeStamp(pvTimeStamp);
    TimeStamp timeStamp(1000,1000,10);
    pvTimeStamp.set(timeStamp);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(NTTable::isNTTable(pvStructure.get()));
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
    ntField = NTField::get();
    pvntField = PVNTField::get();
    test(fd);
    epicsExitCallAtExits();
    CDRMonitor::get().show(fd,true);
    return(0);
}

