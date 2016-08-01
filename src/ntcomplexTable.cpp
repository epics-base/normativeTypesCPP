/* ntcomplexTable.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * This software is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>

#define epicsExportSharedSymbols
#include <pv/ntcomplexTable.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();
static FieldCreatePtr fieldCreate = getFieldCreate();
static UnionConstPtr variantUnion = getFieldCreate()->createVariantUnion();

namespace detail {

NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::addColumn(
        std::string const & name, epics::pvData::UnionConstPtr elementType
        )
{
    if (std::find(columnNames.begin(), columnNames.end(), name) != columnNames.end())
        throw std::runtime_error("duplicate column name");

    columnNames.push_back(name);
    types.push_back(elementType);

    return shared_from_this();
}


NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::addColumn(
        std::string const & name)
{
    return add(name, variantUnion);
}


StructureConstPtr NTComplexTableBuilder::createStructure()
{
    FieldBuilderPtr builder = getFieldCreate()->createFieldBuilder();

    FieldBuilderPtr nestedBuilder =
            builder->
               setId(NTComplexTable::URI)->
               addArray("labels", pvString)->
               addNestedStructure("value");

    vector<string>::size_type len = columnNames.size();
    for (vector<string>::size_type i = 0; i < len; i++)
        nestedBuilder->addArray(columnNames[i], types[i]);

    builder = nestedBuilder->endNested();

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);

    StructureConstPtr s = builder->createStructure();

    reset();
    return s;
}

NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTComplexTableBuilder::createPVStructure()
{
    // fill in labels with default values (the column names)
    size_t len = columnNames.size();
    shared_vector<string> l(len);
    for(size_t i=0; i<len; ++i) l[i] = columnNames[i];
    PVStructurePtr s = getPVDataCreate()->createPVStructure(createStructure());
    s->getSubField<PVStringArray>("labels")->replace(freeze(l));
    return s;
}

NTComplexTablePtr NTComplexTableBuilder::create()
{
    return NTComplexTablePtr(new NTComplexTable(createPVStructure()));
}

NTComplexTableBuilder::NTComplexTableBuilder()
{
    reset();
}

void NTComplexTableBuilder::reset()
{
    columnNames.clear();
    types.clear();
    descriptor = false;
    alarm = false;
    timeStamp = false;
}

NTComplexTableBuilder::shared_pointer NTComplexTableBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTComplexTable::URI("epics:nt/NTComplexTable:1.0");

NTComplexTable::shared_pointer NTComplexTable::wrap(PVStructurePtr const & pvStructure)
{
    if(!isCompatible(pvStructure)) return shared_pointer();
    return wrapUnsafe(pvStructure);
}

NTComplexTable::shared_pointer NTComplexTable::wrapUnsafe(PVStructurePtr const & pvStructure)
{
    return shared_pointer(new NTComplexTable(pvStructure));
}

bool NTComplexTable::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTComplexTable::is_a(PVStructurePtr const & pvStructure)
{
    return is_a(pvStructure->getStructure());
}



bool NTComplexTable::isCompatible(StructureConstPtr const & structure)
{
    if (!structure.get()) return false;

    StructureConstPtr valueField = structure->getField<Structure>("value");
    if (!valueField.get())
        return false;

    FieldConstPtrArray const & fields = valueField->getFields();
    for (FieldConstPtrArray::const_iterator it = fields.begin();
         it != fields.end(); ++it)
    {
        if ((*it)->getType() != unionArray) return false;
    }

    ScalarArrayConstPtr labelsField = structure->getField<ScalarArray>("labels");
    if (!labelsField.get() || labelsField->getElementType() != pvString)
        return false;

    FieldConstPtr field = structure->getField("descriptor");
    if (field.get())
    {
        ScalarConstPtr descriptorField = structure->getField<Scalar>("descriptor");
        if (!descriptorField.get() || descriptorField->getScalarType() != pvString)
            return false;
    }

    NTFieldPtr ntField = NTField::get();

    field = structure->getField("alarm");
    if (field.get() && !ntField->isAlarm(field))
        return false;

    field = structure->getField("timeStamp");
    if (field.get() && !ntField->isTimeStamp(field))
        return false;

    return true;
}

bool NTComplexTable::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure) return false;

    return isCompatible(pvStructure->getStructure());
}

bool NTComplexTable::isValid()
{
    PVFieldPtrArray const & columns = pvValue->getPVFields();
        
    if (getLabels()->getLength() != columns.size()) return false;
    bool first = true;
    int length = 0;
    for (PVFieldPtrArray::const_iterator it = columns.begin();
        it != columns.end();++it)
    {
        PVUnionArrayPtr column = std::tr1::dynamic_pointer_cast<PVUnionArray>(*it);
        if (!column.get()) return false;
        int colLength = column->getLength();
        if (first)
        {
            length = colLength;
            first = false;
        }
        else if (length != colLength)
            return false;
    }

    return true;
}


NTComplexTableBuilderPtr NTComplexTable::createBuilder()
{
    return NTComplexTableBuilderPtr(new detail::NTComplexTableBuilder());
}

bool NTComplexTable::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTComplexTable::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTComplexTable::getPVStructure() const
{
    return pvNTComplexTable;
}

PVStringPtr NTComplexTable::getDescriptor() const
{
    return pvNTComplexTable->getSubField<PVString>("descriptor");
}

PVStructurePtr NTComplexTable::getTimeStamp() const
{
    return pvNTComplexTable->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTComplexTable::getAlarm() const
{
    return pvNTComplexTable->getSubField<PVStructure>("alarm");
}

PVStringArrayPtr NTComplexTable::getLabels() const
{
    return pvNTComplexTable->getSubField<PVStringArray>("labels");
}

StringArray const & NTComplexTable::getColumnNames() const
{
    return pvNTComplexTable->getStructure()->getFieldNames();
}

PVUnionArrayPtr NTComplexTable::getColumn(std::string const & columnName) const
{
    return pvValue->getSubField<PVUnionArray>(columnName);
}

NTComplexTable::NTComplexTable(PVStructurePtr const & pvStructure) :
    pvNTComplexTable(pvStructure), pvValue(pvNTComplexTable->getSubField<PVStructure>("value"))
{}


}}
