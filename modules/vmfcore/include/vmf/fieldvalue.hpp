// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __VMF_FIELD_VALUE_H__
#define __VMF_FIELD_VALUE_H__

#include "variant.hpp"
#include "global.hpp"
#include <string>

namespace vmf
{
class VMF_EXPORT FieldValue : public vmf::Variant
{
public:
    FieldValue() : m_name("")
    {
    }

    FieldValue( const FieldValue& other )
    {
        *this = other;
    }

    virtual ~FieldValue(void)
    {
    }

    FieldValue( const std::string& name, vmf::Variant variant )
        : vmf::Variant( variant )
        , m_name( name )
    {
    }

    const std::string& getName() const { return m_name; }

    FieldValue& operator = ( const FieldValue& other )
    {
        m_name = other.m_name;
        Variant::operator = ( other );

        return *this;
    }

    bool operator == ( const FieldValue& other ) const
    {
        return m_name == other.m_name && Variant::operator == ( other );
    }

private:
    MetaString m_name;
};

}

#endif /* __VMF_FIELD_VALUE_H__ */
