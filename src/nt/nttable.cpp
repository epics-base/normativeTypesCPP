/* nttable.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/nttable.h>

namespace epics { namespace pvData { 

bool NTTable::isNTTable(PVStructurePtr pvStructure)
{
    NTField *ntfield = NTField::get();
    //StandardField *standardField = getStandardField();
    String name = pvStructure->getField()->getFieldName();
    if(name.compare("NTTable")!=0) return false;
    PVStringArray *pvLabel = static_cast<PVStringArray *>
        (pvStructure->getScalarArrayField("label",pvString));
    if(pvLabel==0) return false;
    int nfields = pvLabel->getLength();
    int nextra = 1; // label is 1 field
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField!=0 && pvStructure->getStringField("function")) nextra++;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField!=0 && ntfield->isTimeStamp(pvField->getField())) {
        nextra++;
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField!=0 && ntfield->isAlarm(pvField->getField())) {
        nextra++;
    }
    if(nfields!=(pvStructure->getStructure()->getNumberFields()-nextra)) return false;
    FieldConstPtrArray fields = pvStructure->getStructure()->getFields();
    int n = nfields - nextra;
    for(int i=0; i<n; i++) {
        FieldConstPtr field = fields[i+nextra];
        Type type = field->getType();
        if(type!=scalarArray && type!=structureArray) return false;
    }
    return true;
}

PVStructure::shared_pointer NTTable::create(
    bool hasFunction,bool hasTimeStamp, bool hasAlarm,
    int numberValues,
    FieldConstPtrArray valueFields)
{
    StandardField *standardField = getStandardField();
    int nfields = 1;
    if(hasFunction) nfields++;
    if(hasTimeStamp) nfields++;
    if(hasAlarm) nfields++;
    nfields += numberValues;
    FieldCreate *fieldCreate = getFieldCreate();
    PVDataCreate *pvDataCreate = getPVDataCreate();
    FieldConstPtrArray fields = new FieldConstPtr[nfields];
    int ind = 0;
    if(hasFunction) {
        fields[ind++] = fieldCreate->createScalar(String("function"),pvString);
    }
    if(hasTimeStamp) {
        fields[ind++] = standardField->timeStamp();
    }
    if(hasAlarm) {
        fields[ind++] = standardField->alarm();
    }
    fields[ind++] = standardField->scalarArray("label",pvString);
    for(int i=0; i<numberValues ; i++) fields[ind++] = valueFields[i];
    PVStructure::shared_pointer pvStructure = PVStructure::shared_pointer(
        pvDataCreate->createPVStructure(0,"NTTable",nfields,fields));
    String label[numberValues];
    for(int i=0; i<numberValues; i++) {
        FieldConstPtr field = fields[nfields - numberValues +i];
        label[i] = field->getFieldName();
    }
    PVStringArray *pvLabel = static_cast<PVStringArray *>
        (pvStructure->getScalarArrayField("label",pvString));
    pvLabel->put(0,numberValues,label,0);
    return pvStructure;
}

NTTable::NTTable(PVStructure::shared_pointer const & pvStructure)
: pvNTTable(pvStructure),
  pvFunction(0),
  pvTimeStamp(0),
  pvAlarm(0),
  pvLabel(0),
  offsetFields(1)
{
    NTField *ntfield = NTField::get();
    String name = pvStructure->getField()->getFieldName();
    if(name.compare("NTTable")!=0) {
        throw std::invalid_argument(
            "pvArgument does not have name NTTable");
    }
    PVScalarArray * pvScalarArray
         = pvStructure->getScalarArrayField("label",pvString);
    if(pvScalarArray==0) {
        throw std::invalid_argument(
            "pvArgument did not have a structureArray field value");
    }
    pvLabel = static_cast<PVStringArray *>(pvScalarArray);
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField!=0) {
        pvFunction = pvStructure->getStringField("function");
        offsetFields++;
    }
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField!=0 && ntfield->isTimeStamp(pvField->getField())) {
        pvTimeStamp = static_cast<PVStructure *>(pvField);
        offsetFields++;
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField!=0 && ntfield->isAlarm(pvField->getField())) {
        pvAlarm = static_cast<PVStructure *>(pvField);
        offsetFields++;
    }
}

NTTable::~NTTable()
{
}

PVString * NTTable::getFunction()
{
    return pvFunction;
}

void  NTTable::attachTimeStamp(PVTimeStamp &pvTimeStamp)
{
    if(this->pvTimeStamp==0) return;
    pvTimeStamp.attach(this->pvTimeStamp);
}

void  NTTable::attachAlarm(PVAlarm &pvAlarm)
{
    if(this->pvAlarm==0) return;
    pvAlarm.attach(this->pvAlarm);
}

PVStringArray * NTTable::getLabel()
{
    return pvLabel;
}

int NTTable::getNumberValues()
{
    return pvLabel->getLength();
}

FieldConstPtr NTTable::getField(int index)
{
    return pvNTTable->getStructure()->getFields()[index+offsetFields];
}

PVFieldPtr NTTable::getPVField(int index)
{
    return pvNTTable->getPVFields()[index+offsetFields];
}

}}
