/* ntndarray.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef NTNDARRAY_H
#define NTNDARRAY_H

#include <pv/ntfield.h>

#include <vector>
#include <string>

namespace epics { namespace nt {

class NTNDArray;
typedef std::tr1::shared_ptr<NTNDArray> NTNDArrayPtr;

/**
 * Convenience Class for NTNDArray
 * @author dgh
 */
class NTNDArray
{
public:
    POINTER_DEFINITIONS(NTNDArray);

    static const std::string URI;

    /**
     * Is the structure an NTNDArray.
     * @param structure The structure to test.
     * @return (false,true) if (is not, is) an NTNDArray.
     */
    static bool is_a(epics::pvData::StructureConstPtr const & structure);


    static NTNDArrayPtr create(
        epics::pvData::PVStructurePtr const &pvStructure);

    static NTNDArrayPtr create(bool hasDescriptor,
        bool hasTimeStamp, bool hasAlarm, bool hasDisplay);

    static NTNDArrayPtr create();

    static NTNDArrayPtr clone(epics::pvData::PVStructurePtr const &);

    /**
     * Create a {@code Structure} that represents NTNDArray.
     */
    static epics::pvData::StructureConstPtr createStructure(bool hasDescriptor,
        bool hasTimeStamp, bool hasAlarm, bool hasDisplay);

    /**
     * Destructor.
     */
    ~NTNDArray() {}

     /**
      * Attach a pvTimeStamp to timeStamp field.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

     /**
      * Attach a pvTimeStamp to dataTimeStamp field.
      * @param pvTimeStamp The pvTimeStamp that will be attached.
      * Does nothing if no timeStamp.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
      */
    bool attachDataTimeStamp(epics::pvData::PVTimeStamp &pvTimeStamp) const;

    /**
     * Attach an pvAlarm.
     * @param pvAlarm The pvAlarm that will be attached.
     * Does nothing if no alarm.
      * @return true if the operation was successfull (i.e. this instance has a timeStamp field), otherwise false.
     */
    bool attachAlarm(epics::pvData::PVAlarm &pvAlarm) const;

    /**
     * Get the pvStructure.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getPVStructure() const;

    /**
     * Get the value field.
     * @return The PVField for the values.
     */
    epics::pvData::PVUnionPtr getValue() const;

    /**
     * Get the compressedDataSize field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVLongPtr getCompressedDataSize() const;

    /**
     * Get the uncompressedDataSize field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVLongPtr getUncompressedDataSize() const;

    /**
     * Get the codec field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getCodec() const;

    /**
     * Get the attribute field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructureArrayPtr getAttribute() const;

    /**
     * Get the dimension field.
     * @return the PVStructurePtr.
     */
    epics::pvData::PVStructureArrayPtr getDimension() const;

    /**
     * Get the uniqueId field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVIntPtr getUniqueId() const;

    /**
     * Get the data timeStamp field.
     * @return PVStructurePtr.
     */
    epics::pvData::PVStructurePtr getDataTimeStamp() const;

    /**
     * Get the descriptor field.
     * @return The pvString or null if no function field.
     */
    epics::pvData::PVStringPtr getDescriptor() const;

    /**
     * Get the timeStamp field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getTimeStamp() const;

    /**
     * Get the alarm field.
     * @return PVStructurePtr which may be null.
     */
    epics::pvData::PVStructurePtr getAlarm() const;



private:
    NTNDArray(epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvData::PVStructurePtr pvNTNDArray;
};

}}
#endif  /* NTNDARRAY_H */
