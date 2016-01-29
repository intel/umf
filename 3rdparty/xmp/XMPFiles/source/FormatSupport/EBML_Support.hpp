#ifndef __EBML_Support_hpp__
#define __EBML_Support_hpp__ 1

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "public/include/XMP_Environment.h"	// ! XMP_Environment.h must be the first included header.

#include "public/include/XMP_Const.h"
#include "public/include/XMP_IO.hpp"

#include "XMPFiles/source/XMPFiles_Impl.hpp"
#include "source/XMPFiles_IO.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <functional>
#include <limits>

namespace ebml
{
	typedef XMP_Int64 ebml_offset_t;
	typedef XMP_Uns64 ebml_size_t;
	typedef XMP_Uns64 ebml_id_t;
	typedef XMP_Uns64 ebml_date_t;
	typedef XMP_Uns64 ebml_unsigned_t;
	typedef XMP_Int64 ebml_signed_t;
	typedef std::string ebml_string_t;
	typedef std::string ebml_utf8_t;
	typedef double ebml_float_t;
	typedef XMP_Uns64 ebml_level_t;

	template<typename T>
	ebml_size_t size_for_int(T value)
	{
		ebml_size_t result(1); // always use at least one byte to represent integer value
		value >>= 8;
		while (value)
		{
			++result;
			value >>= 8;
		}
		return result;
	}

	typedef struct ebml_vint_t
	{
		explicit ebml_vint_t(ebml_size_t length, ebml_size_t value) : _length(length), _value(value) {}
		explicit ebml_vint_t(ebml_size_t value) : _length(size_for_int(value)), _value(value) {}
		ebml_size_t _length;
		ebml_size_t _value;
	}
	ebml_vint;

	typedef std::vector<XMP_Uns8> ebml_binary_t;

	enum class ebml_type_t : char
	{
		kEBMLTypeMaster = 'm',
		kEBMLTypeUnsigned = 'u',
		kEBMLTypeSigned = 'i',
		kEBMLTypeString = 's',
		kEBMLTypeUTF8 = '8',
		kEBMLTypeBinary = 'b',
		kEBMLTypeFloat = 'f',
		kEBMLTypeDate = 'd',
		kEBMLTypeUnknown = '?'
	};

	static const ebml_string_t kDocTypeMatroska = "matroska";
	static const ebml_string_t kDocTypeWebM = "webm";	

	static const ebml_unsigned_t kEbmlInvalid = (std::numeric_limits<ebml_unsigned_t>::max)();

	typedef struct ebml_restrictions_t
	{
		ebml_unsigned_t max_id_length;
		ebml_unsigned_t max_size_length;
	}
	ebml_restrictions;

	class ebml_element_t;

	typedef std::function<bool(std::shared_ptr<ebml_element_t>)> on_element_t;

	void ReadEBMLMaster(XMP_IO * fileRef, std::shared_ptr<ebml_element_t> parent, ebml_restrictions_t & restrictions, ebml_level_t level, on_element_t callback);
}

#endif	// __EBML_Support_hpp__
