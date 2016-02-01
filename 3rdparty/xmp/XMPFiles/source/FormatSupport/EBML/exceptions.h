// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2016 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#ifndef _EBML_exceptions_h_
#define _EBML_exceptions_h_

namespace ebml
{
	class invalid_type : public std::logic_error
	{
	public:
		explicit invalid_type() : std::logic_error("invalid type") {}
	};

	class invalid_size : public std::logic_error
	{
	public:
		explicit invalid_size() : std::logic_error("invalid size") {}
	};

	class invalid_level : public std::logic_error
	{
	public:
		explicit invalid_level() : std::logic_error("invalid level") {}
	};

	class invalid_doctype : public std::logic_error
	{
	public:
		explicit invalid_doctype() : std::logic_error("invalid doctype") {}
	};

	class read_error : public std::logic_error
	{
	public:
		explicit read_error() : std::logic_error("read error") {}
	};

	class parsing_aborted : public std::logic_error
	{
	public:
		explicit parsing_aborted() : std::logic_error("parsing aborted") {}
	};

	class no_elements : public std::logic_error
	{
	public:
		explicit no_elements() : std::logic_error("no elements") {}
	};
}

#endif /* _EBML_exceptions_h_ */
