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
/*!
 * \class FieldValue
 * \brief The class that keeps the data containing in the field
 * as well as its name and other atrributes
 */
class VMF_EXPORT FieldValue : public vmf::Variant
{
public:
    /*!
     * \brief Default constructor
     */
    FieldValue() : m_name(""), m_useEncryption(false), m_encryptedData("")
    {}

    /*!
     * \brief Copy constructor
     * \param other An instance to be copied
     */
    FieldValue( const FieldValue& other ) :
        Variant(other),
        m_name(other.m_name),
        m_useEncryption(other.m_useEncryption),
        m_encryptedData(other.m_encryptedData)
    {}

    /*!
     * \brief Move constructor
     * \param other An instance to be moved
     */
    FieldValue(FieldValue&& other) :
        Variant(std::forward<Variant>(other)),
        m_name(std::move(other.m_name)),
        m_useEncryption(other.m_useEncryption),
        m_encryptedData(std::move(other.m_encryptedData))
    {}

    /*!
     * \brief Virtual destructor
     */
    virtual ~FieldValue(void)
    {}

    /*!
     * \brief Constructor with some members
     * \param name Name of the field
     * \param variant Value of the field, Variant type
     * \param useEncryption Flag specifying whether to use separate encryption
     * for this field or not
     */
    FieldValue( const std::string& name, vmf::Variant variant, bool useEncryption = false)
        : vmf::Variant( variant )
        , m_name( name )
        , m_useEncryption(useEncryption)
        , m_encryptedData("")
    {}

    /*!
     * \brief Gets the name of the field
     * \return
     */
    const std::string& getName() const { return m_name; }

    /*!
     * \brief Gets the flag specifying whether to use separate encryption
     * for this field or not
     * \return
     */
    const bool getUseEncryption() const { return m_useEncryption; }

    /*!
     * \brief Sets the flag specifying whether to use separate encryption
     * for this field or not
     * \param useEncryption
     */
    void setUseEncryption(bool useEncryption) { m_useEncryption = useEncryption; }

    /*!
     * \brief Gets encrypted string representation of the value
     * (if the field value isn't decrypted yet, otherwise it should be empty)
     * \return
     */
    const std::string& getEncryptedData() const { return m_encryptedData; }

    /*!
     * \brief Sets encrypted string representation of the value
     * (needed on encryption stage, should be empty after successful saving or export)
     * \param encryptedData
     */
    void setEncryptedData(const std::string& encryptedData) { m_encryptedData = encryptedData; }

    FieldValue& operator = ( const FieldValue& other )
    {
        if (this != &other)
        {
            Variant::operator=(other);
            m_name = other.m_name;
            m_useEncryption = other.m_useEncryption;
            m_encryptedData = other.m_encryptedData;
        }
        return *this;
    }

    FieldValue& operator = (FieldValue&& other)
    {
        if (this != &other)
        {
            Variant::operator=(std::forward<Variant>(other));
            m_name = std::move(other.m_name);
            m_useEncryption = other.m_useEncryption;
            m_encryptedData = std::move(other.m_encryptedData);
        }
        return *this;
    }

    bool operator == ( const FieldValue& other ) const
    {
        return m_name == other.m_name &&
               m_useEncryption == other.m_useEncryption &&
               Variant::operator == ( other );
    }

private:
    /*!
     * \brief m_name Field name
     */
    vmf_string m_name;

    /*!
     * \brief m_useEncryption Flag specifying whether to use separate encryption
     * for this field or not
     */
    bool m_useEncryption;

    /*!
     * \brief m_encryptedData While the field value isn't decrypted yet,
     * this field contains the encrypted string representation of the value
     */
    vmf_string m_encryptedData;
};
}

#endif /* __VMF_FIELD_VALUE_H__ */
