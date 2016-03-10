/* 
 * Copyright 2015 Intel(r) Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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
    vmf_string m_name;
};

}

#endif /* __VMF_FIELD_VALUE_H__ */
