// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPFiles/source/FormatSupport/EBML_Support.hpp"
#include "XMPFiles/source/FormatSupport/EBML/ids.h"
#include "XMPFiles/source/FormatSupport/EBML/exceptions.h"
#include "XMPFiles/source/FormatSupport/EBML/variant.h"
#include "XMPFiles/source/FormatSupport/EBML/element.h"

#ifndef EBML_VERBOSE
#define EBML_VERBOSE 0 /* use for debugging */
#endif /* EBML_VERBOSE */

namespace
{
	using namespace ebml;

	static ebml_string_t ReadString(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		std::string result;
		for (size_t i(0); i < size; ++i)
		{
			char byte(0);
			if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();
			result.push_back(byte);
		}
		return result;
	}

	static ebml_unsigned_t ReadUnsigned(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		ebml_unsigned_t result(0);
		if (size > 8) throw invalid_size();
		for (size_t i(0); i < size; ++i)
		{
			XMP_Uns8 byte(0);
			if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();
			result = (result << 8) | byte;
		}
		return result;
	}

	static ebml_signed_t ReadSigned(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		ebml_signed_t result(0);
		if (size > 8) throw invalid_size();
		if (size > 0)
		{
			XMP_Uns8 byte(0);
			if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();

			result = (byte & 0x80) ? -1 : 0;
			result |= byte & ~0x80;

			for (size_t i(0); i < size - 1; ++i)
			{
				if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();
				result = (result << 8) | byte;
			}
		}
		return result;
	}

	static ebml_float_t ReadFloat(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		ebml_float_t result(.0);
		if (size == 4)
		{
			float dummy(.0f);
			if (4 != fileRef->ReadAll(&dummy, 4)) throw read_error();
			result = dummy;
		}
		else if (size == 8)
		{
			if (8 != fileRef->ReadAll(&result, 8)) throw read_error();
		}
		else throw invalid_size();
		return result;
	}

	static ebml_date_t ReadDate(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		ebml_date_t result(0);
		if (size == 8)
		{
			if (8 != fileRef->ReadAll(&result, 8)) throw read_error();
		}
		else invalid_size();
		return result;
	}

	static ebml_binary_t ReadBinary(XMP_IO * fileRef, ebml_size_t size)
	{
		XMP_Assert(fileRef);
		ebml_binary_t result(size);
		if (size)
		{
			if (size != fileRef->ReadAll(&result[0], size)) throw read_error();
		}
		return result;
	}

	static ebml_variant_t ReadVariant(XMP_IO * fileRef, ebml_size_t size, ebml_type_t type)
	{
		XMP_Assert(fileRef);
		ebml_variant_t result;
		switch (result.type = type)
		{
		case ebml_type_t::kEBMLTypeUnsigned: result.UnsignedValue = ReadUnsigned(fileRef, size); break;
		case ebml_type_t::kEBMLTypeSigned: result.SignedValue = ReadSigned(fileRef, size); break;
		case ebml_type_t::kEBMLTypeString:
		case ebml_type_t::kEBMLTypeUTF8: result.StringValue = ReadString(fileRef, size); break;
		case ebml_type_t::kEBMLTypeDate: result.DateValue = ReadDate(fileRef, size); break;
		case ebml_type_t::kEBMLTypeFloat: result.FloatValue = ReadFloat(fileRef, size); break;
		case ebml_type_t::kEBMLTypeBinary: result.BinaryValue = ReadBinary(fileRef, size); break;
		default: throw invalid_type();
		}
		return result;
	}

	static ebml_vint_t ReadVINT(XMP_IO* fileRef, bool clear, ebml_unsigned_t max_size)
	{
		XMP_Assert(fileRef);

		XMP_Uns8 byte(0);
		if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();

		XMP_Uns8 count(1);
		XMP_Uns8 mask(0x80);

		while (mask > 0)
		{
			if (mask & byte) break;
			++count;
			mask >>= 1;
		}
		if (!mask) throw invalid_size();

		ebml_size_t value = clear ? (byte & ~mask) : byte;

		if (max_size != kEbmlInvalid && count > max_size) throw invalid_size();

		for (XMP_Uns8 i(0); i < count - 1; ++i)
		{
			if (1 != fileRef->ReadAll(&byte, 1)) throw read_error();
			value = (value << 8) | byte;
		}
		return ebml_vint_t{ count, value };
	}

	static ebml_size_t ReadEBMLHeader(XMP_IO* fileRef, ebml_restrictions_t & restrictions, ebml_vint_t & id, ebml_vint_t & size)
	{
		XMP_Assert(fileRef);

		id = ReadVINT(fileRef, false, restrictions.max_id_length);
		size = ReadVINT(fileRef, true, restrictions.max_size_length);
		return id._length + size._length;
	}

	typedef struct ebml_table_t
	{
		ebml_id_t id;
		ebml_type_t type;
		ebml_string_t name;
		ebml_level_t min_level;
		ebml_level_t max_level;
	}
	ebml_table;

	static const ebml_table_t elements[] =
	{
		{ kEBML, ebml_type_t::kEBMLTypeMaster, "EBML", 0, 0 },
		{ kDocType, ebml_type_t::kEBMLTypeString, "DocType", 1, 1 },
		{ kEBMLMaxIDLength, ebml_type_t::kEBMLTypeUnsigned, "EBMLMaxIDLength", 1, 1 },
		{ kEBMLMaxSizeLength, ebml_type_t::kEBMLTypeUnsigned, "EBMLMaxSizeLength", 1, 1 },
		{ kSegment, ebml_type_t::kEBMLTypeMaster, "Segment", 0, 0 },
		{ kTags, ebml_type_t::kEBMLTypeMaster, "Tags", 1, 1 },
		{ kTag, ebml_type_t::kEBMLTypeMaster, "Tag", 2, 2 },
		{ kTargets, ebml_type_t::kEBMLTypeMaster, "Targets", 3, 3 },
		{ kTargetType, ebml_type_t::kEBMLTypeString, "TargetType", 4, 4 },
		{ kSimpleTag, ebml_type_t::kEBMLTypeMaster, "SimpleTag", 3, kEbmlInvalid },
		{ kTagName, ebml_type_t::kEBMLTypeUTF8, "TagName", 4, kEbmlInvalid },
		{ kTagString, ebml_type_t::kEBMLTypeUTF8, "TagString", 4, kEbmlInvalid },
		{ kTagLanguage, ebml_type_t::kEBMLTypeString, "TagLanguage", 4, kEbmlInvalid },
		{ kTagDefault, ebml_type_t::kEBMLTypeUnsigned, "TagDefault", 4, kEbmlInvalid },
		{ kVoid, ebml_type_t::kEBMLTypeBinary, "Void", 0, kEbmlInvalid }
	};

	static ebml_type_t element_type(ebml_id_t id) noexcept
	{
		for (auto i : elements) if (i.id == id) return i.type;
		return ebml_type_t::kEBMLTypeUnknown;
	}

	static std::string element_name(ebml_id_t id) noexcept
	{
		for (auto i : elements) if (i.id == id) return i.name;
		return std::string();
	}

	static void check_level(ebml_id_t id, ebml_level_t level)
	{
		for (auto i : elements)
		{
			if (i.id == id)
			{
				if (level < i.min_level || level > i.max_level) throw invalid_level();
				break;
			}
		}
	}
}

namespace ebml
{
	void ReadEBMLMaster(XMP_IO * fileRef, ebml_element_t::ptr parent, ebml_restrictions_t & restrictions, ebml_level_t level, on_element_t callback)
	{
		XMP_Assert(fileRef);

		XMP_Int64 offset = fileRef->Offset() + parent->_size._value;

		for (ebml_size_t count(0); count < parent->_size._value;)
		{
			auto element = std::make_shared<ebml_element_t>();

			element->_offset = fileRef->Offset();
			
			count += ReadEBMLHeader(fileRef, restrictions, element->_id, element->_size);

			element->_content = fileRef->Offset();
			
			element->_level = level;
			element->_value.type = element_type(element->_id._value);
			element->_name = element_name(element->_id._value);
#if EBML_VERBOSE
			for (ebml_level_t i(0); i < level; ++i) std::cout << "\t";
			std::cout << char(element->_value.type) << " " << std::hex << element->_id._value << " " << element->_name << " " << std::dec << element->_size._value << std::endl;
#endif /* EBML_VERBOSE */
			check_level(element->_id._value, level);

			if (element->_value.type == ebml_type_t::kEBMLTypeMaster) ReadEBMLMaster(fileRef, element, restrictions, level + 1, callback);
			else if (element->_value.type == ebml_type_t::kEBMLTypeUnknown) fileRef->Seek(element->_size._value, kXMP_SeekFromCurrent);
			else
			{
				element->_value = ReadVariant(fileRef, element->_size._value, element->_value.type);
#if EBML_VERBOSE
				for (ebml_level_t i(0); i < level; ++i) std::cout << "\t";
				std::cout << element->_value.to_string() << std::endl;
#endif /* EBML_VERBOSE */
				if (element->_id._value == kEBMLMaxIDLength) restrictions.max_id_length = element->_value.UnsignedValue;
				else if (element->_id._value == kEBMLMaxSizeLength) restrictions.max_size_length = element->_value.UnsignedValue;
				else if (element->_id._value == kDocType)
				{
					if (element->_value.StringValue != kDocTypeMatroska && element->_value.StringValue != kDocTypeWebM) throw invalid_doctype();
				}
			}
			if (callback)
			{
				if (!callback(element)) throw parsing_aborted();
			}
			parent->_children.push_back(element);

			count += element->_size._value;
		}
		fileRef->Seek(offset, kXMP_SeekFromStart);
	}
}
