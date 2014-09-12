/* nttable.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>

#include <pv/nttable.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

static NTFieldPtr ntField = NTField::get();

namespace detail {

NTTableBuilder::shared_pointer NTTableBuilder::add(
        std::string const & name, epics::pvData::ScalarType scalarType
        )
{
    if (std::find(labels.begin(), labels.end(), name) != labels.end())
        throw std::runtime_error("duplicate column name");

    labels.push_back(name);
    types.push_back(scalarType);

    return shared_from_this();
}

StructureConstPtr NTTableBuilder::createStructure()
{
    FieldBuilderPtr builder = getFieldCreate()->createFieldBuilder();

    FieldBuilderPtr nestedBuilder =
            builder->
               setId(NTTable::URI)->
               addArray("labels", pvString)->
               addNestedStructure("value");

    vector<string>::size_type len = labels.size();
    for (vector<string>::size_type i = 0; i < len; i++)
        nestedBuilder->addArray(labels[i], types[i]);

    builder = nestedBuilder->endNested();

    if (descriptor)
        builder->add("descriptor", pvString);

    if (alarm)
        builder->add("alarm", ntField->createAlarm());

    if (timeStamp)
        builder->add("timeStamp", ntField->createTimeStamp());

    StructureConstPtr s = builder->createStructure();

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        builder->add(extraFieldNames[i], extraFields[i]);


    reset();
    return s;
}

NTTableBuilder::shared_pointer NTTableBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTTableBuilder::shared_pointer NTTableBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTTableBuilder::shared_pointer NTTableBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

PVStructurePtr NTTableBuilder::createPVStructure()
{
    PVStructurePtr s = getPVDataCreate()->createPVStructure(createStructure());
    StringArray const & fieldNames =
        s->getSubField<PVStructure>("value")->getStructure()->getFieldNames();
    size_t len = fieldNames.size();
    shared_vector<string> names(len);
    for(size_t i=0; i<len; ++i) names[i] = fieldNames[i];
    s->getSubField<PVStringArray>("labels")->replace(freeze(names));
    return s;
}

NTTablePtr NTTableBuilder::create()
{
    return NTTablePtr(new NTTable(createPVStructure()));
}

NTTableBuilder::NTTableBuilder()
{
    reset();
}

void NTTableBuilder::reset()
{
    labels.clear();
    types.clear();
    descriptor = false;
    alarm = false;
    timeStamp = false;
}

NTTableBuilder::shared_pointer NTTableBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}


}

const std::string NTTable::URI("uri:ev4:nt/2014/pwd:NTTable");

NTTable::shared_pointer NTTable::narrow(PVStructurePtr const & structure)
{
    if (!structure || !is_a(structure->getStructure()))
        return shared_pointer();

    return narrow_unsafe(structure);
}

NTTable::shared_pointer NTTable::narrow_unsafe(PVStructurePtr const & structure)
{
    return shared_pointer(new NTTable(structure));
}

bool NTTable::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
}

bool NTTable::is_compatible(PVStructurePtr const & pvStructure)
{
    PVFieldPtr pvField = pvStructure->getSubField("alarm");
    if(pvField && !ntField->isAlarm(pvField->getField())) return false;
    pvField = pvStructure->getSubField("timeStamp");
    if(pvField && !ntField->isTimeStamp(pvField->getField())) return false;
    PVStringArrayPtr pvLabel = pvStructure->getSubField<PVStringArray>("labels");
    const shared_vector<const string> column(pvLabel->view());
    size_t len = column.size();
    for(size_t i=0; i<len; ++i) {
        string value = "value." + column[i];
        if(!pvStructure->getSubField<PVScalarArray>(value)) return false;
    }
    return true;
}

NTTableBuilderPtr NTTable::createBuilder()
{
    return NTTableBuilderPtr(new detail::NTTableBuilder());
}

bool NTTable::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTTable::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

PVStructurePtr NTTable::getPVStructure() const
{
    return pvNTTable;
}

PVStringPtr NTTable::getDescriptor() const
{
    return pvNTTable->getSubField<PVString>("descriptor");
}

PVStructurePtr NTTable::getTimeStamp() const
{
    return pvNTTable->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTTable::getAlarm() const
{
    return pvNTTable->getSubField<PVStructure>("alarm");
}

PVStringArrayPtr NTTable::getLabels() const
{
    return pvNTTable->getSubField<PVStringArray>("labels");
}

PVFieldPtr NTTable::getColumn(std::string const & columnName) const
{
    return pvNTTable->getSubField("value." + columnName);
}

NTTable::NTTable(PVStructurePtr const & pvStructure) :
    pvNTTable(pvStructure)
{}


}}
