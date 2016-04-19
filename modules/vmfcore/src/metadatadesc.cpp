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
#include "vmf/metadatadesc.hpp"
#include <algorithm>

namespace vmf
{
MetadataDesc::MetadataDesc()
    : m_sSchemaName()
    , m_sMetadataName()
    , m_vFields()
    , m_useEncryption(false)

{
    m_vRefDesc.emplace_back(std::make_shared<ReferenceDesc>("", false));
}

MetadataDesc::MetadataDesc(const std::string& sMetadataName, const std::vector< FieldDesc >& vFields,
                           bool useEncryption)
    : m_sSchemaName()
    , m_sMetadataName(sMetadataName)
    , m_vFields(vFields)
    , m_useEncryption(useEncryption)
{
    m_vRefDesc.emplace_back(std::make_shared<ReferenceDesc>("", false));
    validate();
}

MetadataDesc::MetadataDesc(const std::string& sMetadataName, const std::vector< FieldDesc >& vFields,
                           const std::vector<std::shared_ptr<ReferenceDesc>>& vRefs, bool useEncryption)
    : m_sSchemaName()
    , m_sMetadataName( sMetadataName )
    , m_vFields( vFields )
    , m_vRefDesc( vRefs )
    , m_useEncryption(useEncryption)
{
    if (find_if(m_vRefDesc.begin(), m_vRefDesc.end(), [](const std::shared_ptr<ReferenceDesc>& r)->bool { return r->name.empty(); }) == m_vRefDesc.end())
        m_vRefDesc.emplace_back(std::make_shared<ReferenceDesc>());

    validate();
}

MetadataDesc::MetadataDesc( const std::string& sMetadataName, Variant::Type type )
    : m_sSchemaName()
    , m_sMetadataName( sMetadataName )
    , m_useEncryption(false)
{
    if (type == Variant::type_empty)
    {
        VMF_EXCEPTION(IncorrectParamException, "Invalid metadata descriprion type.");
    }

    m_vFields.emplace_back( FieldDesc( "", type ) );
    m_vRefDesc.emplace_back(std::make_shared<ReferenceDesc>("", false));
}

MetadataDesc::~MetadataDesc(void)
{
}

void MetadataDesc::validate()
{
    if( m_sMetadataName.empty() )
    {
        VMF_EXCEPTION(ValidateException, "Metadata name cannot be empty!" );
    }

    // Check duplicate field names
    std::vector<std::string> vFieldNames;
    std::for_each( m_vFields.begin(), m_vFields.end(), [&]( FieldDesc field )
    {
        vFieldNames.push_back( field.name );
    });

    std::sort( vFieldNames.begin(), vFieldNames.end() );
    vFieldNames.erase( std::unique( vFieldNames.begin(), vFieldNames.end() ), vFieldNames.end() );
    if( vFieldNames.size() < m_vFields.size() )
    {
        VMF_EXCEPTION(ValidateException, "A structure cannot have duplicate field names!" );
    }

    // Check single value case
    if (vFieldNames.size() > 1)
    {
        for (auto it = vFieldNames.begin(); it != vFieldNames.end(); it++)
        {
            if (it->empty())
                VMF_EXCEPTION(ValidateException, "Anonymous field name cannot be used for metadata that has multiple fields!");
        }
    }
}

std::string MetadataDesc::getMetadataName() const
{
    return m_sMetadataName;
}

std::string MetadataDesc::getSchemaName() const
{
    return m_sSchemaName;
}

bool MetadataDesc::getUseEncryption() const
{
    return m_useEncryption;
}

void MetadataDesc::setUseEncryption(bool useEncryption)
{
    m_useEncryption = useEncryption;
}

std::vector<FieldDesc> MetadataDesc::getFields() const
{
    return m_vFields;
}

bool MetadataDesc::getFieldDesc( FieldDesc& field, const std::string& sFieldName ) const
{
    bool bRet = false;

    if( sFieldName.empty() )
    {
        if( m_vFields.size() == 1 )
        {
            field = m_vFields[0];
            bRet = true;
        }
    }
    else
    {
        auto it = std::find_if( m_vFields.begin(), m_vFields.end(), [&sFieldName]( FieldDesc fieldDesc )->bool
        {
            return fieldDesc.name == sFieldName;
        });

        if( it != m_vFields.end() )
        {
            field = *it;
            bRet = true;
        }
    }

    return bRet;
}


FieldDesc& MetadataDesc::getFieldDesc(const std::string &sFieldName)
{
    if( sFieldName.empty() )
    {
        if( m_vFields.size() == 1 )
        {
            return m_vFields[0];
        }
    }
    else
    {
        auto it = std::find_if( m_vFields.begin(), m_vFields.end(), [&sFieldName]( FieldDesc fieldDesc )->bool
        {
            return fieldDesc.name == sFieldName;
        });

        if( it != m_vFields.end() )
        {
            return *it;
        }
    }

    VMF_EXCEPTION(IncorrectParamException, "No field description found: \"" + sFieldName + "\"");
}


const std::vector<std::shared_ptr<ReferenceDesc>>& MetadataDesc::getAllReferenceDescs() const
{
    return m_vRefDesc;
}

void MetadataDesc::declareCustomReference(const std::string& refName, bool isUnique)
{
    for (auto it = m_vRefDesc.begin(); it != m_vRefDesc.end(); it++)
    {
        if ((*it)->name == refName)
            VMF_EXCEPTION(ValidateException, "This reference name already exist!");
    }
    
    m_vRefDesc.emplace_back(std::make_shared<ReferenceDesc>(refName, isUnique, true));
    return;
}

std::shared_ptr<ReferenceDesc> MetadataDesc::getReferenceDesc(const std::string& refName) const
{
    for (auto it = m_vRefDesc.begin(); it != m_vRefDesc.end(); it++)
    {
        if ((*it)->name == refName)
            return (*it);
    }
    return NULL;
}

void MetadataDesc::setSchemaName( const std::string& sAppName )
{
    m_sSchemaName = sAppName;
}
};
