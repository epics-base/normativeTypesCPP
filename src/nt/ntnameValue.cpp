/* ntnameValue.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/ntnameValue.h>

namespace epics { namespace pvData { 

using std::tr1::static_pointer_cast;

bool NTNameValue::isNTNameValue(PVStructurePtr pvStructure)
{
    String name = pvStructure->getField()->getFieldName();
    if(name.compare("NTNameValue")!=0) return false;
    PVFieldPtr pvField = pvStructure->getSubField("names");
    if(pvField==0) return false;
    FieldConstPtr field = pvField->getField();
    if(field->getType()!=scalarArray) return false;
    ScalarArrayConstPtr pscalarArray = static_pointer_cast<const ScalarArray>(field);
    if(pscalarArray->getElementType()!=pvString) return false;
    pvField = pvStructure->getSubField("values");
    if(pvField==0) return false;
    field = pvField->getField();
    if(field->getType()!=scalarArray) return false;
    pscalarArray = static_pointer_cast<const ScalarArray>(field);
    if(pscalarArray->getElementType()!=pvString) return false;
    return true;
}

PVStructure::shared_pointer NTNameValue::create(
    bool hasFunction,bool hasTimeStamp, bool hasAlarm)
{
    StandardField *standardField = getStandardField();
    int nfields = 2;
    if(hasFunction) nfields++;
    if(hasTimeStamp) nfields++;
    if(hasAlarm) nfields++;
    FieldCreate *fieldCreate = getFieldCreate();
    PVDataCreate *pvDataCreate = getPVDataCreate();
    FieldConstPtrArray fields = new FieldConstPtr[nfields];
    fields[0] = fieldCreate->createScalarArray("names",pvString);
    fields[1] = fieldCreate->createScalarArray("values",pvString);
    int ind = 2;
    if(hasFunction) {
        fields[ind++] = fieldCreate->createScalar(String("function"),pvString);
    }
    if(hasTimeStamp) {
        fields[ind++] = standardField->timeStamp();
    }
    if(hasAlarm) {
        fields[ind++] = standardField->alarm();
    }
    return PVStructure::shared_pointer(
        pvDataCreate->createPVStructure(0,"NTNameValue",ind,fields));
}

NTNameValue::NTNameValue(PVStructure::shared_pointer const & pvStructure)
: pvNTNameValue(pvStructure),
  pvFunction(0),
  pvTimeStamp(0),
  pvAlarm(0),
  pvNames(0),
  pvValues(0)
{
    NTField *ntfield = NTField::get();
    String name = pvStructure->getField()->getFieldName();
    if(name.compare("NTNameValue")!=0) {
        throw std::invalid_argument(
            "pvArgument does not have name NTNameValue");
    }
    PVArray * pvArray = pvStructure->getScalarArrayField("names",pvString);
    if(pvArray==0) {
        throw std::invalid_argument(
            "pvArgument does not have a string array field names");
    }
    pvNames = static_cast<PVStringArray *>(pvArray);
    pvArray = pvStructure->getScalarArrayField("values",pvString);
    if(pvArray==0) {
        throw std::invalid_argument(
            "pvArgument does not have a string array field values");
    }
    pvValues = static_cast<PVStringArray *>(pvArray);
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField!=0) {
        pvFunction = pvStructure->getStringField("function");
    }
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField!=0 && ntfield->isTimeStamp(pvField->getField())) {
        pvTimeStamp = static_cast<PVStructure *>(pvField);
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField!=0 && ntfield->isAlarm(pvField->getField())) {
        pvAlarm = static_cast<PVStructure *>(pvField);
    }
}

NTNameValue::~NTNameValue()
{
}

PVString * NTNameValue::getFunction()
{
    return pvFunction;
}

void  NTNameValue::attachTimeStamp(PVTimeStamp &pvTimeStamp)
{
    if(this->pvTimeStamp==0) return;
    pvTimeStamp.attach(this->pvTimeStamp);
}

void  NTNameValue::attachAlarm(PVAlarm &pvAlarm)
{
    if(this->pvAlarm==0) return;
    pvAlarm.attach(this->pvAlarm);
}

PVStringArray * NTNameValue::getNames() {
    return pvNames;
}

PVStringArray * NTNameValue::getValues() {
    return pvValues;
}

}}
