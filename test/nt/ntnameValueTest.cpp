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

using namespace epics::pvData;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();
static String builder;

static void test(FILE * fd)
{
    NTNameValuePtr ntNameValue = NTNameValue::create(true,true,true);
    PVStructurePtr pvStructure = ntNameValue->getPVStructure();
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    builder.clear();
    pvStructure->getStructure()->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    PVStringArrayPtr names = ntNameValue->getNames();
    PVStringArrayPtr values = ntNameValue->getValues();
    size_t n = 2;
    StringArray name;
    StringArray value;
    name.resize(n);
    value.resize(n);
    name[0] = "name 0";
    name[1] = "name 1";
    value[0] = "value 0";
    value[1] = "value 1";
    names->put(0,n,name,0);
    values->put(0,n,value,0);
    PVStringPtr function = ntNameValue->getFunction();
    function->put("test");
    PVAlarm pvAlarm;
    ntNameValue->attachAlarm(pvAlarm);
    Alarm alarm;
    alarm.setMessage("test alarm");
    alarm.setSeverity(majorAlarm);
    alarm.setStatus(clientStatus);
    pvAlarm.set(alarm);
    PVTimeStamp pvTimeStamp;
    ntNameValue->attachTimeStamp(pvTimeStamp);
    TimeStamp timeStamp(1000,1000,10);
    pvTimeStamp.set(timeStamp);
    builder.clear();
    pvStructure->toString(&builder);
    fprintf(fd,"%s\n",builder.c_str());
    assert(NTNameValue::isNTNameValue(pvStructure));
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

