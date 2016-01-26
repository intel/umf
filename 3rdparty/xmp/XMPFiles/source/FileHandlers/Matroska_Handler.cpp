// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPFiles/source/FileHandlers/Matroska_Handler.hpp"
#include "XMPFiles/source/FormatSupport/EBML_Support.hpp"
#include "XMPFiles/source/FormatSupport/EBML/ids.h"
#include "XMPFiles/source/FormatSupport/EBML/exceptions.h"
#include "XMPFiles/source/FormatSupport/EBML/variant.h"
#include "XMPFiles/source/FormatSupport/EBML/element.h"

using namespace ebml;

// =================================================================================================
// Matroska_CheckFormat
// ===============
//
// A Matroska file must begin with EBML [1A][45][DF][A3], a 4 byte length

class MatroskaDOM
{
public:
    MatroskaDOM(XMP_IO * fileRef)
    {
        assert(fileRef);
        fileRef->Rewind();

        _root = std::make_shared<ebml_element_t>(ebml_type_t::kEBMLTypeMaster, fileRef->Length());

        ebml_restrictions restrictions;
        restrictions.max_id_length = 4;
        restrictions.max_size_length = 8;

        ReadEBMLMaster(fileRef, _root, restrictions, 0, std::bind(&MatroskaDOM::OnElement, this, std::placeholders::_1));

        auto segments = _root->getElementsById(kSegment);
        if (segments.empty()) throw no_elements();
    }
    bool OnElement(ebml_element_t::ptr element)
    {
        return true;
    }
    ebml_element_t::ptr _root;
};

bool Matroska_CheckFormat(XMP_FileFormat  format,
    XMP_StringPtr    filePath,
    XMP_IO*      	file,
    XMPFiles*        parent)
{
    IgnoreParam(format); IgnoreParam(parent);
    XMP_Assert(format == kXMP_MatroskaFile);

    try
    {
        MatroskaDOM DOM(file);

        return true;
    }
    catch (std::logic_error &)
    {
        return false;
    }
}	// Matroska_CheckFormat

extern XMPFileHandler * Matroska_MetaHandlerCTor(XMPFiles * parent)
{
    return new Matroska_MetaHandler(parent);
}

Matroska_MetaHandler::Matroska_MetaHandler(XMPFiles* parent)
    : XMPFileHandler(parent)
{
    this->parent = parent;
    this->handlerFlags = kMatroska_HandlerFlags;
    this->stdCharForm = kXMP_Char8Bit;
}

void Matroska_MetaHandler::CacheFileData()
{
    XMP_Assert(parent && parent->ioRef);
    try
    {
        containsXMP = false;
        _dom = std::make_shared<MatroskaDOM>(parent->ioRef);

        for (auto segment : _dom->_root->getElementsById(kSegment))
        {
            for (auto tags : segment->getElementsById(kTags))
            {
                for (auto tag : tags->getElementsById(kTag))
                {
                    for (auto simple_tag : tag->getElementsById(kSimpleTag))
                    {
                        auto tag_name = simple_tag->getElementById(kTagName);
                        if (tag_name->_value.StringValue == "XMP")
                        {
                            auto tag_string = simple_tag->getElementById(kTagString);

                            xmpPacket = tag_string->_value.StringValue;
                            containsXMP = true;

                            break;
                        }
                    }
                }
            }
        }
    }
    catch (std::logic_error & e)
    {
        e.what();
        XMP_Validate(false, e.what(), kXMPErr_BadFileFormat);
    }
}

void Matroska_MetaHandler::UpdateFile(bool doSafeUpdate)
{
    if (!needsUpdate) return; // If needsUpdate is set then at least the XMP changed.

    needsUpdate = false;	// Make sure only called once.
    XMP_Assert(!doSafeUpdate);	// This should only be called for "unsafe" updates.
    XMP_Assert(parent && parent->ioRef);

    XMP_AbortProc abortProc = parent->abortProc;
    void *        abortArg = parent->abortArg;
    const bool    checkAbort = (abortProc != 0);

    bool localProgressTracking(false);
    XMP_ProgressTracker* progressTracker = parent->progressTracker;
    if (progressTracker)
    {
        float xmpSize = static_cast<float>(xmpPacket.size());
        if (progressTracker->WorkInProgress())
        {
            progressTracker->AddTotalWork(xmpSize);
        }
        else
        {
            localProgressTracking = true;
            progressTracker->BeginWork(xmpSize);
        }
    }
    UpdateXMP();

    if (localProgressTracking) progressTracker->WorkComplete();
}

void Matroska_MetaHandler::UpdateXMP()
{
    XMP_IO* fileRef = parent->ioRef;

    fileRef->Seek(fileRef->Length(), kXMP_SeekFromStart);

    for (auto segment : _dom->_root->getElementsById(kSegment))
    {
        for (auto tags : segment->getElementsById(kTags))
        {
            for (auto tag : tags->getElementsById(kTag))
            {
                for (auto simple_tag : tag->getElementsById(kSimpleTag))
                {
                    auto tag_name = simple_tag->getElementById(kTagName);
                    if (tag_name->_value.StringValue == "XMP")
                    {
                        auto tag_string = simple_tag->getElementById(kTagString);

                        // we have found valid XMP, and if it's in the very end of file, we can truncate segment
                        if (tag_string->_offset + tag_string->size() == fileRef->Length())
                        {
                            segment->_size._value -= tags->size();

                            fileRef->Truncate(tags->_offset);
                            fileRef->Seek(tags->_offset, kXMP_SeekFromStart);
                        }
                        // otherwise, make old XMP tag Void and create new one from the scratch
                        else
                        {
                            tags->wipe(fileRef);
                        }
                    }
                }
            }
        }
    }
    auto segments = _dom->_root->getElementsById(kSegment);
    auto segment = segments.back();

    auto tag_name = std::make_shared<ebml_element_t>(kTagName, ebml_variant_t("XMP"));
    auto tag_string = std::make_shared<ebml_element_t>(kTagString, ebml_variant_t(xmpPacket));
    auto tag_language = std::make_shared<ebml_element_t>(kTagLanguage, ebml_variant_t("eng"));
    auto tag_default = std::make_shared<ebml_element_t>(kTagDefault, ebml_variant_t(1ULL));
    auto simple_tag = std::make_shared<ebml_element_t>(kSimpleTag, ebml_element_t::vec{ tag_language, tag_default, tag_name, tag_string });
    auto tag = std::make_shared<ebml_element_t>(kTag, simple_tag);
    auto tags = std::make_shared<ebml_element_t>(kTags, tag);

    tags->write(fileRef);

    segment->_size._value += tags->size();
    segment->update_header(fileRef);
}

void Matroska_MetaHandler::WriteTempFile(XMP_IO* tempRef)
{
    XMP_Assert(needsUpdate);

    XMP_IO* originalRef = parent->ioRef;
    
    bool localProgressTracking(false);
    XMP_ProgressTracker* progressTracker = parent->progressTracker;
    if (progressTracker)
    {
        float xmpSize = static_cast<float>(xmpPacket.size());
        if (progressTracker->WorkInProgress())
        {
            progressTracker->AddTotalWork(xmpSize);
        }
        else
        {
            localProgressTracking = true;
            progressTracker->BeginWork(xmpSize);
        }
    }

    XMP_Assert(tempRef);
    XMP_Assert(originalRef);

    tempRef->Rewind();
    originalRef->Rewind();
    XIO::Copy(originalRef, tempRef, originalRef->Length(), parent->abortProc, parent->abortArg);

    try
    {
        parent->ioRef = tempRef;	// ! Fool UpdateFile into using the temp file.
        UpdateFile(false);
        parent->ioRef = originalRef;
    }
    catch (...)
    {
        parent->ioRef = originalRef;
        throw;
    }
    if (localProgressTracking) progressTracker->WorkComplete();
}

void Matroska_MetaHandler::ProcessXMP()
{
    if (processedXMP) return;
    processedXMP = true;	// Make sure we only come through here once.

    // Process the XMP packet.

    if (!xmpPacket.empty())
    {
        XMP_Assert(containsXMP);

        xmpObj.ParseFromBuffer(xmpPacket.c_str(), static_cast<XMP_StringLen>(xmpPacket.size()));

        containsXMP = true;
    }
}
