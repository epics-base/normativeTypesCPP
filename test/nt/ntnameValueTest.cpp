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
#include <ctime>
#include <list>
#include <iostream>

#include <epicsAssert.h>

#include <pv/nt.h>
#include <pv/sharedVector.h>

using namespace epics::pvData;
using std::string;
using std::cout;
using std::endl;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();

static void test(FILE * fd)
{
    NTNameValuePtr ntNameValue = NTNameValue::create(true,true,true);
    PVStructurePtr pvStructure = ntNameValue->getPVStructure();
    cout << *pvStructure << endl;
    cout << *pvStructure->getStructure() << endl;
    PVStringArrayPtr names = ntNameValue->getNames();
    PVStringArrayPtr values = ntNameValue->getValues();
    size_t n = 2;
    shared_vector<string> name(n);
    shared_vector<string> value(n);
    name[0] = "name 0";
    name[1] = "name 1";
    value[0] = "value 0";
    value[1] = "value 1";
    names->replace(freeze(name));
    values->replace(freeze(value));
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
    cout << *pvStructure << endl;
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

