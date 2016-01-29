// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#ifndef _EBML_element_h_
#define _EBML_element_h_

namespace ebml
{
	class ebml_element_t
	{
	public:
		typedef std::shared_ptr<ebml_element_t> ptr;
		typedef std::vector<ptr> vec;

		explicit ebml_element_t() : _id(0), _size(0), _offset(0), _content(0), _level(0) {}
		explicit ebml_element_t(ebml_type_t type, ebml_size_t size) : _id(0), _size(size), _offset(0), _content(0), _level(0) { _value.type = type; }
		explicit ebml_element_t(ebml_id_t id) : _id(id), _size(0), _offset(0), _content(0), _level(0) {}
		explicit ebml_element_t(ebml_id_t id, const ebml_variant_t & value) : _id(id), _size(0), _offset(0), _content(0), _level(0), _value(value) {}
		explicit ebml_element_t(ebml_id_t id, ptr child) : _id(id), _size(0), _offset(0), _content(0), _level(0) { _children.push_back(child); _value.type = ebml_type_t::kEBMLTypeMaster; }
		explicit ebml_element_t(ebml_id_t id, vec children) :_id(id), _size(0), _offset(0), _content(0), _level(0), _children(children) { _value.type = ebml_type_t::kEBMLTypeMaster; }
		virtual ~ebml_element_t() noexcept {}

		ebml_size_t content_size() const;
		ebml_size_t size() const;
		void WriteVINT(XMP_IO * fileRef, const ebml_vint_t & value, bool adjust);
		void write(XMP_IO* fileRef);
		void update_header(XMP_IO* fileRef);
		void update_content(XMP_IO* fileRef);
		void wipe(XMP_IO * fileRef);
		vec getElementsById(ebml_id_t id) const noexcept;
		ptr getElementById(ebml_id_t id) const;

		std::string _name;
		ebml_vint_t _id, _size;
		ebml_offset_t _offset, _content;
		ebml_level_t _level;
		ebml_variant_t _value;
		vec _children;
	};
}

#endif /* _EBML_element_h_ */
