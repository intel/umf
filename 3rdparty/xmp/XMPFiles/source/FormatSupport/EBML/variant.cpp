// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "XMPFiles/source/FormatSupport/EBML_Support.hpp"
#include "XMPFiles/source/FormatSupport/EBML/exceptions.h"
#include "XMPFiles/source/FormatSupport/EBML/variant.h"

namespace ebml
{
	ebml_size_t ebml_variant_t::size() const
	{
		ebml_size_t result(0);
		switch (type)
		{
		case ebml_type_t::kEBMLTypeString:
		case ebml_type_t::kEBMLTypeUTF8: result = StringValue.size(); break;
		case ebml_type_t::kEBMLTypeUnsigned:
		case ebml_type_t::kEBMLTypeSigned:
		case ebml_type_t::kEBMLTypeFloat:
		case ebml_type_t::kEBMLTypeDate: result = 8; break; // for integral types always use 8 byte to represent them
		case ebml_type_t::kEBMLTypeBinary: result = BinaryValue.size(); break;
		default: throw invalid_type();
		}
		return result;
	}

	const void * ebml_variant_t::data() const
	{
		const void * result(nullptr);
		switch (type)
		{
		case ebml_type_t::kEBMLTypeUnsigned: result = &UnsignedValue; break;
		case ebml_type_t::kEBMLTypeSigned: result = &SignedValue; break;
		case ebml_type_t::kEBMLTypeString:
		case ebml_type_t::kEBMLTypeUTF8: result = StringValue.c_str(); break;
		case ebml_type_t::kEBMLTypeBinary: result = &BinaryValue[0]; break;
		case ebml_type_t::kEBMLTypeFloat: result = &FloatValue; break;
		case ebml_type_t::kEBMLTypeDate: result = &DateValue; break;
		default: throw invalid_type();
		}
		return result;
	}

	std::string ebml_variant_t::to_string() const
	{
		std::stringstream stream;
		switch (type)
		{
		case ebml_type_t::kEBMLTypeUnsigned: stream << UnsignedValue; break;
		case ebml_type_t::kEBMLTypeSigned: stream << SignedValue; break;
		case ebml_type_t::kEBMLTypeString: stream << StringValue; break;
		case ebml_type_t::kEBMLTypeUTF8: stream << StringValue; break;
		case ebml_type_t::kEBMLTypeDate: stream << DateValue; break;
		case ebml_type_t::kEBMLTypeFloat: stream << FloatValue; break;
		case ebml_type_t::kEBMLTypeBinary: for (auto i : BinaryValue) stream << std::hex << " " << i; break;
		default: throw invalid_type();
		}
		return stream.str();
	}
}
