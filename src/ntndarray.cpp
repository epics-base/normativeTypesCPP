/* ntndarray.cpp */
/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

#include <algorithm>

#include <epicsThread.h>

#include <pv/lock.h>
#include <pv/sharedPtr.h>

#include "validator.h"

#define epicsExportSharedSymbols
#include <pv/ntndarray.h>
#include <pv/ntndarrayAttribute.h>
#include <pv/ntutils.h>

using namespace std;
using namespace epics::pvData;

namespace epics { namespace nt {

namespace detail {

static StructureConstPtr *base_s;
static epicsThreadOnceId base_once = EPICS_THREAD_ONCE_INIT;

void NTNDArrayBuilder::once(void *)
{
    FieldBuilderPtr fb(FieldBuilder::begin());
    StandardFieldPtr standardField = getStandardField();

    for (int i = pvBoolean; i < pvString; ++i) {
        ScalarType st = static_cast<ScalarType>(i);
        fb->addArray(std::string(ScalarTypeFunc::name(st)) + "Value", st);
    }

    UnionConstPtr value(fb->createUnion());

    StructureConstPtr codec(fb->setId("codec_t")->
        add("name", pvString)->
        add("parameters", getFieldCreate()->createVariantUnion())->
        createStructure());

    StructureConstPtr dimension(fb->setId("dimension_t")->
        add("size", pvInt)->
        add("offset",  pvInt)->
        add("fullSize",  pvInt)->
        add("binning",  pvInt)->
        add("reverse",  pvBoolean)->
        createStructure());

    StructureConstPtr attribute(NTNDArrayAttribute::createBuilder()->createStructure());

    base_s = new StructureConstPtr;
    *base_s = fb->setId(NTNDArray::URI)->
        add("value", value)->
        add("codec", codec)->
        add("compressedSize", pvLong)->
        add("uncompressedSize", pvLong)->
        addArray("dimension", dimension)->
        add("uniqueId", pvInt)->
        add("dataTimeStamp", standardField->timeStamp())->
        addArray("attribute", attribute)->
        createStructure();
}

StructureConstPtr NTNDArrayBuilder::createStructure()
{
    epicsThreadOnce(&base_once, &NTNDArrayBuilder::once, 0);

    StandardFieldPtr standardField(getStandardField());
    FieldBuilderPtr fb(FieldBuilder::begin(*base_s));

    if (descriptor)
        fb->add("descriptor", pvString);

    if (alarm)
        fb->add("alarm", standardField->alarm());

    if (timeStamp)
        fb->add("timeStamp", standardField->timeStamp());

    if (display)
        fb->add("display", standardField->display());

    size_t extraCount = extraFieldNames.size();
    for (size_t i = 0; i< extraCount; i++)
        fb->add(extraFieldNames[i], extraFields[i]);

    return fb->createStructure();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addDescriptor()
{
    descriptor = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addAlarm()
{
    alarm = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addTimeStamp()
{
    timeStamp = true;
    return shared_from_this();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::addDisplay()
{
    display = true;
    return shared_from_this();
}

PVStructurePtr NTNDArrayBuilder::createPVStructure()
{
    return getPVDataCreate()->createPVStructure(createStructure());
}

NTNDArrayPtr NTNDArrayBuilder::create()
{
    return NTNDArrayPtr(new NTNDArray(createPVStructure()));
}

NTNDArrayBuilder::NTNDArrayBuilder()
{
    reset();
}

void NTNDArrayBuilder::reset()
{
    descriptor = false;
    timeStamp = false;
    alarm = false;
    display = false;
    extraFieldNames.clear();
    extraFields.clear();
}

NTNDArrayBuilder::shared_pointer NTNDArrayBuilder::add(string const & name, FieldConstPtr const & field)
{
    extraFields.push_back(field); extraFieldNames.push_back(name);
    return shared_from_this();
}

}

const std::string NTNDArray::URI("epics:nt/NTNDArray:1.0");
const std::string ntAttrStr("epics:nt/NTAttribute:1.0");

NTNDArray::shared_pointer NTNDArray::wrap(PVStructurePtr const & pvStructure)
{
    if(!isCompatible(pvStructure)) return shared_pointer();
    return wrapUnsafe(pvStructure);
}

NTNDArray::shared_pointer NTNDArray::wrapUnsafe(PVStructurePtr const & pvStructure)
{
    return shared_pointer(new NTNDArray(pvStructure));
}

bool NTNDArray::is_a(StructureConstPtr const & structure)
{
    return NTUtils::is_a(structure->getID(), URI);
}

bool NTNDArray::is_a(PVStructurePtr const & pvStructure)
{
    return is_a(pvStructure->getStructure());
}

static Validator::Definition* definition;
static epicsThreadOnceId definition_once = EPICS_THREAD_ONCE_INIT;

static void definition_init(void *)
{
    StructureConstPtr structure(
        NTNDArray::createBuilder()->
            addDescriptor()->
            addAlarm()->
            addDisplay()->
            addTimeStamp()->
            createStructure());

    definition = new Validator::Definition;
    definition->structure = std::static_pointer_cast<const Field>(structure);

    // TODO: these should be all getFieldT
    definition->optional.insert(structure->getField("descriptor").get());
    definition->optional.insert(structure->getField("alarm").get());
    definition->optional.insert(structure->getField("timeStamp").get());
    definition->optional.insert(structure->getField("display").get());

    // TODO: the following should be a helper in NTNDArrayAttribute
    StructureConstPtr attribute(
        std::static_pointer_cast<const StructureArray>(
            structure->getField("attribute")
        )->getStructure()
    );

    definition->optional.insert(attribute->getField("tags").get());
    definition->optional.insert(attribute->getField("alarm").get());
    definition->optional.insert(attribute->getField("timeStamp").get());
}

bool NTNDArray::isCompatible(StructureConstPtr const &structure)
{
    if(!structure.get()) return false;

    epicsThreadOnce(&definition_once, &definition_init, 0);

    return Validator::isCompatible(*definition, *structure);
}


bool NTNDArray::isCompatible(PVStructurePtr const & pvStructure)
{
    if(!pvStructure.get()) return false;

    return isCompatible(pvStructure->getStructure());
}

bool NTNDArray::isValid()
{
    int64 valueSize = getValueSize();
    int64 compressedSize = getCompressedDataSize()->get();
    if (valueSize != compressedSize)
        return false;

    long expectedUncompressed = getExpectedUncompressedSize();
    long uncompressedSize = getUncompressedDataSize()->get();
    if (uncompressedSize != expectedUncompressed)
        return false;

    std::string codecName = getCodec()->getSubField<PVString>("name")->get();
    if (codecName == "" && valueSize < uncompressedSize)
        return false;

    return true;
}

int64 NTNDArray::getExpectedUncompressedSize()
{
    int64 size = 0;
    PVStructureArrayPtr pvDim = getDimension();

    if (pvDim->getLength() != 0)
    {
        PVStructureArray::const_svector data = pvDim->view();
        size = getValueTypeSize();
        for (PVStructureArray::const_svector::const_iterator it = data.begin();
        it != data.end(); ++it )
        {
            PVStructurePtr dim = *it;
            size *= dim->getSubField<PVInt>("size")->get();
        }
    }

    return size;
}

int64 NTNDArray::getValueSize()
{
    int64 size = 0;
    PVScalarArrayPtr storedValue = getValue()->get<PVScalarArray>();
    if (!storedValue.get())
    {
        size = storedValue->getLength()*getValueTypeSize();
    }
    return size;
}

int64 NTNDArray::getValueTypeSize()
{
    int64 typeSize = 0;
    PVScalarArrayPtr storedValue = getValue()->get<PVScalarArray>();
    if (storedValue.get())
    {
        switch (storedValue->getScalarArray()->getElementType())
        {
        case pvBoolean:
        case pvByte:
        case pvUByte:
            typeSize = 1;
            break;

        case pvShort:
        case pvUShort:
            typeSize = 2;
            break;

        case pvInt:
        case pvUInt:
        case pvFloat:
            typeSize = 4;
            break;

        case pvLong:
        case pvULong:
        case pvDouble:
            typeSize = 8;
            break;

        default:
            break;
        }
    }
    return typeSize;
}

NTNDArrayBuilderPtr NTNDArray::createBuilder()
{
    return NTNDArrayBuilderPtr(new detail::NTNDArrayBuilder());
}


bool NTNDArray::attachTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNDArray::attachDataTimeStamp(PVTimeStamp &pvTimeStamp) const
{
    PVStructurePtr ts = getDataTimeStamp();
    if (ts)
        return pvTimeStamp.attach(ts);
    else
        return false;
}

bool NTNDArray::attachAlarm(PVAlarm &pvAlarm) const
{
    PVStructurePtr al = getAlarm();
    if (al)
        return pvAlarm.attach(al);
    else
        return false;
}

bool NTNDArray::attachDisplay(PVDisplay &pvDisplay) const
{
    PVStructurePtr dp = getDisplay();
    if (dp)
        return pvDisplay.attach(dp);
    else
        return false;
}

PVStructurePtr NTNDArray::getPVStructure() const
{
    return pvNTNDArray;
}

PVUnionPtr NTNDArray::getValue() const
{
    return pvNTNDArray->getSubField<PVUnion>("value");
}

PVStructurePtr NTNDArray::getCodec() const
{
    return pvNTNDArray->getSubField<PVStructure>("codec");
}

PVLongPtr NTNDArray::getCompressedDataSize() const
{
    return pvNTNDArray->getSubField<PVLong>("compressedSize");
}

PVLongPtr NTNDArray::getUncompressedDataSize() const
{
    return pvNTNDArray->getSubField<PVLong>("uncompressedSize");
}

PVStructureArrayPtr NTNDArray::getDimension() const
{
    return pvNTNDArray->getSubField<PVStructureArray>("dimension");
}

PVIntPtr NTNDArray::getUniqueId() const
{
    return pvNTNDArray->getSubField<PVInt>("uniqueId");
}

PVStructurePtr NTNDArray::getDataTimeStamp() const
{
    return pvNTNDArray->getSubField<PVStructure>("dataTimeStamp");
}

PVStructureArrayPtr NTNDArray::getAttribute() const
{
    return pvNTNDArray->getSubField<PVStructureArray>("attribute");
}

PVStringPtr NTNDArray::getDescriptor() const
{
    return pvNTNDArray->getSubField<PVString>("descriptor");
}

PVStructurePtr NTNDArray::getTimeStamp() const
{
    return pvNTNDArray->getSubField<PVStructure>("timeStamp");
}

PVStructurePtr NTNDArray::getAlarm() const
{
    return pvNTNDArray->getSubField<PVStructure>("alarm");
}

PVStructurePtr NTNDArray::getDisplay() const
{
    return pvNTNDArray->getSubField<PVStructure>("display");
}


NTNDArray::NTNDArray(PVStructurePtr const & pvStructure) :
    pvNTNDArray(pvStructure)
{}


}}
