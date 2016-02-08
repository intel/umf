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

#define BUILTIN_OP_PREFIX "com.intel.statistics.builtin_operation."

#define BUILTIN_OP_NAME_MIN     BUILTIN_OP_PREFIX "min"
#define BUILTIN_OP_NAME_MAX     BUILTIN_OP_PREFIX "max"
#define BUILTIN_OP_NAME_AVERAGE BUILTIN_OP_PREFIX "average"
#define BUILTIN_OP_NAME_COUNT   BUILTIN_OP_PREFIX "count"
#define BUILTIN_OP_NAME_SUM     BUILTIN_OP_PREFIX "sum"
#define BUILTIN_OP_NAME_LAST    BUILTIN_OP_PREFIX "last"

namespace vmf
{

// struct StatUpdateMode

std::string StatUpdateMode::toString( StatUpdateMode::Type val )
{
    switch( val )
    {
    case StatUpdateMode::Disabled: return "StatUpdateMode::Disabled";
    case StatUpdateMode::Manual:   return "StatUpdateMode::Manual";
    case StatUpdateMode::OnAdd:    return "StatUpdateMode::OnAdd";
    case StatUpdateMode::OnTimer:  return "StatUpdateMode::OnTimer";
    }
    VMF_EXCEPTION( vmf::InternalErrorException, "Enum value is invalid upon conversion to string" );
}

StatUpdateMode::Type StatUpdateMode::fromString( const std::string& str )
{
    if( str == "StatUpdateMode::Disabled" ) return StatUpdateMode::Disabled;
    if( str == "StatUpdateMode::Manual"   ) return StatUpdateMode::Manual;
    if( str == "StatUpdateMode::OnAdd"    ) return StatUpdateMode::OnAdd;
    if( str == "StatUpdateMode::OnTimer"  ) return StatUpdateMode::OnTimer;
    VMF_EXCEPTION( vmf::InternalErrorException, "Enum value is invalid upon conversion from string" );
}

// class IStatOp: builtin operations

class StatOpMin: public IStatOp
{
public:
    StatOpMin()
        {}
    virtual ~StatOpMin()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_value = Variant(); }
    virtual bool handle( StatAction::Type action, const Variant& inputValue )
        {
            switch( action )
            {
            case StatAction::Add:
                if( m_value.isEmpty() )
                    m_value = inputValue;
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                else
                    switch( m_value.getType() )
                    {
                    case Variant::type_integer:
                        if( inputValue.get_integer() < m_value.get_integer() )
                            m_value = inputValue;
                        break;
                    case Variant::type_real:
                        if( inputValue.get_real() < m_value.get_real() )
                            m_value = inputValue;
                        break;
                    default:
                        VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                    }
                break;
            case StatAction::Remove:
                if( m_value.isEmpty() )
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                switch( m_value.getType() )
                {
                case Variant::type_integer:
                    return false;
                case Variant::type_real:
                    return false;
                default:
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                }
            break;
            }
            return true;
        }
    virtual const Variant& getValue() const
        { return m_value; }

private:
    Variant m_value;

public:
    static IStatOp* createInstance()
        { return new StatOpMin(); }
    static const std::string className;
};

class StatOpMax: public IStatOp
{
public:
    StatOpMax()
        {}
    virtual ~StatOpMax()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_value = Variant(); }
    virtual bool handle( StatAction::Type action, const Variant& inputValue )
        {
            switch( action )
            {
            case StatAction::Add:
                if( m_value.isEmpty() )
                    m_value = inputValue;
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                else
                    switch( m_value.getType() )
                    {
                    case Variant::type_integer:
                        if( inputValue.get_integer() > m_value.get_integer() )
                            m_value = inputValue;
                        break;
                    case Variant::type_real:
                        if( inputValue.get_real() > m_value.get_real() )
                            m_value = inputValue;
                        break;
                    default:
                        VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                    }
                break;
            case StatAction::Remove:
                if( m_value.isEmpty() )
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                switch( m_value.getType() )
                {
                case Variant::type_integer:
                    return false;
                case Variant::type_real:
                    return false;
                default:
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                }
                break;
            }
            return true;
        }
    virtual const Variant& getValue() const
        { return m_value; }

private:
    Variant m_value;

public:
    static IStatOp* createInstance()
        { return new StatOpMax(); }
    static const std::string className;
};

class StatOpAverage: public IStatOp
{
public:
    StatOpAverage()
        : m_count( 0 ) {}
    virtual ~StatOpAverage()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_count = 0; m_value = Variant(); }
    virtual bool handle( StatAction::Type action, const Variant& inputValue )
        {
            switch( action )
            {
            case StatAction::Add:
                if( m_value.isEmpty() )
                    { m_count = 1; m_value = inputValue; }
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                else
                    switch( m_value.getType() )
                    {
                    case Variant::type_integer:
                        ++m_count; m_value = Variant( m_value.get_integer() + inputValue.get_integer() );
                        break;
                    case Variant::type_real:
                        ++m_count; m_value = Variant( m_value.get_real() + inputValue.get_real() );
                        break;
                    default:
                        VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                    }
                break;
            case StatAction::Remove:
                if( m_value.isEmpty() )
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                switch( m_value.getType() )
                {
                case Variant::type_integer:
                    --m_count; m_value = Variant( m_value.get_integer() - inputValue.get_integer() );
                    break;
                case Variant::type_real:
                    --m_count; m_value = Variant( m_value.get_real() - inputValue.get_real() );
                    break;
                default:
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                }
                break;
            }
            return true;
        }
    virtual const Variant& getValue() const
        {
            switch( m_value.getType() )
            {
            case Variant::type_unknown: // isEmpty()
                break;
            case Variant::type_integer:
                m_temp = Variant( ((vmf_real) m_value.get_integer()) / m_count );
                return m_temp;
            case Variant::type_real:
                m_temp = Variant( ((vmf_real) m_value.get_real()) / m_count );
                return m_temp;
            default:
                VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
            }
            return m_value;
        }

private:
    Variant m_value;
    vmf_integer m_count;
    mutable Variant m_temp; // temp return value for getValue()

public:
    static IStatOp* createInstance()
        { return new StatOpAverage(); }
    static const std::string className;
};

class StatOpCount: public IStatOp
{
public:
    StatOpCount()
        : m_count( 0 ) {}
    virtual ~StatOpCount()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_count = 0; }
    virtual bool handle( StatAction::Type action, const Variant& /*inputValue*/ )
        {
            switch( action )
            {
            case StatAction::Add:
                ++m_count;
                break;
            case StatAction::Remove:
                if( !(m_count > 0) )
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                --m_count;
                break;
            }
            return true;
        }
    virtual const Variant& getValue() const
        { m_temp = Variant( (vmf_integer)m_count ); return m_temp; }

private:
    vmf_integer m_count;
    mutable Variant m_temp; // temp return value for getValue()

public:
    static IStatOp* createInstance()
        { return new StatOpCount(); }
    static const std::string className;
};

class StatOpSum: public IStatOp
{
public:
    StatOpSum()
        {}
    virtual ~StatOpSum()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_value = Variant(); }
    virtual bool handle( StatAction::Type action, const Variant& inputValue )
        {
            switch( action )
            {
            case StatAction::Add:
                if( m_value.isEmpty() )
                    { m_value = inputValue; }
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                else
                    switch( m_value.getType() )
                    {
                    case Variant::type_integer:
                        m_value = Variant( m_value.get_integer() + inputValue.get_integer() );
                        break;
                    case Variant::type_real:
                        m_value = Variant( m_value.get_real() + inputValue.get_real() );
                        break;
                    default:
                        VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                    }
                break;
            case StatAction::Remove:
                if( m_value.isEmpty() )
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                else if( m_value.getType() != inputValue.getType() )
                    VMF_EXCEPTION( vmf::TypeCastException, "Type mismatch" );
                switch( m_value.getType() )
                {
                case Variant::type_integer:
                    m_value = Variant( m_value.get_integer() - inputValue.get_integer() );
                    break;
                case Variant::type_real:
                    m_value = Variant( m_value.get_real() - inputValue.get_real() );
                    break;
                default:
                    VMF_EXCEPTION( vmf::NotImplementedException, "Operation not applicable to this data type" );
                }
                break;
            }
            return true;
        }
    virtual const Variant& getValue() const
        { return m_value; }

private:
    Variant m_value;

public:
    static IStatOp* createInstance()
        { return new StatOpSum(); }
    static const std::string className;
};

class StatOpLast: public IStatOp
{
public:
    StatOpLast()
        {}
    virtual ~StatOpLast()
        {}

public:
    virtual const std::string& getName() const
        { return className; }
    virtual void reset()
        { m_value = Variant(); }
    virtual bool handle( StatAction::Type action, const Variant& inputValue )
        {
            switch( action )
            {
            case StatAction::Add:
                m_value = inputValue;
                break;
            case StatAction::Remove:
                return false;
            }
            return true;
        }
    virtual const Variant& getValue() const
        { return m_value; }

private:
    Variant m_value;

public:
    static IStatOp* createInstance()
        { return new StatOpLast(); }
    static const std::string className;
};

const std::string StatOpMin    ::className = BUILTIN_OP_NAME_MIN;
const std::string StatOpMax    ::className = BUILTIN_OP_NAME_MAX;
const std::string StatOpAverage::className = BUILTIN_OP_NAME_AVERAGE;
const std::string StatOpCount  ::className = BUILTIN_OP_NAME_COUNT;
const std::string StatOpSum    ::className = BUILTIN_OP_NAME_SUM;
const std::string StatOpLast   ::className = BUILTIN_OP_NAME_LAST;

// class StatOpFactory

IStatOp* StatOpFactory::create( const std::string& name )
{
    // TODO: use lock for thread-safe access to UserOpMap instance
    const UserOpMap& ops = getClassMap();

    auto it = ops.find( name );
    if( it == ops.end() )
    {
        VMF_EXCEPTION( vmf::NotFoundException, "User operation not registered: '" + name + "'" );
    }

    IStatOp* op = (it->second)();
    if( op == nullptr )
    {
        VMF_EXCEPTION( vmf::NullPointerException, "User operation isn't created: '" + name + "'" );
    }

    return op;
}

void StatOpFactory::registerUserOp( const std::string& name, InstanceCreator createInstance )
{
    if( (name == "") || (createInstance == nullptr) )
    {
        VMF_EXCEPTION( vmf::IncorrectParamException, "Incorrect parameters upon registering user operation" );
    }

    // TODO: use lock for thread-safe access to UserOpMap instance
    UserOpMap& ops = getClassMap();

    auto it = ops.find( name );
    if( it != ops.end() )
    {
        if( it->second != createInstance )
        {
            VMF_EXCEPTION( vmf::IncorrectParamException, "User operation is registered twice: '" + name + "'" );
        }
        else
        {
            // Registered twice with the same createInstance() function; it's not an error
        }
    }
    else
    {
        ops.insert( UserOpItem( name, createInstance ));
    }
}

StatOpFactory::UserOpMap& StatOpFactory::getClassMap()
{
    // Singletone model based on initialisation of function-local static object
    // Object initialized once on first time we have entered the function
    static UserOpMap ops;

    if( ops.empty() )
    {
        ops.insert( UserOpItem( minName()    , StatOpMin::createInstance     ));
        ops.insert( UserOpItem( maxName()    , StatOpMax::createInstance     ));
        ops.insert( UserOpItem( averageName(), StatOpAverage::createInstance ));
        ops.insert( UserOpItem( countName()  , StatOpCount::createInstance   ));
        ops.insert( UserOpItem( sumName()    , StatOpSum::createInstance     ));
        ops.insert( UserOpItem( lastName()   , StatOpLast::createInstance    ));
    }

    return ops;
}

const std::string& StatOpFactory::minName()     { static const std::string name( BUILTIN_OP_NAME_MIN     ); return name; }
const std::string& StatOpFactory::maxName()     { static const std::string name( BUILTIN_OP_NAME_MAX     ); return name; }
const std::string& StatOpFactory::averageName() { static const std::string name( BUILTIN_OP_NAME_AVERAGE ); return name; }
const std::string& StatOpFactory::countName()   { static const std::string name( BUILTIN_OP_NAME_COUNT   ); return name; }
const std::string& StatOpFactory::sumName()     { static const std::string name( BUILTIN_OP_NAME_SUM     ); return name; }
const std::string& StatOpFactory::lastName()    { static const std::string name( BUILTIN_OP_NAME_LAST    ); return name; }

// class StatField (StatFieldDesc)

StatField::StatField(
        const std::string& name,
        const std::string& schemaName,
        const std::string& metadataName,
        const std::string& fieldName,
        const std::string& opName )
    : m_desc( name, schemaName, metadataName, fieldName, opName )
    , m_op( StatOpFactory::create( opName ))
    , m_state( StatState::UpToDate )
    , m_isActive( false )
{
}

StatField::StatField( const StatField& other )
    : m_desc( other.m_desc )
    , m_op( (other.m_op != nullptr) ? StatOpFactory::create( other.m_op->getName() ) : nullptr )
    , m_state( other.m_state )
    , m_isActive( other.m_isActive )
{
}

StatField::StatField()
    : m_desc()
    , m_op( nullptr )
    , m_state( StatState::UpToDate )
    , m_isActive( false )
{
}

StatField::~StatField()
{
    delete m_op; m_op = nullptr;
}

StatField::StatFieldDesc& StatField::StatFieldDesc::operator=( const StatField::StatFieldDesc& other )
{
    m_name         = other.m_name;
    m_schemaName   = other.m_schemaName;
    m_metadataName = other.m_metadataName;
    m_metadataDesc = nullptr;
    m_fieldName    = other.m_fieldName;
    m_fieldDesc    = FieldDesc();
    m_opName       = other.m_opName;

    setStream( nullptr );
    setStream( other.getStream() );

    return *this;
}

StatField& StatField::operator=( const StatField& other )
{
    m_desc = other.m_desc;

    delete m_op;
    m_op = (other.m_op != nullptr) ? StatOpFactory::create( other.m_op->getName() ) : nullptr;

    m_state = other.m_state;
    m_isActive = other.m_isActive;

    return *this;
}

StatState::Type StatField::handle( StatAction::Type action, std::shared_ptr< Metadata > metadata )
{
    const std::shared_ptr< MetadataDesc > metadataDesc = metadata->getDesc();
    if( metadataDesc && (this->getMetadataDesc() == metadataDesc ))
    {
        const std::string& fieldName = this->getFieldName();
        Metadata::iterator it = metadata->findField( fieldName );
        if( it != metadata->end() )
        {
            updateState( m_op->handle( action, *it ));
        }
    }
    return getState();
}

void StatField::update( bool doRescan )
{
    if( isActive() && (doRescan || (getState() != StatState::UpToDate)) )
    {
        m_op->reset();
        MetadataSet metadataSet = getStream()->getAll();
        for( auto metadata : metadataSet )
        {
            handle( StatAction::Add, metadata );
        }
        m_state = StatState::UpToDate;
    }
}

void StatField::updateState( bool didUpdate )
{
    if( !didUpdate )
    {
        m_state = StatState::NeedRescan;
    }
}

void StatField::StatFieldDesc::setStream( MetadataStream* pMetadataStream )
{
    m_pMetadataStream = pMetadataStream;
    if( m_pMetadataStream != nullptr )
    {
        std::shared_ptr< MetadataSchema > schema = m_pMetadataStream->getSchema( m_schemaName );
        if( schema == nullptr )
        {
            VMF_EXCEPTION( IncorrectParamException, "Unknown schema '" + m_schemaName + "'" );
        }

        m_metadataDesc = schema->findMetadataDesc( m_metadataName );
        if( m_metadataDesc == nullptr )
        {
            VMF_EXCEPTION( IncorrectParamException, "Unknown metadata '" + m_metadataName + "' for schema '" + m_schemaName + "'" );
        }

        if( m_metadataDesc->getFieldDesc( m_fieldDesc, m_fieldName ) != true )
        {
            VMF_EXCEPTION( IncorrectParamException, "Unknown field '" + m_fieldName + "' for metadata '" + m_metadataName + "' for schema '" + m_schemaName + "'" );
        }
    }
    else
    {
        m_metadataDesc = nullptr;
        m_fieldDesc = FieldDesc();
    }
}

void StatField::setStream( MetadataStream* pMetadataStream )
{
    m_desc.setStream( pMetadataStream );
    m_isActive = bool( pMetadataStream != nullptr );
}

// class Stat (StatDesc)

Stat::Stat( const std::string& name, const std::vector< StatField >& fields, StatUpdateMode::Type updateMode )
    : m_desc( name )
    , m_fields( fields )
    , m_updateMode( StatUpdateMode::Manual )
    , m_state( StatState::UpToDate )
    , m_isActive( false )
{
}

Stat::~Stat()
{
}

void Stat::notify( StatAction::Type action, std::shared_ptr< Metadata > metadata )
{
    if( isActive() && (m_updateMode != StatUpdateMode::Disabled) )
    {
        std::for_each( m_fields.begin(), m_fields.end(), [&]( StatField& statField )
        {
            updateState( statField.handle( action, metadata ));
        });
    }
}

void Stat::update( bool doRescan )
{
    if( isActive() && (m_updateMode != StatUpdateMode::Disabled) && (getState() != StatState::UpToDate) )
    {
        std::for_each( m_fields.begin(), m_fields.end(), [&]( StatField& statField )
        {
            statField.update( doRescan );
        });
        m_state = StatState::UpToDate;
    }
}

void Stat::setUpdateMode( StatUpdateMode::Type updateMode )
{
    if( updateMode != m_updateMode )
    {
        switch( updateMode )
        {
        case StatUpdateMode::OnAdd:
        case StatUpdateMode::OnTimer:
            VMF_LOG_WARNING( "Asynchronous update modes not implemented yet, StatUpdateMode::Manual used instead" );
            updateMode = StatUpdateMode::Manual;
            // fall down
        case StatUpdateMode::Disabled:
        case StatUpdateMode::Manual:
            m_updateMode = updateMode;
            break;
        default:
            VMF_EXCEPTION( vmf::NotImplementedException, "Unknown update mode" );
        }
    }
}

void Stat::updateState( StatState::Type state )
{
    switch( state )
    {
    case StatState::NeedRescan:
        if( unsigned(state) > unsigned(m_state) )
            m_state = state;
        break;
    case StatState::NeedUpdate:
        if( unsigned(state) > unsigned(m_state) )
            m_state = state;
        break;
    case StatState::UpToDate:
    default:
        break;
    }
}

std::vector< std::string > Stat::getAllFieldNames() const
{
    std::vector< std::string > names;

    std::for_each( m_fields.begin(), m_fields.end(), [&]( const StatField& statField )
    {
        names.push_back( statField.getName() );
    });

    return names;
}

const StatField& Stat::getField( const std::string& name ) const
{
    auto it = std::find_if( m_fields.begin(), m_fields.end(), [&]( const StatField& statField )->bool
    {
        return statField.getName() == name;
    });

    if( it == m_fields.end() )
    {
        VMF_EXCEPTION( vmf::NotFoundException, "Statistics field not found: '" + name + "'" );
    }

    return *it;
}

void Stat::setStream( MetadataStream* pMetadataStream )
{
    std::for_each( m_fields.begin(), m_fields.end(), [&]( StatField& statField )
    {
        statField.setStream( pMetadataStream );
    });
    m_isActive = bool( pMetadataStream != nullptr );
}

} // namespace vmf
