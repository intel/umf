// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#ifndef _EBML_variant_h_
#define _EBML_variant_h_

#if !defined(_MSC_FULL_VER) || _MSC_FULL_VER < 190023026
  // MSVC before 2015 doesn't support 'noexcept'
  #define _ALLOW_KEYWORD_MACROS 1
  #define noexcept throw()
#endif

namespace ebml
{
	class ebml_variant_t
	{
	public:
		explicit ebml_variant_t() noexcept : type(ebml_type_t::kEBMLTypeUnknown) {}
		explicit ebml_variant_t(ebml_unsigned_t value) : UnsignedValue(value), type(ebml_type_t::kEBMLTypeUnsigned) {}
		explicit ebml_variant_t(ebml_signed_t value) : SignedValue(value), type(ebml_type_t::kEBMLTypeSigned) {}
		explicit ebml_variant_t(const char * value) : StringValue(value ? value : ""), type(ebml_type_t::kEBMLTypeString) {}
		explicit ebml_variant_t(const ebml_string_t & value) : StringValue(value), type(ebml_type_t::kEBMLTypeString) {}
		explicit ebml_variant_t(const ebml_binary_t & value) : BinaryValue(value), type(ebml_type_t::kEBMLTypeBinary) {}
		explicit ebml_variant_t(ebml_float_t value) : FloatValue(value), type(ebml_type_t::kEBMLTypeFloat) {}
		virtual ~ebml_variant_t() noexcept {}

		ebml_size_t size() const;
		const void * data() const;
		std::string to_string() const;

		ebml_unsigned_t UnsignedValue;
		ebml_signed_t SignedValue;
		ebml_string_t StringValue;
		ebml_binary_t BinaryValue;
		ebml_float_t FloatValue;
		ebml_date_t DateValue;
		ebml_type_t type;
	};
}

#endif /* _EBML_variant_h_ */
