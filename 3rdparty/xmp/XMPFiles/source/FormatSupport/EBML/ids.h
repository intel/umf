// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#ifndef _EBML_ids_h_
#define _EBML_ids_h_

namespace ebml
{
	/* minimal set of common EBML Ids used in Matroska, according to the official specification http://matroska.org/technical/specs/index.html */
	static constexpr ebml_id_t kEBML = 0x1A45DFA3ULL;
	static constexpr ebml_id_t kDocType = 0x4282ULL;
	static constexpr ebml_id_t kEBMLMaxIDLength = 0x42F2;
	static constexpr ebml_id_t kEBMLMaxSizeLength = 0x42F3;
	static constexpr ebml_id_t kSegment = 0x18538067;
	static constexpr ebml_id_t kTags = 0x1254C367;
	static constexpr ebml_id_t kTag = 0x7373;
	static constexpr ebml_id_t kTargets = 0x63C0;
	static constexpr ebml_id_t kTargetType = 0x63CA;
	static constexpr ebml_id_t kSimpleTag = 0x67C8;
	static constexpr ebml_id_t kTagName = 0x45A3;
	static constexpr ebml_id_t kTagString = 0x4487;
	static constexpr ebml_id_t kTagLanguage = 0x447A;
	static constexpr ebml_id_t kTagDefault = 0x4484;
	static constexpr ebml_id_t kVoid = 0xEC;
}

#endif /* _EBML_ids_h_ */
