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
#include "vmf/statistics.hpp"
#include "vmf/metadata.hpp"
#include "vmf/metadatastream.hpp"
//#include <cstring>
//#include <string>
//#include <memory>
//#include <sstream>
//#include <cmath>
//#include <limits>
//#include <iomanip>

namespace vmf
{

// standard operation classes

#define STD_OP_PREFIX "com.intel.statistics.std_operation."

class MinOperation: public IStatisticsOperation
{
public:
    MinOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~MinOperation() {};

public:
    virtual std::string getName() const
        { return std::string( STD_OP_PREFIX "Min" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            default:
                VMF_EXCEPTION( IncorrectParamException, "Operation can't be applied to such data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & Add) != 0); };
    virtual void reset()
        {
            switch( m_dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_value = Variant(); /* empty value for the first time */
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            if( mode == Add )
            {
                switch( m_dataType )
                {
                case Variant::type_integer:
                    if( m_value.isEmpty() || (inputValue.get_integer() < m_value.get_integer()) )
                        m_value = inputValue;
                    break;
                case Variant::type_real:
                    if( m_value.isEmpty() || (inputValue.get_real() < m_value.get_real()) )
                        m_value = inputValue;
                    break;
                default:
                    VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
                }
            }
        };
    virtual Variant getValue() const
        { return m_value; };

private:
    Variant::Type m_dataType;
    Variant m_value;
};

class MaxOperation: public IStatisticsOperation
{
public:
    MaxOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~MaxOperation() {};

public:
    virtual std::string getName() const
        { return std::string( STD_OP_PREFIX "Max" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            default:
                VMF_EXCEPTION( IncorrectParamException, "Operation can't be applied to such data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & Add) != 0); };
    virtual void reset()
        {
            switch( m_dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_value = Variant(); /* empty value for the first time */
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            if( mode == Add )
            {
                switch( m_dataType )
                {
                case Variant::type_integer:
                    if( m_value.isEmpty() || (inputValue.get_integer() > m_value.get_integer()) )
                        m_value = inputValue;
                    break;
                case Variant::type_real:
                    if( m_value.isEmpty() || (inputValue.get_real() > m_value.get_real()) )
                        m_value = inputValue;
                    break;
                default:
                    VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
                }
            }
        };
    virtual Variant getValue() const
        { return m_value; };

private:
    Variant::Type m_dataType;
    Variant m_value;
};

class AverageOperation: public IStatisticsOperation
{
public:
    AverageOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~AverageOperation() {};

public:
    virtual std::string getName() const
        { return std::string(  STD_OP_PREFIX "Average" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            default:
                VMF_EXCEPTION( IncorrectParamException, "Operation can't be applied to such data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & (Add | Remove)) != 0); };
    virtual void reset()
        {
            switch( m_dataType )
            {
            case Variant::type_integer:
                m_value = Variant( (vmf_integer)0 );
                break;
            case Variant::type_real:
                m_value = Variant( (vmf_real)0 );
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
            m_count = 0;
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            switch( m_dataType )
            {
            case Variant::type_integer:
                switch( mode )
                {
                case Add:    ++m_count; m_value = Variant( m_value.get_integer() + inputValue.get_integer() ); break;
                case Remove: --m_count; m_value = Variant( m_value.get_integer() - inputValue.get_integer() ); break;
                default: /* not affected */ break;
                }
                break;
            case Variant::type_real:
                switch( mode )
                {
                case Add:    ++m_count; m_value = Variant( m_value.get_real() + inputValue.get_real() ); break;
                case Remove: --m_count; m_value = Variant( m_value.get_real() - inputValue.get_real() ); break;
                default: /* not affected */ break;
                }
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };
    virtual Variant getValue() const
        {
            switch( m_dataType )
            {
            case Variant::type_integer:
                return Variant( ((vmf_real) m_value.get_integer()) / m_count );
            case Variant::type_real:
                return Variant( ((vmf_real) m_value.get_real()) / m_count );
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };

private:
    Variant::Type m_dataType;
    Variant m_value;
    vmf_integer m_count;
};

class MedianOperation: public IStatisticsOperation
{
public:
    MedianOperation()
        : m_dataType( Variant::type_unknown )
        {
            VMF_EXCEPTION( NotImplementedException, "'Median' operation not implemented yet!" );
        };
    virtual ~MedianOperation() {};

public:
    virtual std::string getName() const
        { return std::string(  STD_OP_PREFIX "Median" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            default:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return false; };
    virtual void reset()
        {};
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {};
    virtual Variant getValue() const
        { return m_value; };

private:
    Variant::Type m_dataType;
    Variant m_value;
};

class CountOperation: public IStatisticsOperation
{
public:
    CountOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~CountOperation() {};

public:
    virtual std::string getName() const
        { return std::string( STD_OP_PREFIX "Count" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            default:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & (Add | Remove | Change)) != 0); };
    virtual void reset()
        {
            m_count = 0;
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            switch( mode )
            {
            case Add:    ++m_count; break;
            case Remove: --m_count; break;
            default: /* not affected */ break;
            }
        };
    virtual Variant getValue() const
        { return Variant( (vmf_integer)m_count ); };

private:
    Variant::Type m_dataType;
    vmf_integer m_count;
};

class SumOperation: public IStatisticsOperation
{
public:
    SumOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~SumOperation() {};

public:
    virtual std::string getName() const
        { return std::string( STD_OP_PREFIX "Sum" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            case Variant::type_integer:
            case Variant::type_real:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            default:
                VMF_EXCEPTION( IncorrectParamException, "Operation can't be applied to such data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & (Add | Remove)) != 0); };
    virtual void reset()
        {
            switch( m_dataType )
            {
            case Variant::type_integer:
                m_value = Variant( (vmf_integer)0 );
                break;
            case Variant::type_real:
                m_value = Variant( (vmf_real)0 );
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            switch( m_dataType )
            {
            case Variant::type_integer:
                switch( mode )
                {
                case Add:    m_value = Variant( m_value.get_integer() + inputValue.get_integer() ); break;
                case Remove: m_value = Variant( m_value.get_integer() - inputValue.get_integer() ); break;
                default: /* not affected */ break;
                }
                break;
            case Variant::type_real:
                switch( mode )
                {
                case Add:    m_value = Variant( m_value.get_real() + inputValue.get_real() ); break;
                case Remove: m_value = Variant( m_value.get_real() - inputValue.get_real() ); break;
                default: /* not affected */ break;
                }
                break;
            default:
                VMF_EXCEPTION( InternalErrorException, "Operation can't be applied to such data type!" );
            }
        };
    virtual Variant getValue() const
        { return m_value; };

private:
    Variant::Type m_dataType;
    Variant m_value;
};

class LastValueOperation: public IStatisticsOperation
{
public:
    LastValueOperation()
        : m_dataType( Variant::type_unknown ) {};
    virtual ~LastValueOperation() {};

public:
    virtual std::string getName() const
        { return std::string( STD_OP_PREFIX "LastValue" ); };
    virtual void setDataType( Variant::Type dataType )
        {
            switch( dataType )
            {
            default:
                m_dataType = dataType;
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            }
        };
    virtual bool canImmediate( Mode mode ) const
        { return bool((mode & (Add | Change)) != 0); };
    virtual void reset()
        {
            switch( m_dataType )
            {
            default:
                m_value = Variant(); /* empty value for the first time */
                break;
            case Variant::type_unknown:
                VMF_EXCEPTION( InternalErrorException, "Operation doesn't accept 'unknown' data type!" );
            }
        };
    virtual void handleValue( Mode mode, const Variant& inputValue )
        {
            if( inputValue.getType() != m_dataType )
            {
                VMF_EXCEPTION( IncorrectParamException, "Operation expects different data type!" );
            }
            switch( mode )
            {
            case Add: /* fall down */
            case Change: m_value = inputValue; break;
            default: /* not affected */ break; // i.e. field set to dirty state and need to be updated on rescan
            }
        };
    virtual Variant getValue() const
        { return m_value; };

private:
    Variant::Type m_dataType;
    Variant m_value;
};

// class IStatisticsOperation

IStatisticsOperation::IStatisticsOperation()
{
}

IStatisticsOperation::~IStatisticsOperation()
{
}

std::shared_ptr< IStatisticsOperation > IStatisticsOperation::create( Type operationType, Variant::Type dataType )
{
    std::shared_ptr< IStatisticsOperation > operation( nullptr );
    bool isStandard = true;
    switch( operationType )
    {
    case Min:
        operation = std::make_shared< MinOperation >();
        break;
    case Max:
        operation = std::make_shared< MaxOperation >();
        break;
    case Average:
        operation = std::make_shared< AverageOperation >();
        break;
    case Median:
        operation = std::make_shared< MedianOperation >();
        break;
    case Count:
        operation = std::make_shared< CountOperation >();
        break;
    case Sum:
        operation = std::make_shared< SumOperation >();
        break;
    case LastValue:
        operation = std::make_shared< LastValueOperation >();
        break;
    default:
        operation = MetadataStream::findUserOperation( operationType /*, dataType*/ );
        isStandard = false;
        break;
    }
    validate( operation, isStandard );
    operation->setDataType( dataType );
    operation->reset();
    return operation;
}

void IStatisticsOperation::validate( std::shared_ptr< IStatisticsOperation > operation, bool isStandard )
{
    if( operation == nullptr )
    {
        VMF_EXCEPTION( NullPointerException, "Description pointer is empty!" );
    }
    if( !operation->canImmediate(Add) )
    {
        if( isStandard )
        {
            VMF_EXCEPTION( InternalErrorException, "Standard operation doesn't provide an Add/Immediate capability!" );
        }
        else
        {
            VMF_EXCEPTION( NotImplementedException, "User operation doesn't provide an Add/Immediate capability!" );
        }
    }
}

// class StatisticsDesc

StatisticsDesc::StatisticsDesc(
        const std::string& statisticsName,
        std::shared_ptr< MetadataDesc > metadataDesc,
        const std::string& fieldName )
    : m_statisticsName( statisticsName )
    , m_metadataDesc( metadataDesc )
    , m_fieldName( fieldName )
{
}

StatisticsDesc::~StatisticsDesc()
{
}

const std::string& StatisticsDesc::getStatisticsName() const
{
    return m_statisticsName;
}

std::shared_ptr< MetadataDesc > StatisticsDesc::getMetadataDesc() const
{
    return m_metadataDesc;
}

const std::string& StatisticsDesc::getFieldName() const
{
    return m_fieldName;
}

// class StatisticsField

StatisticsField::StatisticsField(
        std::shared_ptr< StatisticsDesc > desc,
        std::shared_ptr< IStatisticsOperation > operation )
    : m_desc( desc )
    , m_operation( operation )
    , m_dirty( false )
{
}

StatisticsField::~StatisticsField()
{
}

std::shared_ptr< StatisticsDesc > StatisticsField::getDesc() const
{
    return m_desc;
}

std::shared_ptr< IStatisticsOperation > StatisticsField::getOperation() const
{
    return m_operation;
}

bool StatisticsField::isDirty() const
{
    return m_dirty;
}

void StatisticsField::setDirty( bool dirtyState )
{
    m_dirty = dirtyState;
}

Variant StatisticsField::getValue() const
{
    return m_operation->getValue();
}

// class Statistics

Statistics::Statistics()
    : m_fields( std::vector< std::shared_ptr< StatisticsField >>() )
    , m_pStream( nullptr )
    , m_schemaName( "" )
    , m_dirty( false )
{
}

Statistics::~Statistics()
{
    m_pStream = nullptr;
}

void Statistics::handleMetadata(
        IStatisticsOperation::Mode mode,
        const std::shared_ptr< Metadata > spMetadata )
{
    for( auto spStatisticsField : m_fields )
    {
        const std::shared_ptr< StatisticsDesc > spStatisticsDesc = spStatisticsField->getDesc();
        const std::shared_ptr< MetadataDesc > spMetadataDesc = spMetadata->getDesc();
        if( spStatisticsDesc->getMetadataDesc() == spMetadataDesc )
        {
            const std::string& fieldName = spStatisticsDesc->getFieldName();
            Metadata::iterator it = spMetadata->findField( fieldName );
            if( it != spMetadata->end() )
            {
                FieldDesc fd;
                if( spMetadataDesc->getFieldDesc( fd, fieldName ) == true )
                {
                    // TODO: we can/must add also fd checks - e.g. value type
                    const FieldValue& fv = *it;
                    std::shared_ptr< IStatisticsOperation > operation =
                            spStatisticsField->getOperation();
                    if( operation->canImmediate( mode ))
                    {
                        operation->handleValue( mode, fv );
                    }
                    else
                    {
                        spStatisticsField->setDirty( true );
                        setDirty( true );
                    }
                }
            }
        }
    }
}

void Statistics::handleMetadataField(
        IStatisticsOperation::Mode mode,
        const std::shared_ptr< Metadata > spMetadata,
        const std::string& fieldName )
{
    for( auto spStatisticsField : m_fields )
    {
        const std::shared_ptr< StatisticsDesc > spStatisticsDesc = spStatisticsField->getDesc();
        const std::shared_ptr< MetadataDesc > spMetadataDesc = spMetadata->getDesc();
        if( (spStatisticsDesc->getMetadataDesc() == spMetadataDesc) &&
            (spStatisticsDesc->getFieldName() == fieldName) )
        {
            Metadata::iterator it = spMetadata->findField( fieldName );
            if( it != spMetadata->end() )
            {
                FieldDesc fd;
                if( spMetadataDesc->getFieldDesc( fd, fieldName ) == true )
                {
                    // TODO: we can/must add also fd checks - e.g. value type
                    const FieldValue& fv = *it;
                    std::shared_ptr< IStatisticsOperation > operation =
                            spStatisticsField->getOperation();
                    if( operation->canImmediate( mode ))
                    {
                        operation->handleValue( mode, fv );
                    }
                    else
                    {
                        spStatisticsField->setDirty( true );
                        setDirty( true );
                    }
                }
            }
        }
    }
}

void Statistics::rescan()
{
    if( isDirty() )
    {
        for( auto spStatisticsField : m_fields )
        {
            if( spStatisticsField->isDirty() )
            {
                std::shared_ptr< IStatisticsOperation > operation =
                        spStatisticsField->getOperation();
                operation->reset();
            }
        }
        MetadataSet ms = m_pStream->getAll();
        for( auto spMetadata : ms )
        {
            std::shared_ptr< MetadataDesc > spMetadataDesc = spMetadata->getDesc();
            for( auto spStatisticsField : m_fields )
            {
                if( spStatisticsField->isDirty() )
                {
                    std::shared_ptr< StatisticsDesc > spStatisticsDesc = spStatisticsField->getDesc();
                    if( (spMetadataDesc->getSchemaName() == m_schemaName) &&
                        (spStatisticsDesc->getMetadataDesc() == spMetadataDesc) )
                    {
                        const std::string& fieldName = spStatisticsDesc->getFieldName();
                        Metadata::iterator it = spMetadata->findField( fieldName );
                        if( it != spMetadata->end() )
                        {
                            FieldDesc fd;
                            if( spMetadataDesc->getFieldDesc( fd, fieldName ) == true )
                            {
                                // TODO: we can/must add also fd checks - e.g. value type
                                const FieldValue& fv = *it;
                                std::shared_ptr< IStatisticsOperation > operation =
                                        spStatisticsField->getOperation();
                                /* assert( operation->canImmediate( IStatisticsOperation::Add ) == true ); */
                                operation->handleValue( IStatisticsOperation::Add, fv );
                            }
                        }
                    }
                }
            }
        }
        for( auto spStatisticsField : m_fields )
        {
            if( spStatisticsField->isDirty() )
            {
                spStatisticsField->setDirty( false );
            }
        }
        setDirty( false );
    }
}

void Statistics::reset()
{
    for( auto spStatisticsField : m_fields )
    {
        std::shared_ptr< IStatisticsOperation > operation =
                spStatisticsField->getOperation();
        operation->reset();
        spStatisticsField->setDirty( false );
    }
    setDirty( false );
}

bool Statistics::isDirty() const
{
    return m_dirty;
}

void Statistics::setDirty( bool dirtyState )
{
    m_dirty = dirtyState;
}

void Statistics::setSchemaName( std::string schemaName )
{
    m_schemaName = schemaName;
}

void Statistics::setMetadataStream( const MetadataStream* pStream )
{
    m_pStream = pStream;
}

void Statistics::addField(
        const std::string& statisticsName,
        const std::string& metadataName,
        const std::string& fieldName,
        unsigned operationType )
{
    if( findStatisticsField( statisticsName ) != nullptr )
    {
        VMF_EXCEPTION( IncorrectParamException, "Statistics field with same name already exists!" );
    }

    std::shared_ptr< MetadataSchema > spSchema = getSchema();

    std::shared_ptr< MetadataDesc > spMetadataDesc = spSchema->findMetadataDesc( metadataName );
    if( spMetadataDesc == nullptr )
    {
        VMF_EXCEPTION( IncorrectParamException, "Unknown metadata name!" );
    }

    FieldDesc fd;
    if( spMetadataDesc->getFieldDesc( fd, fieldName ) != true )
    {
        VMF_EXCEPTION( IncorrectParamException, "Unknown metadata field name!" );
    }

    std::shared_ptr< StatisticsDesc > spStatisticsDesc =
            std::make_shared< StatisticsDesc >( statisticsName, spMetadataDesc, fieldName );
    std::shared_ptr< IStatisticsOperation > spOperation =
            IStatisticsOperation::create( IStatisticsOperation::Type(operationType), fd.type );

    std::shared_ptr< StatisticsField > spStatisticsField =
            std::make_shared< StatisticsField >( spStatisticsDesc, spOperation );

    m_fields.push_back( spStatisticsField );
}

const std::shared_ptr< StatisticsField > Statistics::findStatisticsField( const std::string& statisticsName ) const
{
    auto it = std::find_if( m_fields.begin(), m_fields.end(), [&]( const std::shared_ptr< StatisticsField >& spField )->bool
    {
        return spField->getDesc()->getStatisticsName() == statisticsName;
    });

    if( it != m_fields.end() )
        return *it;

    return nullptr;
}

std::vector< std::shared_ptr< StatisticsField >> Statistics::getStatisticsFields() const
{
    return m_fields;
}

std::shared_ptr< MetadataSchema > Statistics::getSchema() const
{
    if( (m_pStream != nullptr) && !m_schemaName.empty() )
    {
        std::shared_ptr< MetadataSchema > spSchema = m_pStream->getSchema( m_schemaName );
        if( spSchema != nullptr )
        {
            return spSchema;
        }
    }
    VMF_EXCEPTION( IncorrectParamException, "Unknown schema for statistics object" );
}

} // namespace vmf
