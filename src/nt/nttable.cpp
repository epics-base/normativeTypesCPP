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

namespace detail {

static NTFieldPtr ntField = NTField::get();

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
    PVStringArray::svector l;
    l.resize(labels.size());
    std::copy(labels.begin(), labels.end(), l.begin());

    PVStructurePtr s = getPVDataCreate()->createPVStructure(createStructure());
    s->getSubField<PVStringArray>("labels")->replace(freeze(l));

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

}

const std::string NTTable::URI("uri:ev4:nt/2012/pwd:NTTable");

bool NTTable::is_a(StructureConstPtr const & structure)
{
    return structure->getID() == URI;
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
