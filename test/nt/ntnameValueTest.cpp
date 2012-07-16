/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/*
 * ntnameValueTest.cpp
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


static PVDataCreate * pvDataCreate = 0;
static NTField *ntField = 0;
static PVNTField *pvntField = 0;
static String builder("");

static void test(FILE * fd)
{
    PVStructure::shared_pointer pvStructure = NTNameValue::create(true,true,true);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    builder.clear();
    pvStructure->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    NTNameValue ntNameValue(pvStructure);
    PVStringArray *names = ntNameValue.getNames();
    PVStringArray *values = ntNameValue.getValues();
    int n = 2;
    String name[] = {String("name 0"),String("name 1")};
    String value[] = {String("value 0"),String("value 1")};
    names->put(0,n,name,0);
    values->put(0,n,value,0);
    PVString *function = ntNameValue.getFunction();
    function->put("test");
    PVAlarm pvAlarm;
    ntNameValue.attachAlarm(pvAlarm);
    Alarm alarm;
    alarm.setMessage("test alarm");
    alarm.setSeverity(majorAlarm);
    alarm.setStatus(clientStatus);
    pvAlarm.set(alarm);
    PVTimeStamp pvTimeStamp;
    ntNameValue.attachTimeStamp(pvTimeStamp);
    TimeStamp timeStamp(1000,1000,10);
    pvTimeStamp.set(timeStamp);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(NTNameValue::isNTNameValue(pvStructure.get()));
    NTNameValue::shared_pointer pntNameValue
        = NTNameValue::shared_pointer(new NTNameValue(pvStructure));
}


int main(int argc,char *argv[])
{
    char *fileName = 0;
    if(argc>1) fileName = argv[1];
    FILE * fd = stdout;
    if(fileName!=0 && fileName[0]!=0) {
        fd = fopen(fileName,"w+");
    }
    pvDataCreate = getPVDataCreate();
    ntField = NTField::get();
    pvntField = PVNTField::get();
    test(fd);
    epicsExitCallAtExits();
    CDRMonitor::get().show(fd,true);
    return(0);
}

