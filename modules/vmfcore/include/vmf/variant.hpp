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
/*!
* \file variant.hpp
* \brief %Variant class definition
* \details The file contains %Variant class definition.
*/

#ifndef __VMF_VARIANT_H__
#define __VMF_VARIANT_H__

#include "vmf/global.hpp"
#include <vector>

namespace vmf
{
    class IData; // helper class for Variant implementation

    /*!
    * \class Variant
    * \brief Variant is a simple C++ implementation of BASIC VARIANT type. The class is similar to
    * boost::variant. A Variant object can hold a value from any of the basic variable type.
    * Variant objects are used as the base class of FieldValue, which is the building block class
    * for all metadata items.
    */
    class VMF_EXPORT Variant
    {
    public:

        /*! \enum Type
        * \brief Types of variant values.
        */
        enum Type
        {
            type_unknown = 0,
            type_integer,
            type_real,
            type_string,
            type_vec2d,
            type_vec3d,
            type_vec4d,
            type_rawbuffer,
            type_integer_vector,
            type_real_vector,
            type_string_vector,
            type_vec2d_vector,
            type_vec3d_vector,
            type_vec4d_vector
        };

        /*!
        * \brief Constructor
        */
        Variant();

        /*!
        * \brief Copy constructor
        * \param [in] other Source Variant object
        */
        Variant(const Variant& other);

        /*!
        * \brief Copy constructor with move semantics. This feature requires C++11 compatible compiler.
        * \param [in] other Source Variant object
        */
        Variant(Variant&& other);

        /*!
        * \brief Destructor
        */
        ~Variant();

#define DECLARE_VMF_TYPE( T ) \
    Variant( const vmf_##T& v); \
    Variant& operator = ( const vmf_##T& v ); \
    const vmf_##T& get_##T() const; \
    operator const vmf_##T& () const;

        DECLARE_VMF_TYPE( integer )
        DECLARE_VMF_TYPE( real )
        DECLARE_VMF_TYPE( string)
        DECLARE_VMF_TYPE( vec2d )
        DECLARE_VMF_TYPE( vec3d )
        DECLARE_VMF_TYPE( vec4d )
        DECLARE_VMF_TYPE( rawbuffer )

#define DECLARE_VECTOR_VMF_TYPE( T ) \
    Variant( const std::vector<vmf_##T>& v); \
    Variant& operator = ( const std::vector<vmf_##T>& v ); \
    const std::vector<vmf_##T>& get_##T##_vector() const; \
    operator const std::vector<vmf_##T>& () const;

        DECLARE_VECTOR_VMF_TYPE( integer )
        DECLARE_VECTOR_VMF_TYPE( real )
        DECLARE_VECTOR_VMF_TYPE( string )
        DECLARE_VECTOR_VMF_TYPE( vec2d )
        DECLARE_VECTOR_VMF_TYPE( vec3d )
        DECLARE_VECTOR_VMF_TYPE( vec4d )

        /*!
        * \brief Constructor
        * \param [in] value The source value to be used to construct the object.
        */
        Variant(const int& value);
        Variant(const unsigned int& value);

        /*!
        * \brief Constructor
        * \param [in] value The source value to be used to construct the object.
        */
        Variant(const float& value);

        /*!
        * \brief Copy operator
        * \param [in] value The source value to be used to construct the object.
        */
        Variant& operator = (const int& value);
        Variant& operator = (const unsigned int& value);

        /*!
        * \brief Copy operator
        * \param [in] value The source value to be used to construct the object.
        */
        Variant& operator = (const float& value);

        /*!
        * \brief Constructor
        * \param [in] value The source value to be used to construct the object.
        */
        Variant(const std::vector<int>& value);

        /*!
        * \brief Constructor
        * \param [in] value The source value to be used to construct the object.
        */
        Variant(const std::vector<float>& value);

        /*!
        * \brief Copy operator
        * \param [in] value The source value to be used to construct the object.
        */
        Variant& operator = (const std::vector<int>& value);

        /*!
        * \brief Copy operator
        * \param [in] value The source value to be used to construct the object.
        */
        Variant& operator = (const std::vector<float>& value);

        /*!
        * \brief Constructor
        * \param [in] pszString The source string to be used to construct the object.
        */
        Variant(const char* pszString);

        /*!
        * \brief Copy operator
        * \param [in] pszString The source string to be used to construct the object.
        * \return The reference to this object.
        */
        Variant& operator = (const char* pszString);

        /*!
        * \brief Copy operator
        * \param [in] other The source object.
        * \return The reference to this object.
        */
        Variant& operator = (const Variant& other);

        /*!
        * \brief Copy operator with move semantics. Requires C++ 11 compatible compiler.
        * \param [in] other The source object.
        * \return The reference to this object.
        */
        Variant& operator = (Variant&& other);

        /*!
        * \brief Comparison operator
        * \param [in] other The object to compare against.
        * \return true if the input object contains the same value as this object.
        * \details This function can compare variant objects that have different types, but are convertible
        * from one to another.
        */
        bool operator == (const Variant& other) const;

        /*!
        * \brief Unequal operator
        * \param [in] other The object to compare against.
        * \return true if the input object contains different value as this object.
        */
        bool operator != (const Variant& other) const;

        /*!
        * \brief Return the string representation of the value.
        * \return The string description of the value.
        * \details The string returned from this routine does not contain information of the data type.
        */
        std::string toString() const;

        /*!
        * \brief Assign value by a string.
        * \param [in] eType The type of the value contained in the string.
        * \param [in] sValue The string that contains the value. This is typically the value returned from toString().
        * \details This function should be used with toString().
        */
        void fromString(Type eType, const std::string& sValue);

        /*!
        * \brief Return the type of the value stored.
        * \return The type of the internal value.
        */
        Type getType() const;

        /*!
        * \brief Check if a value is stored in the object.
        * \return 'true' if no value is tored in the object (i.e. its type is 'type_unknown').
        */
        bool isEmpty() const;

        /*!
        * \brief Get the string representation of the type of the value stored.
        * \return The description of the type.
        */
        std::string getTypeName() const;

        /*!
        * \brief Convert the value from one type to a new type.
        * \param [in] tarType The target type to convert to.
        * \details This function can only convert value from convertible type. If the type is not convertible,
        * the function throws exception. For convertible types, the function may also throw exceptions if the
        * the value is not within the range the new type can represent.
        */
        void convertTo(Type tarType);

        /*!
        * \brief Convert type value to string representation.
        * \param [in] t the type value.
        * \return The string representation of the type.
        */
        static std::string typeToString(Type t);

        /*!
        * \brief Convert string type's representation to Variant::Type.
        * \param [in] sFieldType the string representation of the type.
        * \return converted Variant::Type.
        */
        static Type typeFromString(const std::string& sFieldType);

        /*!
        * \brief Check if it is possible to convert a value from the source type to the target type.
        * \param [in] srcType The source type.
        * \param [in] tarType The target type.
        * \return true if it is possible to convert a value from source type to the target type.
        * \details String type is not convertible to other types, and other types are not convertible to
        * string type. Type_int8 is usually used for storing a character. It is not convertible to other
        * types. Type_double and type_float can only convert to a double or a floating numbers. All other
        * integer types may be converted to floating or double, or to other integer types.
        *
        * %isConvertible() does not guarantee a value with source type will be able to convert to
        * the target type with no issue. Function convertTo() may still throw exceptions if the value is
        * out of the range of what the new type can represent.
        */
        static bool isConvertible(Type srcType, Type tarType);

        /*!
        * \brief Get the minimum limit that can be represented in the input type.
        * \param [in] eType The type to be checked.
        * \return lower numeric limit of the type
        * \throw IncorrectParamException for non-numeric types
        */
        template<typename T> static T minLimit(Type eType)
        {
            T limit = (T)0;

            switch (eType)
            {
            default:
            case vmf::Variant::type_unknown:
            case vmf::Variant::type_string:
                VMF_EXCEPTION(IncorrectParamException, "Only numeric type has limit!");

            case vmf::Variant::type_integer:
                limit = (T)std::numeric_limits<vmf::vmf_integer>::lowest();
                break;
            case type_real:
                limit = (T)std::numeric_limits<vmf::vmf_real>::lowest();
                break;
            }

            return limit;
        }

        /*!
        * \brief Get the maximum limit that can be represented in the input type.
        * \param [in] eType The type to be checked.
        * \return upper numeric limit of the type
        * \throw IncorrectParamException for non-numeric types
        */
        template<typename T> static T maxLimit(Type eType)
        {
            T limit = (T) 0;

            switch( eType )
            {
            default:
            case vmf::Variant::type_unknown:
            case vmf::Variant::type_string:
                VMF_EXCEPTION(IncorrectParamException, "Only numeric type has limit!" );

            case vmf::Variant::type_integer:
                limit = (T)std::numeric_limits<vmf::vmf_integer>::max();
                break;
            case type_real:
                limit = (T)std::numeric_limits<vmf::vmf_real>::max();
                break;
            }

            return limit;
        }

        /*!
        * \brief Applies base64 encoding algo to input raw_buffer value
        * \return encoded raw_buffer value as std::string
        */
        static std::string base64encode(const vmf_rawbuffer& value);

        /*!
        * \brief Applies base64 decoding algo to input std::string
        * \return decoded raw_buffer value from input std::string
        */
        static vmf_rawbuffer base64decode(const std::string& base64Str);

    private:
        /*!
        * \brief Release internal memory.
        */
        void release();
        
        IData* data;
        Type m_type;
    };
};

#endif /* __VMF_VARIANT_H__ */
