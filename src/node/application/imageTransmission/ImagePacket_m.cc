//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/imageTransmission/ImagePacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ImagePacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

ImagePacketHeader::ImagePacketHeader()
{
    nodeID = 0;
    locX = 0;
    locY = 0;
    imagePktNumber = 0;
    totalImgPackets = 0;
    setNumber = 0;
    currentSnapshotTime = 0;
    bufferSize = 0;
    imageSourceNumber = 0;
    originalImagesFolder = 0;
}

void doPacking(cCommBuffer *b, ImagePacketHeader& a)
{
    doPacking(b,a.nodeID);
    doPacking(b,a.locX);
    doPacking(b,a.locY);
    doPacking(b,a.imagePktNumber);
    doPacking(b,a.totalImgPackets);
    doPacking(b,a.setNumber);
    doPacking(b,a.currentSnapshotTime);
    doPacking(b,a.bufferSize);
    doPacking(b,a.imageSourceNumber);
    doPacking(b,a.originalImagesFolder);
}

void doUnpacking(cCommBuffer *b, ImagePacketHeader& a)
{
    doUnpacking(b,a.nodeID);
    doUnpacking(b,a.locX);
    doUnpacking(b,a.locY);
    doUnpacking(b,a.imagePktNumber);
    doUnpacking(b,a.totalImgPackets);
    doUnpacking(b,a.setNumber);
    doUnpacking(b,a.currentSnapshotTime);
    doUnpacking(b,a.bufferSize);
    doUnpacking(b,a.imageSourceNumber);
    doUnpacking(b,a.originalImagesFolder);
}

class ImagePacketHeaderDescriptor : public cClassDescriptor
{
  public:
    ImagePacketHeaderDescriptor();
    virtual ~ImagePacketHeaderDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ImagePacketHeaderDescriptor);

ImagePacketHeaderDescriptor::ImagePacketHeaderDescriptor() : cClassDescriptor("ImagePacketHeader", "")
{
}

ImagePacketHeaderDescriptor::~ImagePacketHeaderDescriptor()
{
}

bool ImagePacketHeaderDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ImagePacketHeader *>(obj)!=NULL;
}

const char *ImagePacketHeaderDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ImagePacketHeaderDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 10+basedesc->getFieldCount(object) : 10;
}

unsigned int ImagePacketHeaderDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<10) ? fieldTypeFlags[field] : 0;
}

const char *ImagePacketHeaderDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "nodeID",
        "locX",
        "locY",
        "imagePktNumber",
        "totalImgPackets",
        "setNumber",
        "currentSnapshotTime",
        "bufferSize",
        "imageSourceNumber",
        "originalImagesFolder",
    };
    return (field>=0 && field<10) ? fieldNames[field] : NULL;
}

int ImagePacketHeaderDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeID")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "locX")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "locY")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "imagePktNumber")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "totalImgPackets")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "setNumber")==0) return base+5;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentSnapshotTime")==0) return base+6;
    if (fieldName[0]=='b' && strcmp(fieldName, "bufferSize")==0) return base+7;
    if (fieldName[0]=='i' && strcmp(fieldName, "imageSourceNumber")==0) return base+8;
    if (fieldName[0]=='o' && strcmp(fieldName, "originalImagesFolder")==0) return base+9;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ImagePacketHeaderDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned short",
        "double",
        "double",
        "int",
        "int",
        "int",
        "simtime_t",
        "int",
        "int",
        "string",
    };
    return (field>=0 && field<10) ? fieldTypeStrings[field] : NULL;
}

const char *ImagePacketHeaderDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ImagePacketHeaderDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacketHeader *pp = (ImagePacketHeader *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ImagePacketHeaderDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacketHeader *pp = (ImagePacketHeader *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->nodeID);
        case 1: return double2string(pp->locX);
        case 2: return double2string(pp->locY);
        case 3: return long2string(pp->imagePktNumber);
        case 4: return long2string(pp->totalImgPackets);
        case 5: return long2string(pp->setNumber);
        case 6: return double2string(pp->currentSnapshotTime);
        case 7: return long2string(pp->bufferSize);
        case 8: return long2string(pp->imageSourceNumber);
        case 9: return oppstring2string(pp->originalImagesFolder);
        default: return "";
    }
}

bool ImagePacketHeaderDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacketHeader *pp = (ImagePacketHeader *)object; (void)pp;
    switch (field) {
        case 0: pp->nodeID = string2ulong(value); return true;
        case 1: pp->locX = string2double(value); return true;
        case 2: pp->locY = string2double(value); return true;
        case 3: pp->imagePktNumber = string2long(value); return true;
        case 4: pp->totalImgPackets = string2long(value); return true;
        case 5: pp->setNumber = string2long(value); return true;
        case 6: pp->currentSnapshotTime = string2double(value); return true;
        case 7: pp->bufferSize = string2long(value); return true;
        case 8: pp->imageSourceNumber = string2long(value); return true;
        case 9: pp->originalImagesFolder = (value); return true;
        default: return false;
    }
}

const char *ImagePacketHeaderDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *ImagePacketHeaderDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacketHeader *pp = (ImagePacketHeader *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ImagePacket);

ImagePacket::ImagePacket(const char *name, int kind) : ::ApplicationPacket(name,kind)
{
    bufferFragment_arraysize = 0;
    this->bufferFragment_var = 0;
}

ImagePacket::ImagePacket(const ImagePacket& other) : ::ApplicationPacket(other)
{
    bufferFragment_arraysize = 0;
    this->bufferFragment_var = 0;
    copy(other);
}

ImagePacket::~ImagePacket()
{
    delete [] bufferFragment_var;
}

ImagePacket& ImagePacket::operator=(const ImagePacket& other)
{
    if (this==&other) return *this;
    ::ApplicationPacket::operator=(other);
    copy(other);
    return *this;
}

void ImagePacket::copy(const ImagePacket& other)
{
    this->imgPktHeader_var = other.imgPktHeader_var;
    delete [] this->bufferFragment_var;
    this->bufferFragment_var = (other.bufferFragment_arraysize==0) ? NULL : new unsigned char[other.bufferFragment_arraysize];
    bufferFragment_arraysize = other.bufferFragment_arraysize;
    for (unsigned int i=0; i<bufferFragment_arraysize; i++)
        this->bufferFragment_var[i] = other.bufferFragment_var[i];
}

void ImagePacket::parsimPack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimPack(b);
    doPacking(b,this->imgPktHeader_var);
    b->pack(bufferFragment_arraysize);
    doPacking(b,this->bufferFragment_var,bufferFragment_arraysize);
}

void ImagePacket::parsimUnpack(cCommBuffer *b)
{
    ::ApplicationPacket::parsimUnpack(b);
    doUnpacking(b,this->imgPktHeader_var);
    delete [] this->bufferFragment_var;
    b->unpack(bufferFragment_arraysize);
    if (bufferFragment_arraysize==0) {
        this->bufferFragment_var = 0;
    } else {
        this->bufferFragment_var = new unsigned char[bufferFragment_arraysize];
        doUnpacking(b,this->bufferFragment_var,bufferFragment_arraysize);
    }
}

ImagePacketHeader& ImagePacket::getImgPktHeader()
{
    return imgPktHeader_var;
}

void ImagePacket::setImgPktHeader(const ImagePacketHeader& imgPktHeader)
{
    this->imgPktHeader_var = imgPktHeader;
}

void ImagePacket::setBufferFragmentArraySize(unsigned int size)
{
    unsigned char *bufferFragment_var2 = (size==0) ? NULL : new unsigned char[size];
    unsigned int sz = bufferFragment_arraysize < size ? bufferFragment_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        bufferFragment_var2[i] = this->bufferFragment_var[i];
    for (unsigned int i=sz; i<size; i++)
        bufferFragment_var2[i] = 0;
    bufferFragment_arraysize = size;
    delete [] this->bufferFragment_var;
    this->bufferFragment_var = bufferFragment_var2;
}

unsigned int ImagePacket::getBufferFragmentArraySize() const
{
    return bufferFragment_arraysize;
}

unsigned char ImagePacket::getBufferFragment(unsigned int k) const
{
    if (k>=bufferFragment_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferFragment_arraysize, k);
    return bufferFragment_var[k];
}

void ImagePacket::setBufferFragment(unsigned int k, unsigned char bufferFragment)
{
    if (k>=bufferFragment_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferFragment_arraysize, k);
    this->bufferFragment_var[k] = bufferFragment;
}

class ImagePacketDescriptor : public cClassDescriptor
{
  public:
    ImagePacketDescriptor();
    virtual ~ImagePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ImagePacketDescriptor);

ImagePacketDescriptor::ImagePacketDescriptor() : cClassDescriptor("ImagePacket", "ApplicationPacket")
{
}

ImagePacketDescriptor::~ImagePacketDescriptor()
{
}

bool ImagePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ImagePacket *>(obj)!=NULL;
}

const char *ImagePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ImagePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int ImagePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *ImagePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "imgPktHeader",
        "bufferFragment",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int ImagePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "imgPktHeader")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bufferFragment")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ImagePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "ImagePacketHeader",
        "unsigned char",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *ImagePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ImagePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacket *pp = (ImagePacket *)object; (void)pp;
    switch (field) {
        case 1: return pp->getBufferFragmentArraySize();
        default: return 0;
    }
}

std::string ImagePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacket *pp = (ImagePacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getImgPktHeader(); return out.str();}
        case 1: return ulong2string(pp->getBufferFragment(i));
        default: return "";
    }
}

bool ImagePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacket *pp = (ImagePacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setBufferFragment(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *ImagePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(ImagePacketHeader));
        default: return NULL;
    };
}

void *ImagePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ImagePacket *pp = (ImagePacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getImgPktHeader()); break;
        default: return NULL;
    }
}


