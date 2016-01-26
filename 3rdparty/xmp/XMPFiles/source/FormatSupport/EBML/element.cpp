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

namespace ebml
{
	ebml_size_t ebml_element_t::content_size() const
	{
		ebml_size_t result(0);

		if (_value.type == ebml_type_t::kEBMLTypeMaster)
		{
			for (auto i : _children) result += i->size();
		}
		else result = _value.size();
		return result;
	}

	ebml_size_t ebml_element_t::size() const
	{
		auto cs = content_size();
		return cs + _id._length + size_for_int(cs);
	}

	void ebml_element_t::WriteVINT(XMP_IO * fileRef, const ebml_vint_t & value, bool adjust)
	{
		XMP_Assert(fileRef);

		auto shift = value._length - 1;
		ebml_size_t mask = (0x80 >> shift);
		mask <<= shift * 8;
		ebml_size_t v = adjust ? (value._value | mask) : value._value;

		for (auto i = 0; i < value._length; ++i)
		{
			auto bits = value._length - i - 1;
			auto shift = bits * 8;
			XMP_Uns8 byte = (v >> shift) & 0xFF;
			fileRef->Write(&byte, 1);
		}
	}

	void ebml_element_t::write(XMP_IO* fileRef)
	{
		XMP_Assert(fileRef);

		WriteVINT(fileRef, _id, false);
		WriteVINT(fileRef, ebml_vint_t(content_size()), true);

		if (_value.type == ebml_type_t::kEBMLTypeMaster) for (auto i : _children) i->write(fileRef);
		else fileRef->Write(_value.data(), _value.size());
	}

	void ebml_element_t::update_header(XMP_IO* fileRef)
	{
		XMP_Assert(fileRef);

		fileRef->Seek(_offset, kXMP_SeekFromStart);

		WriteVINT(fileRef, _id, false);
		WriteVINT(fileRef, _size, true);
	}

	void ebml_element_t::update_content(XMP_IO* fileRef)
	{
		XMP_Assert(fileRef);

		fileRef->Seek(_offset, kXMP_SeekFromStart);

		if (_value.type == ebml_type_t::kEBMLTypeMaster) throw invalid_type();
		fileRef->Write(_value.data(), _value.size());
	}

	void ebml_element_t::wipe(XMP_IO * fileRef)
	{
		XMP_Assert(fileRef);

		auto old_length = _id._length;

		_id._value = kVoid;
		_id._length = size_for_int(kVoid); // this is 1 byte

		auto diff = old_length - _id._length;

		_size._value += diff;

		update_header(fileRef);
	}

	ebml::ebml_element_t::vec ebml_element_t::getElementsById(ebml_id_t id) const noexcept
	{
		vec result;
		for (auto i : _children) if (id == i->_id._value) result.push_back(i);
		return result;
	}

	ebml::ebml_element_t::ptr ebml_element_t::getElementById(ebml_id_t id) const
	{
		auto elements = getElementsById(id);
		if (elements.empty()) throw no_elements();
		return getElementsById(id).front();
	}

}
