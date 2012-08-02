/* ntnameValue.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntnameValue.h>

namespace epics { namespace pvData { 

using std::tr1::static_pointer_cast;

bool NTNameValue::isNTNameValue(PVStructurePtr const & pvStructure)
{
    PVFieldPtr pvField = pvStructure->getSubField("names");
    if(pvField.get()==NULL) return false;
    FieldConstPtr field = pvField->getField();
    if(field->getType()!=scalarArray) return false;
    ScalarArrayConstPtr pscalarArray =
        static_pointer_cast<const ScalarArray>(field);
    if(pscalarArray->getElementType()!=pvString) return false;
    pvField = pvStructure->getSubField("values");
    if(pvField==0) return false;
    field = pvField->getField();
    if(field->getType()!=scalarArray) return false;
    pscalarArray = static_pointer_cast<const ScalarArray>(field);
    if(pscalarArray->getElementType()!=pvString) return false;
    return true;
}

NTNameValuePtr NTNameValue::create(
    bool hasFunction,bool hasTimeStamp, bool hasAlarm)
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 2;
    if(hasFunction) nfields++;
    if(hasTimeStamp) nfields++;
    if(hasAlarm) nfields++;
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    FieldConstPtrArray fields;
    StringArray names;
    fields.resize(nfields);
    names.resize(nfields);
    names[0] = "names";
    fields[0] = fieldCreate->createScalarArray(pvString);
    names[1] = "values";
    fields[1] = fieldCreate->createScalarArray(pvString);
    size_t ind = 2;
    if(hasFunction) {
        names[ind] = "function";
        fields[ind++] = fieldCreate->createScalar(pvString);
    }
    if(hasTimeStamp) {
        names[ind] = "timeStamp";
        fields[ind++] = standardField->timeStamp();
    }
    if(hasAlarm) {
        names[ind] = "alarm";
        fields[ind++] = standardField->alarm();
    }
    StructureConstPtr st = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(st);
    return NTNameValuePtr(new NTNameValue(pvStructure));
}

NTNameValue::NTNameValue(PVStructure::shared_pointer const & pvStructure)
: pvNTNameValue(pvStructure)
{
    NTFieldPtr ntfield = NTField::get();
    PVScalarArrayPtr pvArray =
        pvStructure->getScalarArrayField("names",pvString);
    pvNames = static_pointer_cast<PVStringArray>(pvArray);
    pvArray = pvStructure->getScalarArrayField("values",pvString);
    pvValues = static_pointer_cast<PVStringArray>(pvArray);
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField.get()!=NULL) {
        pvFunction = pvStructure->getStringField("function");
    }
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField.get()!=NULL && ntfield->isTimeStamp(pvField->getField())) {
        pvTimeStamp = static_pointer_cast<PVStructure>(pvField);
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField.get()!=NULL && ntfield->isAlarm(pvField->getField())) {
        pvAlarm = static_pointer_cast<PVStructure>(pvField);
    }
}


void  NTNameValue::attachTimeStamp(PVTimeStamp &pv)
{
    if(pvTimeStamp.get()==NULL) return;
    pv.attach(pvTimeStamp);
}

void  NTNameValue::attachAlarm(PVAlarm &pv)
{
    if(pvAlarm.get()==NULL) return;
    pv.attach(pvAlarm);
}

}}
