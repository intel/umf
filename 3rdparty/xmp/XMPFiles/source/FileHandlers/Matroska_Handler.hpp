// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#ifndef __Matroska_Handler_hpp__
#define __Matroska_Handler_hpp__	1

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.
#include "public/include/XMP_Const.h"

#include "source/Endian.h"
#include "source/XIO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"

#include <memory>

#if !defined(_MSC_FULL_VER) || _MSC_FULL_VER < 190023026
  // MSVC before 2015 doesn't support 'noexcept'
  #define _ALLOW_KEYWORD_MACROS 1
  #define noexcept throw()
#endif

extern XMPFileHandler * Matroska_MetaHandlerCTor(XMPFiles * parent);

extern bool Matroska_CheckFormat(XMP_FileFormat format,
	XMP_StringPtr  filePath,
	XMP_IO*	fileRef,
	XMPFiles *	 parent);

static const XMP_OptionBits kMatroska_HandlerFlags = (kXMPFiles_CanInjectXMP |
	kXMPFiles_CanExpand |
	kXMPFiles_CanRewrite |
	kXMPFiles_PrefersInPlace |
	kXMPFiles_CanReconcile |
	kXMPFiles_AllowsOnlyXMP |
	kXMPFiles_ReturnsRawPacket |
	kXMPFiles_AllowsSafeUpdate |
	kXMPFiles_CanNotifyProgress
	);

class MatroskaDOM;

class Matroska_MetaHandler : public XMPFileHandler
{
public:
	explicit Matroska_MetaHandler(XMPFiles* parent);
	~Matroska_MetaHandler() noexcept {}

	void CacheFileData() override;
	void UpdateFile(bool doSafeUpdate) override;
	void WriteTempFile(XMP_IO* tempRef) override;
	void ProcessXMP() override;
	void UpdateXMP();
	std::shared_ptr<MatroskaDOM> _dom;
};

#endif /* __Matroska_Handler_hpp__ */
