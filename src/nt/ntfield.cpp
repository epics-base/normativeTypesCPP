/* ntArgument.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/lock.h>
#include <pv/ntfield.h>

namespace epics { namespace pvData { 



NTField *NTField::get()
{
    static Mutex mutex;
    static NTField *ntstructureField = 0;
    Lock xx(mutex);
    if(ntstructureField==0) ntstructureField = new NTField();
    return ntstructureField;
}

NTField::NTField()
: fieldCreate(getFieldCreate()),
  standardField(getStandardField())
{
}

bool NTField::isEnumerated(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=2) return false;
   FieldConstPtr f = fields[0];
   if(f->getFieldName().compare("index")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar* s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvInt) return false;
   f = fields[1];
   if(f->getFieldName().compare("choices")!=0) return false;
   if(f->getType()!=scalarArray) return false;
   const ScalarArray* sa = static_cast<const ScalarArray*>(f.get());
   if(sa->getElementType()!=pvString) return false;
   return true;
}

bool NTField::isTimeStamp(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   if(field->getFieldName().compare("timeStamp")!=0) return false;
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=3) return false;
   FieldConstPtr f = fields[0];
   if(f->getFieldName().compare("secondsPastEpoch")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar* s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvLong) return false;
   f = fields[1];
   if(f->getFieldName().compare("nanoSeconds")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvInt) return false;
   f = fields[2];
   if(f->getFieldName().compare("userTag")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvInt) return false;
   return true;
}

bool NTField::isAlarm(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   if(field->getFieldName().compare("alarm")!=0) return false;
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=3) return false;
   FieldConstPtr f = fields[0];
   if(f->getFieldName().compare("severity")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar* s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvInt) return false;
   f = fields[1];
   if(f->getFieldName().compare("status")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvInt) return false;
   f = fields[2];
   if(f->getFieldName().compare("message")!=0) return false;
   if(f->getType()!=scalar) return false;
   s = static_cast<const Scalar*>(f.get());
   if(s->getScalarType()!=pvString) return false;
   return true;
}

bool NTField::isDisplay(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   if(field->getFieldName().compare("display")!=0) return false;
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=4) return false;
   // look at limit first
   FieldConstPtr f = fields[3];
   if(f->getFieldName().compare("limit")!=0) return false;
   if(f->getType()!=structure) return false;
   const Structure* s = static_cast<const Structure*>(f.get());
   FieldConstPtrArray subfields = s->getFields();
   n = s->getNumberFields();
   if(n!=2) return false;
   f = subfields[0];
   if(f->getFieldName().compare("low")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar* sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = subfields[1];
   if(f->getFieldName().compare("high")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[0];
   if(f->getFieldName().compare("description")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvString) return false;
   f = fields[1];
   if(f->getFieldName().compare("format")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvString) return false;
   f = fields[2];
   if(f->getFieldName().compare("units")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvString) return false;
   return true;
}

bool NTField::isAlarmLimit(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   if(field->getFieldName().compare("alarmLimit")!=0) return false;
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=4) return false;
   FieldConstPtr f = fields[0];
   if(f->getFieldName().compare("highAlarm")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar *sc = static_cast<const Scalar *>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[1];
   if(f->getFieldName().compare("highWarning")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar *>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[2];
   if(f->getFieldName().compare("lowWarning")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar *>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[3];
   if(f->getFieldName().compare("lowAlarm")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar *>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   return true;
}

bool NTField::isControl(FieldConstPtr field)
{
   if(field->getType()!=structure) return false;
   const Structure *st = static_cast<const Structure *>(field.get());
   if(field->getFieldName().compare("control")!=0) return false;
   FieldConstPtrArray fields = st->getFields();
   int n = st->getNumberFields();
   if(n!=2) return false;
   FieldConstPtr f = fields[1];
   if(f->getFieldName().compare("minStep")!=0) return false;
   if(f->getType()!=scalar) return false;
   const Scalar* sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[0];
   if(f->getFieldName().compare("limit")!=0) return false;
   if(f->getType()!=structure) return false;
   const Structure* s = static_cast<const Structure*>(f.get());
   fields = s->getFields();
   n = s->getNumberFields();
   if(n!=2) return false;
   f = fields[0];
   if(f->getFieldName().compare("low")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   f = fields[1];
   if(f->getFieldName().compare("high")!=0) return false;
   if(f->getType()!=scalar) return false;
   sc = static_cast<const Scalar*>(f.get());
   if(sc->getScalarType()!=pvDouble) return false;
   return true;
}

StructureConstPtr NTField::createEnumerated(String fieldName)
{
    return standardField->enumerated(fieldName);
}

StructureConstPtr NTField::createTimeStamp()
{
    return standardField->timeStamp();
}

StructureConstPtr NTField::createAlarm()
{
    return standardField->alarm();
}

StructureConstPtr NTField::createDisplay()
{
    return standardField->display();
}

StructureConstPtr NTField::createAlarmLimit()
{
    int numFields = 4;
    FieldConstPtrArray fields = new FieldConstPtr[numFields];
    fields[0] = fieldCreate->createScalar(String("highAlarm"),pvDouble);
    fields[1] = fieldCreate->createScalar(String("highWarning"),pvDouble);
    fields[2] = fieldCreate->createScalar(String("lowWarning"),pvDouble);
    fields[3] = fieldCreate->createScalar(String("lowAlarm"),pvDouble);
    return fieldCreate->createStructure(String("alarmLimit"),numFields,fields);
}

StructureConstPtr NTField::createControl()
{
    return standardField->control();
}

StructureArrayConstPtr NTField::createEnumeratedArray(String fieldName)
{
    StructureConstPtr st = createEnumerated(fieldName);
    return fieldCreate->createStructureArray(fieldName,st);
}

StructureArrayConstPtr NTField::createTimeStampArray(String fieldName)
{
    StructureConstPtr st = createTimeStamp();
    return fieldCreate->createStructureArray(fieldName,st);
}

StructureArrayConstPtr NTField::createAlarmArray(String fieldName)
{
    StructureConstPtr st = createAlarm();
    return fieldCreate->createStructureArray(fieldName,st);
}

PVNTField *PVNTField::get()
{
    static Mutex mutex;
    static PVNTField *pvntstructureField = 0;
    Lock xx(mutex);
    if(pvntstructureField==0) pvntstructureField = new PVNTField();
    return pvntstructureField;
}

PVNTField::PVNTField()
: pvDataCreate(getPVDataCreate()),
  standardPVField(getStandardPVField()),
  ntstructureField(NTField::get())
{
}


PVStructurePtr PVNTField::createEnumerated(
    PVStructurePtr parent,
    String fieldName,
    StringArray choices,
    int numberChoices)
{
    return standardPVField->enumerated(parent,fieldName,choices,numberChoices);
}

PVStructurePtr PVNTField::createTimeStamp(PVStructurePtr parent)
{
    return standardPVField->timeStamp(parent);
}

PVStructurePtr PVNTField::createAlarm(PVStructurePtr parent)
{
    return standardPVField->alarm(parent);
}

PVStructurePtr PVNTField::createDisplay(PVStructurePtr parent)
{
    return standardPVField->display(parent);
}

PVStructurePtr PVNTField::createAlarmLimit(PVStructurePtr parent)
{
    StructureConstPtr structure = NTField::get()->createAlarmLimit();
    return pvDataCreate->createPVStructure(parent,structure);
}


PVStructurePtr PVNTField::createControl(PVStructurePtr parent)
{
    return standardPVField->control(parent);
}

PVStructureArray *PVNTField::createEnumeratedArray(
    PVStructurePtr parent,String fieldName)
{
    StructureArrayConstPtr sa = ntstructureField->createEnumeratedArray(fieldName);
    return pvDataCreate->createPVStructureArray(parent,sa);
}

PVStructureArray *PVNTField::createTimeStampArray(
    PVStructurePtr parent,String fieldName)
{
    StructureArrayConstPtr sa = ntstructureField->createTimeStampArray(fieldName);
    return pvDataCreate->createPVStructureArray(parent,sa);
}

PVStructureArray *PVNTField::createAlarmArray(
    PVStructurePtr parent,String fieldName)
{
    StructureArrayConstPtr sa = ntstructureField->createAlarmArray(fieldName);
    return pvDataCreate->createPVStructureArray(parent,sa);
}

}}

