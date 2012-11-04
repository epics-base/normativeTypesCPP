/* nttable.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/nttable.h>

namespace epics { namespace pvData { 

using std::tr1::static_pointer_cast;

bool NTTable::isNTTable(PVStructurePtr const & pvStructure)
{
    NTFieldPtr ntfield = NTField::get();
    PVStringArrayPtr pvLabel = static_pointer_cast<PVStringArray>
        (pvStructure->getScalarArrayField("label",pvString));
    if(pvLabel.get()==NULL) return false;
    size_t nfields = pvLabel->getLength();
    size_t nextra = 1; // label is 1 field
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField.get()!=NULL
    && pvStructure->getStringField("function").get()!=NULL) nextra++;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField!=0 && ntfield->isTimeStamp(pvField->getField())) {
        nextra++;
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField.get()!=NULL && ntfield->isAlarm(pvField->getField())) {
        nextra++;
    }
    if(nfields!=(pvStructure->getStructure()->getNumberFields()-nextra)) {
        return false;
    }
    FieldConstPtrArray fields = pvStructure->getStructure()->getFields();
    for(size_t i=0; i<nfields; i++) {
        FieldConstPtr field = fields[i+nextra];
        Type type = field->getType();
        if(type!=scalarArray && type!=structureArray) return false;
    }
    return true;
}

NTTablePtr NTTable::create(
    bool hasFunction,bool hasTimeStamp, bool hasAlarm,
    StringArray const & valueNames,
    FieldConstPtrArray const &valueFields)
{
    StandardFieldPtr standardField = getStandardField();
    size_t nfields = 1;
    if(hasFunction) nfields++;
    if(hasTimeStamp) nfields++;
    if(hasAlarm) nfields++;
    nfields += valueFields.size();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    FieldConstPtrArray fields;
    StringArray names;
    fields.resize(nfields);
    names.resize(nfields);
    size_t ind = 0;
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
    names[ind] = "label";
    fields[ind++] = fieldCreate->createScalarArray(pvString);
    size_t numberValues = valueNames.size();
    for(size_t i=0; i<numberValues ; i++) {
        names[ind] = valueNames[i];
        fields[ind++] = valueFields[i];
    }
    StructureConstPtr st = fieldCreate->createStructure(names,fields);
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(st);
    PVStringArrayPtr pvLabel = static_pointer_cast<PVStringArray>
        (pvStructure->getScalarArrayField("label",pvString));
    pvLabel->put(0,numberValues,valueNames,0);
    return NTTablePtr(new NTTable(pvStructure));
}

NTTablePtr NTTable::clone(PVStructurePtr const & pv)
{
    PVStructurePtr pvStructure = getPVDataCreate()->createPVStructure(pv);
    return NTTablePtr(new NTTable(pvStructure));
}

NTTable::NTTable(PVStructurePtr const & pvStructure)
: pvNTTable(pvStructure),
  offsetFields(1)
{
    NTFieldPtr ntfield = NTField::get();
    PVScalarArrayPtr pvScalarArray
         = pvStructure->getScalarArrayField("label",pvString);
    pvLabel = static_pointer_cast<PVStringArray>(pvScalarArray);
    PVFieldPtr pvField = pvStructure->getSubField("function");
    if(pvField.get()!=NULL) {
        offsetFields++;
        pvFunction = pvStructure->getStringField("function");
    }
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField.get()!=NULL && ntfield->isTimeStamp(pvField->getField())) {
        offsetFields++;
        pvTimeStamp = static_pointer_cast<PVStructure>(pvField);
    }
    pvField = pvStructure->getSubField("alarm");
    if(pvField.get()!=NULL && ntfield->isAlarm(pvField->getField())) {
        offsetFields++;
        pvAlarm = static_pointer_cast<PVStructure>(pvField);
    }
}


void  NTTable::attachTimeStamp(PVTimeStamp &pv)
{
    if(pvTimeStamp.get()==NULL) return;
    pv.attach(pvTimeStamp);
}

void  NTTable::attachAlarm(PVAlarm &pv)
{
    if(pvAlarm.get()==NULL) return;
    pv.attach(pvAlarm);
}

size_t NTTable::getNumberValues()
{
    return pvLabel->getLength();
}

FieldConstPtr NTTable::getField(size_t index)
{
    FieldConstPtrArray fields = pvNTTable->getStructure()->getFields();
    return fields[index + offsetFields];
}

PVFieldPtr NTTable::getPVField(size_t index)
{
    PVFieldPtrArray pvFields = pvNTTable->getPVFields();
    return pvFields[index+offsetFields];
}

}}
