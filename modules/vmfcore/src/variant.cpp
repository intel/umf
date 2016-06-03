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
#include "vmf/variant.hpp"
#include <cstring>
#include <string>
#include <memory>
#include <sstream>
#include <cmath>
#include <limits>
#include <iomanip>

namespace vmf
{

class IData
{
public:
    virtual ~IData() {}
    virtual IData* clone() const = 0;
};

template<class T> class Data : public IData
{
public:
    T content;
    Data(const T& value) : content(value) {}
    ~Data() {}
    IData* clone() const
    {
        return new Data<T>(content);
    }
};



Variant::Variant() : data(nullptr), m_type(type_empty) {}

Variant::Variant(const Variant& other) : data(other.data ? other.data->clone() : nullptr), m_type(other.getType()) {}

Variant::Variant(Variant&& other)
{
	data = other.data; other.data = nullptr;
	m_type = other.m_type; other.m_type = Variant::type_empty;
}

Variant::~Variant()
{
    release();
}

#define IMPLEMENT_VMF_TYPE( T )\
Variant::Variant( const vmf_##T& value)\
{\
    m_type = type_##T;\
    data = new Data<vmf_##T>(value);\
}\
Variant& Variant::operator = ( const vmf_##T& value )\
{\
    release();\
    m_type = type_##T;\
    data = new Data<vmf_##T>(value);\
    return *this;\
}\
const vmf_##T& Variant::get_##T() const\
{\
    if( type_##T == m_type )\
    {\
        return dynamic_cast<Data<vmf_##T>*>(data)->content;\
    }\
    VMF_EXCEPTION(TypeCastException, "bad cast");\
}\
Variant::operator const vmf_##T& () const\
{\
    return get_##T();\
}

IMPLEMENT_VMF_TYPE( integer )
IMPLEMENT_VMF_TYPE( real )
IMPLEMENT_VMF_TYPE( string )
IMPLEMENT_VMF_TYPE( vec2d )
IMPLEMENT_VMF_TYPE( vec3d )
IMPLEMENT_VMF_TYPE( vec4d )
IMPLEMENT_VMF_TYPE( rawbuffer )

#define IMPLEMENT_VECTOR_VMF_TYPE( T ) \
Variant::Variant( const std::vector<vmf_##T>& value)\
{\
    m_type = type_##T##_vector;\
    data = new Data<std::vector<vmf_##T>>(value);\
}\
Variant& Variant::operator = ( const std::vector<vmf_##T>& value )\
{\
    release();\
    m_type = type_##T##_vector;\
    data = new Data<std::vector<vmf_##T>>(value);\
    return *this;\
}\
const std::vector<vmf_##T>& Variant::get_##T##_vector() const\
{\
    if( type_##T##_vector == m_type )\
    {\
        return dynamic_cast<Data<std::vector<vmf_##T>>*>(data)->content;\
    }\
    VMF_EXCEPTION(TypeCastException, "bad cast");\
}\
Variant::operator const std::vector<vmf_##T>& () const\
{\
    return get_##T##_vector();\
}

IMPLEMENT_VECTOR_VMF_TYPE( integer )
IMPLEMENT_VECTOR_VMF_TYPE( real )
IMPLEMENT_VECTOR_VMF_TYPE( string )
IMPLEMENT_VECTOR_VMF_TYPE( vec2d )
IMPLEMENT_VECTOR_VMF_TYPE( vec3d )
IMPLEMENT_VECTOR_VMF_TYPE( vec4d )

Variant::Variant(const int& value) : data(new Data<vmf_integer>(value)), m_type(type_integer) {}
Variant::Variant(const unsigned int& value) : data(new Data<vmf_integer>(value)), m_type(type_integer) {}

Variant::Variant(const float& value) : data(new Data<vmf_real>(value)), m_type(type_real) {}

Variant& Variant::operator = ( const int& value )
{
    release();

    m_type = type_integer;
    data = new Data<vmf_integer>(value);
    return *this;
}

Variant& Variant::operator = (const unsigned int& value)
{
    release();

    m_type = type_integer;
    data = new Data<vmf_integer>(value);
    return *this;
}

Variant& Variant::operator = (const float& value)
{
    release();

    m_type = type_real;
    data = new Data<vmf_real>(value);
    return *this;
}

Variant::Variant( const std::vector<int>& value ) : m_type(type_integer_vector)
{
    data = new Data<std::vector<vmf_integer>>(std::vector<vmf_integer>(value.begin(), value.end()));
}

Variant::Variant( const std::vector<float>& value ) :  m_type(type_real_vector)
{
    data = new Data<std::vector<vmf_real>>(std::vector<vmf_real>(value.begin(), value.end()));
}

Variant& Variant::operator = ( const std::vector<int>& value )
{
    release();

    m_type = type_integer_vector;
    data = new Data<std::vector<vmf_integer>>(std::vector<vmf_integer>(value.begin(), value.end()));
    return *this;
}

Variant& Variant::operator = (const std::vector<float>& value)
{
    release();

    m_type = type_real_vector;
    data = new Data<std::vector<vmf_real>>(std::vector<vmf_real>(value.begin(), value.end()));
    return *this;
}

Variant::Variant(const char* pszString) : data(new Data<vmf_string>(std::string(pszString))), m_type(type_string) {}

Variant& Variant::operator = ( const char* pszString )
{
    release();

    m_type = type_string;
    data = new Data<vmf_string>(std::string(pszString));
    return *this;
}

Variant& Variant::operator = (const Variant& other)
{
    if (this != &other)
    {
        release();

        m_type = other.getType();
		if (m_type == type_empty || other.data == nullptr)
            data = nullptr;
        else
            data = other.data->clone();
    }

    return *this;
}

Variant& Variant::operator = (Variant&& other)
{
    if(this != &other)
    {
        std::swap(this->data, other.data);
        std::swap(this->m_type, other.m_type);
    }

    return *this;
}

#define COMPARE_OBJECT(T) \
    case type_##T: \
        bIsEqual = dynamic_cast<Data<vmf_##T>*>(data)->content == other.get_##T(); \
        break;

#define COMPARE_VECTOR_OBJECT( T ) \
    case type_##T##_vector: \
    { \
        auto content = dynamic_cast<Data<std::vector<vmf_##T>>*>(data)->content; \
        if( content.size() == other.get_##T##_vector().size() ) \
            bIsEqual = std::equal( content.begin(), content.end(), other.get_##T##_vector().begin() ); \
    } \
        break;

bool Variant::operator == (const Variant& other) const
{
    if( m_type != other.m_type )
    {
        if( isConvertible( other.getType(), m_type ) )
        {
            Variant newVar(other);
            newVar.convertTo( m_type );

            return this->operator ==( newVar );
        }
        else if( isConvertible( m_type, other.getType() ) )
        {
            Variant newVar(*this);
            newVar.convertTo( other.getType() );

            return newVar.operator ==( other );
        }
        else
        {
            VMF_EXCEPTION(IncorrectParamException, "Can't compare Variant objects with incompatible types");
        }
    }

    bool bIsEqual = false;
    switch(m_type)
    {
        COMPARE_OBJECT( integer )
        case type_real:
            bIsEqual = DOUBLE_EQ(dynamic_cast<Data<vmf_real>*>(data)->content, other.get_real());
        break;
        COMPARE_OBJECT( string )
        COMPARE_OBJECT( vec2d )
        COMPARE_OBJECT( vec3d )
        COMPARE_OBJECT( vec4d )
        COMPARE_OBJECT( rawbuffer )
        COMPARE_VECTOR_OBJECT( integer )
        case type_real_vector:
        {
            auto content = dynamic_cast<Data<std::vector<vmf_real>>*>(data)->content;
            if( content.size() == other.get_real_vector().size() )
                bIsEqual = std::equal( content.begin(), content.end(), other.get_real_vector().begin(), DOUBLE_EQ );
        }
        break;
        COMPARE_VECTOR_OBJECT(string)
        COMPARE_VECTOR_OBJECT(vec2d)
        COMPARE_VECTOR_OBJECT(vec3d)
        COMPARE_VECTOR_OBJECT(vec4d)
        case type_empty:
            bIsEqual = true;
            break;
        default:
        VMF_EXCEPTION(IncorrectParamException, "unknown type.");
        break;
    }

    return bIsEqual;
}

bool Variant::operator != ( const Variant& other ) const
{
    return !( this->operator == ( other ));
}

#define SIMPLE_TYPE_TO_STRING( VAL ) \
    ss << VAL;

#define REAL_TYPE_TO_STRING( VAL ) \
    ss << std::setprecision(std::numeric_limits<double>::digits10) << VAL;

#define VEC2_TYPE_TO_STRING( VAL ) \
    ss  << std::setprecision(std::numeric_limits<double>::digits10) << VAL.x << \
    " " << std::setprecision(std::numeric_limits<double>::digits10) << VAL.y;

#define VEC3_TYPE_TO_STRING(VAL) \
    VEC2_TYPE_TO_STRING(VAL) \
    ss << " " << std::setprecision(std::numeric_limits<double>::digits10) << VAL.z;

#define VEC4_TYPE_TO_STRING( VAL ) \
    VEC3_TYPE_TO_STRING(VAL) \
    ss << " " << std::setprecision(std::numeric_limits<double>::digits10) << VAL.w;

#define VECTOR_TYPE_TO_STRING( T , OP ) \
{ \
    auto content = dynamic_cast<Data<std::vector<vmf_##T>>*>(data)->content; \
    const char * separator = ""; \
    for(auto it = content.begin(); it != content.end(); it++) \
    { \
        ss << separator; \
        OP( (*it) ) \
        separator = " ; "; \
    } \
}

std::string Variant::toString(bool withType) const
{
    std::stringstream ss;

    if (withType) ss << '(' << getTypeName() << ')' << ' ';

    switch( m_type )
    {
    default:
    case type_empty:
        ss << "<empty value>";
        break;
    case type_integer:
        SIMPLE_TYPE_TO_STRING(dynamic_cast<Data<vmf_integer>*>(data)->content)
        break;
    case type_real:
        REAL_TYPE_TO_STRING(dynamic_cast<Data<vmf_real>*>(data)->content)
            break;
    case type_string:
        SIMPLE_TYPE_TO_STRING(dynamic_cast<Data<vmf_string>*>(data)->content.c_str())
        break;
    case type_vec2d:
        VEC2_TYPE_TO_STRING(dynamic_cast<Data<vmf_vec2d>*>(data)->content)
        break;
    case type_vec3d:
        VEC3_TYPE_TO_STRING(dynamic_cast<Data<vmf_vec3d>*>(data)->content)
        break;
    case type_vec4d:
        VEC4_TYPE_TO_STRING(dynamic_cast<Data<vmf_vec4d>*>(data)->content)
        break;
    case type_rawbuffer:
        ss << base64encode(dynamic_cast<Data<vmf_rawbuffer>*>(data)->content);
        break;
    case type_integer_vector:
        VECTOR_TYPE_TO_STRING(integer, SIMPLE_TYPE_TO_STRING)
        break;
    case type_real_vector:
        VECTOR_TYPE_TO_STRING(real, REAL_TYPE_TO_STRING)
        break;
    case type_string_vector:
        {
            auto content = dynamic_cast<Data<std::vector<vmf_string>>*>(data)->content;
            const char * separator = "";
            for (auto it = content.begin(); it != content.end(); it++)
            {
                ss << separator;
                SIMPLE_TYPE_TO_STRING(base64encode(vmf_rawbuffer(it->c_str(), it->size() + 1)))
                separator = " ; ";
            }
        }
        break;
    case type_vec2d_vector:
        VECTOR_TYPE_TO_STRING(vec2d, VEC2_TYPE_TO_STRING)
        break;
    case type_vec3d_vector:
        VECTOR_TYPE_TO_STRING(vec3d, VEC3_TYPE_TO_STRING)
        break;
    case type_vec4d_vector:
        VECTOR_TYPE_TO_STRING(vec4d, VEC4_TYPE_TO_STRING)
        break;
    }

    return ss.str();
}

void Variant::fromString(const std::string& value)
{
    size_t i = 0;
    while (value[i] == ' ') i++;
    if (value[i] == '(')
    {
        size_t j = value.find(')');
        if (j != value.npos)
        {
            m_type = typeFromString(value.substr(i + 1, j - i - 1));
            if (j + 1 < value.size() && value[j + 1] == ' ') j++;
            fromString(m_type, value.substr(j + 1));
            return;
        }
    }
    VMF_EXCEPTION(IncorrectParamException, "Error decoding value string: " + value);
}

void Variant::fromString(Type eType, const std::string& sValue)
{
    release();

    m_type = eType;
    std::string value(sValue);

    // skipping "(type)" if any
    size_t j = value.find(')');
    if (j != value.npos) value = value.substr(j + 1);


    std::stringstream ss(value);
    switch (m_type)
    {
    case type_empty:
        break;
    case type_integer:
        {
            vmf_integer temp_integer;
            ss >> temp_integer;
            data = new Data<vmf_integer>(temp_integer);
        }
        break;
    case type_real:
        {
            vmf_real temp_real;
            ss >> temp_real;
            data = new Data<vmf_real>(temp_real);
        }
        break;
    case type_string:
        data = new Data<vmf_string>(sValue);
        break;
    case type_vec2d:
        {
            vmf_real x, y;
            ss >> x >> y;
            data = new Data<vmf_vec2d>(vmf_vec2d(x, y));
        }
        break;
    case type_vec3d:
        {
            vmf_real x, y, z;
            ss >> x >> y >> z;
            data = new Data<vmf_vec3d>(vmf_vec3d(x, y, z));
        }
        break;
    case type_vec4d:
        {
            vmf_real x, y, z, w;
            ss >> x >> y >> z >> w;
            data = new Data<vmf_vec4d>(vmf_vec4d(x, y, z, w));
        }
        break;
    case type_rawbuffer:
        {
            std::string s;
            ss >> s;
            data = new Data<vmf_rawbuffer>(vmf_rawbuffer(base64decode(s)));
            break;
        }
        break;
    case type_integer_vector:
        {
            std::vector<vmf_integer> vec;
            while(ss)
            {
                vmf_integer temp;
                vmf_char separator = ';';
                ss >> temp;
                vec.push_back(temp);
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_integer>>(vec);
        }
        break;
    case type_real_vector:
        {
            std::vector<vmf_real> vec;
            while(ss)
            {
                vmf_real temp;
                vmf_char separator = ';';
                ss >> temp;
                vec.push_back(temp);
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_real>>(vec);
        }
        break;
    case type_string_vector:
        {
            std::vector<vmf_string> vec;
            while(ss)
            {
                vmf_string temp;
                vmf_char separator = ';';
                ss >> temp;
                vec.push_back( base64decode(temp).data() );
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_string>>(vec);
        }
        break;
    case type_vec2d_vector:
        {
            std::vector<vmf_vec2d> vec;
            vmf_char separator = ';';
            while(ss)
            {
                vmf_real x, y;
                ss >> x >> y;
                vec.push_back( vmf_vec2d(x,y) );
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_vec2d>>(vec);
        }
        break;
    case type_vec3d_vector:
        {
            std::vector<vmf_vec3d> vec;
            vmf_char separator = ';';
            while(ss)
            {
                vmf_real x, y, z;
                ss >> x >> y >> z;
                vec.push_back( vmf_vec3d(x, y, z) );
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_vec3d>>(vec);
        }
        break;
    case type_vec4d_vector:
        {
            std::vector<vmf_vec4d> vec;
            vmf_char separator = ';';
            while(ss)
            {
                vmf_real x, y, z, w;
                ss >> x >> y >> z >> w;
                vec.push_back( vmf_vec4d(x, y, z, w) );
                if(ss)
                    ss >> separator;
                if(separator != ';')
                    VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid array item separator: " + to_string(separator));
            }
            data = new Data<std::vector<vmf_vec4d>>(vec);
        }
        break;
    default:
        VMF_EXCEPTION(IncorrectParamException, "unexpected type");
    }
}

Variant::Type Variant::getType() const
{
    return m_type;
}

bool Variant::isEmpty() const
{
    return m_type == type_empty;
}

std::string Variant::getTypeName() const
{
    return typeToString(m_type);
}

void Variant::convertTo(Type type)
{
    // No need to convert if the types are the same
    if (m_type == type)
        return;

    // Check if the types are convertible
    if (!isConvertible(m_type, type))
    {
        VMF_EXCEPTION(TypeCastException, "Cannot convert value to the target type!" );
    }

    // Convert value to double, and check to see if the value is out of range of what the new type can represent.
    std::string sValue = toString();
    double fValue;
    std::stringstream ss( sValue );
    ss >> fValue;

    if (fValue < minLimit<double>(type) || fValue > maxLimit<double>(type))
    {
        VMF_EXCEPTION(OutOfRangeException, "Cannot convert value exceeds limit!" );
    }

    // Now convert the value to the new type
    this->fromString(type, sValue);
}

#define TYPE_TO_STRING(T, VAL) \
    case(vmf::Variant::type_##T): \
    sType = #VAL;\
    break;

std::string Variant::typeToString(Type t)
{
    std::string sType;
    switch (t)
    {
        default:
        TYPE_TO_STRING( empty, empty )
        TYPE_TO_STRING( integer , integer )
        TYPE_TO_STRING( real , real )
        TYPE_TO_STRING( string , string )
        TYPE_TO_STRING( vec2d , vec2d )
        TYPE_TO_STRING( vec3d , vec3d )
        TYPE_TO_STRING( vec4d , vec4d )
        TYPE_TO_STRING( rawbuffer , rawbuffer )
        TYPE_TO_STRING( integer_vector, integer[] )
        TYPE_TO_STRING( real_vector, real[] )
        TYPE_TO_STRING( string_vector, string[] )
        TYPE_TO_STRING( vec2d_vector, vec2d[] )
        TYPE_TO_STRING( vec3d_vector, vec3d[] )
        TYPE_TO_STRING( vec4d_vector, vec4d[] )
    }

    return sType;
}

#define TYPE_FROM_STRING( T , VAL) \
    if(sFieldType == #VAL) return type_##T

Variant::Type Variant::typeFromString(const std::string& sFieldType)
{
    TYPE_FROM_STRING(empty, empty);
    TYPE_FROM_STRING(integer, char);
    TYPE_FROM_STRING(integer, integer);
    TYPE_FROM_STRING(real, real);
    TYPE_FROM_STRING(string, string);
    TYPE_FROM_STRING(vec2d, vec2d);
    TYPE_FROM_STRING(vec3d, vec3d);
    TYPE_FROM_STRING(vec4d, vec4d);
    TYPE_FROM_STRING(rawbuffer, rawbuffer);
    TYPE_FROM_STRING(integer_vector, char[]);
    TYPE_FROM_STRING(integer_vector, integer[]);
    TYPE_FROM_STRING(real_vector, real[]);
    TYPE_FROM_STRING(string_vector, string[]);
    TYPE_FROM_STRING(vec2d_vector, vec2d[]);
    TYPE_FROM_STRING(vec3d_vector, vec3d[]);
    TYPE_FROM_STRING(vec4d_vector, vec4d[]);

    VMF_EXCEPTION(IncorrectParamException, std::string("Invalid type string: ") + sFieldType);
}

bool Variant::isConvertible(Type srcType, Type dstType)
{
    if (srcType == dstType)
        return true;

    // String cannot be converted to or from other types
    if (srcType == type_string || dstType == type_string)
        return false;

    // As long as the source is not float or double, we allow it to convert. Additional check on limits is required later.
    if (srcType != type_real && srcType != type_vec2d && srcType != type_vec3d && srcType != type_vec4d && srcType != type_rawbuffer)
        return true;

    // Deny any other forms of conversions.
    return false;
}

std::string Variant::base64encode(const vmf_rawbuffer& value)
{
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int size = (int)value.size();
    const char * data = value.data();

    while (size > 0)
    {
        const char *c0 = data++, *c1 = size > 1 ? data++ : "\0", *c2 = size > 2 ? data++ : "\0";

        result += base64_chars[(*c0 & 0xfc) >> 2];
        result += base64_chars[((*c0 & 0x03) << 4) | ((*c1 & 0xf0) >> 4)];
        result += size > 1 ? base64_chars[((*c1 & 0x0f) << 2) | ((*c2 & 0xc0) >> 6)] : '=';
        result += size > 2 ? base64_chars[*c2 & 0x3f] : '=';

        size -= 3;
    }
    return result;
}

vmf_rawbuffer Variant::base64decode(const std::string& base64Str)
{
    const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    if (base64Str.empty())
        return vmf_rawbuffer();

    size_t size = base64Str.size();
    if (base64Str.size() % 4 != 0)
        VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid base64 string size (isn't multiple of 4)");

    size_t i = 0;
    auto rit = base64Str.rbegin();
    while (rit != base64Str.rend() && *rit++ == '=') ++i;
    if (i > 2)
        VMF_EXCEPTION(vmf::IncorrectParamException, "Invalid base64 string: more than 2 trailing '=' symbols");

    size_t  res_size = size / 4 * 3 - i;
    std::unique_ptr<char[]> result(new char[res_size]);

    const char * in = base64Str.c_str();
    i = 0;
    while (i < res_size)
    {
        char c[4];
        for (int j = 0; j < 4; j++)
        {
            c[j] = (char)base64_chars.find(*in++);
            if (c[j] < 0 && i + j <= res_size)
                VMF_EXCEPTION(vmf::IncorrectParamException, "Input base64 string contains invalid symbol");
        }

        result[i++] = (c[0] << 2) | (c[1] >> 4);
        if (i < res_size) result[i++] = (c[1] << 4) | (c[2] >> 2);
        if (i < res_size) result[i++] = ((c[2] & 0x3) << 6) | c[3];
    }
    return vmf_rawbuffer(result.get(), res_size);
}

void Variant::release()
{
    delete data;
    data = nullptr;
};

}//vmf
