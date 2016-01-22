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

#ifndef __VMF_STATISTICS_H__
#define __VMF_STATISTICS_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "variant.hpp"
#include "global.hpp"
#include <memory>
#include <string>

namespace vmf
{
class Metadata;
class MetadataDesc;
class MetadataSchema;
class MetadataStream;

/*!
* \class IStatisticsOperation
* \brief %IStatisticsOperation is an interface for all statistical operations.
* All operations MUST support immediate value handling in Add mode (canImmediate(Add)==true);
* it's requirement and should be checked at runtime (on configuration stage).
*/
class VMF_EXPORT IStatisticsOperation
{
public:
    IStatisticsOperation();
    virtual ~IStatisticsOperation();

public:
    enum Type { Min, Max, Average, Median, Count, Sum, LastValue, _UserBegin };
    enum Mode { Add=0x01, Remove=0x02, Change=0x04 };

public:
    static std::shared_ptr< IStatisticsOperation > create( Type operationType, Variant::Type dataType );
    static void validate( std::shared_ptr< IStatisticsOperation > operation, bool isStandard );

public:
    virtual std::string getName() const = 0;
    virtual void setDataType( Variant::Type dataType) = 0;
    virtual bool canImmediate( Mode mode ) const = 0;
    virtual void reset() = 0;
    virtual void handleValue( Mode mode, const Variant& inputValue ) = 0;
    virtual Variant getValue() const = 0;
};

/*!
* \class StatisticsDesc
* \brief Statistics field descriptor
*/
class VMF_EXPORT StatisticsDesc
{
public:
    StatisticsDesc(
            const std::string& statisticsName,
            std::shared_ptr< MetadataDesc > metadataDesc,
            const std::string& fieldName );
    virtual ~StatisticsDesc();

public:
    const std::string& getStatisticsName() const;
    std::shared_ptr< MetadataDesc > getMetadataDesc() const;
    const std::string& getFieldName() const;

private:
    std::string m_statisticsName;
    std::shared_ptr< MetadataDesc > m_metadataDesc;
    std::string m_fieldName;
};

/*!
* \class StatisticsField
* \brief Statistics field record
*/
class VMF_EXPORT StatisticsField
{
public:
    StatisticsField(
            std::shared_ptr< StatisticsDesc > desc,
            std::shared_ptr< IStatisticsOperation > operation );
    virtual ~StatisticsField();

public:
    std::shared_ptr< StatisticsDesc > getDesc() const;
    std::shared_ptr< IStatisticsOperation > getOperation() const;
    bool isDirty() const;
    void setDirty( bool dirtyState );
    Variant getValue() const;

private:
    std::shared_ptr< StatisticsDesc > m_desc;
    std::shared_ptr< IStatisticsOperation > m_operation;
    bool m_dirty;
};

/*!
* \class Statistics
* \brief Statistics object itself, one per schema
*/
class VMF_EXPORT Statistics
{
    friend class MetadataSchema;
    friend class MetadataStream;

public:
    Statistics();
    virtual ~Statistics();

    void handleMetadata(
            IStatisticsOperation::Mode mode,
            const std::shared_ptr< Metadata > spMetadata );
    void handleMetadataField(
            IStatisticsOperation::Mode mode,
            const std::shared_ptr< Metadata > spMetadata,
            const std::string& fieldName );
    void rescan();
    void reset();
    bool isDirty() const;
    void setDirty( bool dirtyState );

    void addField(
            const std::string& statisticsName,
            const std::string& metadataName,
            const std::string& fieldName,
            unsigned operationType );
    const std::shared_ptr< StatisticsField > findStatisticsField( const std::string& statisticsName ) const;
    std::vector< std::shared_ptr< StatisticsField >> getStatisticsFields() const;

protected:
    void setSchemaName( std::string schemaName );
    void setMetadataStream( const MetadataStream* pStream );

private:
    std::shared_ptr< MetadataSchema > getSchema() const;

    std::vector< std::shared_ptr< StatisticsField >> m_fields;
    const MetadataStream* m_pStream;
    std::string m_schemaName;
    bool m_dirty;
};

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_STATISTICS_H__ */
