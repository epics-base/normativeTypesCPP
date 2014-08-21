/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/*
 * ntnameValueTest.cpp
 *
 *  Created on: 2014.08
 *      Author: Marty Kraimer
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <list>
#include <iostream>

#include <epicsUnitTest.h>
#include <testMain.h>


#include <pv/nt.h>
#include <pv/sharedVector.h>
#include <pv/ntmultiChannel.h>

using namespace epics::pvData;
using namespace epics::nt;
using std::string;
using std::cout;
using std::endl;
using std::vector;

static bool debug = true;

static  FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static NTFieldPtr ntField = NTField::get();
static PVNTFieldPtr pvntField = PVNTField::get();

static void test()
{
    vector<string> optionNames(10);
    optionNames[0] = "alarm";
    optionNames[1] = "timeStamp";
    optionNames[2] = "severity";
    NTMultiChannelPtr multiChannel = NTMultiChannel::create(optionNames);
    testOk1(multiChannel.get()!=NULL);
    PVStructurePtr pvStructure = multiChannel->getPVStructure();
    testOk1(pvStructure.get()!=NULL);
    size_t nchan = 3;
    shared_vector<string> names(nchan);
    names[0] = "channel 0";
    names[1] = "channel 1";
    names[2] = "channel 2";
    shared_vector<const string> channelNames(freeze(names));
    PVStringArrayPtr pvChannelName = multiChannel->getChannelName();
    pvChannelName->replace(channelNames);
    if(debug) {cout << *pvStructure << endl;}
    UnionConstPtr unionPtr =
       fieldCreate->createFieldBuilder()->
           add("doubleValue", pvDouble)->
           add("intValue", pvInt)->
           createUnion();
    multiChannel = NTMultiChannel::create(
         optionNames,unionPtr,channelNames);
    testOk1(multiChannel.get()!=NULL);
    pvStructure = multiChannel->getPVStructure();
    if(debug) {cout << *pvStructure << endl;}
    PVUnionArrayPtr pvValue = multiChannel->getValue();
    shared_vector<PVUnionPtr> unions(2);
    unions[0] = pvDataCreate->createPVUnion(unionPtr);
    unions[1] = pvDataCreate->createPVUnion(unionPtr);
    unions[0]->select("doubleValue");
    unions[1]->select("intValue");
    PVDoublePtr pvDouble = unions[0]->get<PVDouble>();
    pvDouble->put(1.235);
    PVIntPtr pvInt = unions[1]->get<PVInt>();
    pvInt->put(5);
    pvValue->replace(freeze(unions));
    shared_vector<int32> severities(nchan);
    severities[0] = 0;
    severities[1] = 1;
    severities[2] = 2;
    PVIntArrayPtr pvSeverity = multiChannel->getSeverity();
    pvSeverity->replace(freeze(severities));
    if(debug) {cout << *pvStructure << endl;}
}


MAIN(testCreateRequest)
{
    testPlan(10);
    test();
    return testDone();
}

