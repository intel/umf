// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file metadatareference.hpp
* \brief %Reference class header file
*/

#ifndef __VMF_METADATA_REFERENCE_H__
#define __VMF_METADATA_REFERENCE_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "metadatadesc.hpp"
#include "metadata.hpp"

namespace vmf
{
class Metadata;

class VMF_EXPORT Reference
{
private:
    std::shared_ptr<ReferenceDesc> desc;
    std::weak_ptr<Metadata>  md;

public:

    Reference();

    Reference(std::shared_ptr<ReferenceDesc>& desc, std::weak_ptr<Metadata>& md);
    Reference(std::shared_ptr<ReferenceDesc>& desc, const std::shared_ptr<Metadata>& md);

    ~Reference();

    std::weak_ptr<Metadata> getReferenceMetadata() const;
    std::shared_ptr<ReferenceDesc> getReferenceDescription() const;
    void setReferenceMetadata(const std::shared_ptr<Metadata>& spMetadata);
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_METADATA_REFERENCE_H__ */