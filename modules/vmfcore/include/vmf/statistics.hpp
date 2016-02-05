/*.
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
#include "metadatadesc.hpp"
#include <map>
#include <memory>
#include <string>

namespace vmf
{
class Metadata;
class MetadataSchema;
class MetadataStream;

struct StatState { enum Type { UpToDate=1, NeedUpdate=2, NeedRescan=3 }; };
struct StatAction { enum Type { Add, Remove /*, Change*/ }; };

struct StatUpdateMode
{
    enum Type { Disabled, Manual, OnAdd, OnTimer };

    static std::string toString( StatUpdateMode::Type val );
    static StatUpdateMode::Type fromString( const std::string& str );
};

class Stat;
class StatField;

class VMF_EXPORT IStatOp
{
public:
    IStatOp() {}
    virtual ~IStatOp() {}

public:
    virtual const std::string& getName() const = 0;
    virtual void reset() = 0;
    virtual bool handle( StatAction::Type action, const Variant& inputValue ) = 0;
    virtual const Variant& getValue() const = 0;
};

class VMF_EXPORT StatOpFactory
{
public:
    typedef IStatOp* (*InstanceCreator)();
private:
    typedef std::pair< std::string, InstanceCreator > UserOpItem;
    typedef std::map< std::string, InstanceCreator > UserOpMap;

public:
    static IStatOp* create( const std::string& name );
    static void registerUserOp( const std::string& name, InstanceCreator createInstance );

    // required: UserOp::className & UserOp::createInstance members
    template< class UserOp >
    inline static void registerUserOp()
        {
            static_assert( std::is_base_of< IStatOp, UserOp >::value,
                           "User operation must implement IStatOp interface" );
            registerUserOp( UserOp::className, UserOp::createInstance );
        }

    static const char minName[];
    static const char maxName[];
    static const char averageName[];
    static const char countName[];
    static const char sumName[];
    static const char lastName[];

private:
    static UserOpMap& getClassMap();
};

class VMF_EXPORT StatField
{
    friend class Stat; // setStream()

private:
    class StatFieldDesc
    {
    public:
        StatFieldDesc( const std::string& name, const std::string& schemaName,
                       const std::string& metadataName, const std::string& fieldName,
                       const std::string& opName )
            : m_name( name ), m_schemaName( schemaName ), m_metadataName( metadataName ),
              m_metadataDesc( nullptr ), m_fieldName( fieldName ), m_fieldDesc(),
              m_opName( opName ), m_pMetadataStream( nullptr ) {}
        explicit StatFieldDesc( const StatFieldDesc& other )
            : m_name( other.m_name ), m_schemaName( other.m_schemaName ), m_metadataName( other.m_metadataName ),
              m_metadataDesc( nullptr ), m_fieldName( other.m_fieldName ), m_fieldDesc(),
              m_opName( other.m_opName ), m_pMetadataStream( nullptr ) {}
        StatFieldDesc()
            : m_name( "" ), m_schemaName( "" ), m_metadataName( "" ),
              m_metadataDesc( nullptr ), m_fieldName( "" ), m_fieldDesc(),
              m_opName( "" ), m_pMetadataStream( nullptr ) {}
        ~StatFieldDesc() {}

        StatFieldDesc& operator=( const StatFieldDesc& other );

        const std::string& getName() const { return m_name; }
        const std::string& getSchemaName() const { return m_schemaName; }
        const std::string& getMetadataName() const { return m_metadataName; }
        std::shared_ptr< MetadataDesc > getMetadataDesc() const { return m_metadataDesc; }
        const std::string& getFieldName() const { return m_fieldDesc.name; }
        const std::string& getOpName() const { return m_opName; }

    public:
        void setStream( MetadataStream* pMetadataStream );
        MetadataStream* getStream() const { return m_pMetadataStream; }

    private:
        std::string m_name;
        std::string m_schemaName;
        std::string m_metadataName;
        std::shared_ptr< MetadataDesc > m_metadataDesc;
        std::string m_fieldName;
        FieldDesc m_fieldDesc;
        std::string m_opName;
        MetadataStream* m_pMetadataStream;
    };

public:
    StatField( const std::string& name, const std::string& schemaName,
               const std::string& metadataName, const std::string& fieldName,
               const std::string& opName );
    explicit StatField( const StatField& other );
    StatField();
    ~StatField();

    StatField& operator=( const StatField& other );

    const std::string& getName() const { return m_desc.getName(); }
    std::shared_ptr< MetadataDesc > getMetadataDesc() const { return m_desc.getMetadataDesc(); }
    const std::string& getFieldName() const { return m_desc.getFieldName(); }
    const std::string& getOpName() const { return m_desc.getOpName(); }

    StatState::Type getState() const { return m_state; }

    void update( bool doRescan = false );
    StatState::Type handle( StatAction::Type action, std::shared_ptr< Metadata > metadata );

    const Variant& getValue() const { return m_op->getValue(); }

private:
    void setStream( MetadataStream* pMetadataStream );
    MetadataStream* getStream() const { return m_desc.getStream(); }

    bool isActive() const { return m_isActive; }
    void updateState( bool didUpdate );

    StatFieldDesc m_desc;
    IStatOp* m_op;
    StatState::Type m_state;
    bool m_isActive;
};

class VMF_EXPORT Stat
{
    friend class MetadataStream; // setStream()

private:
    class StatDesc
    {
    public:
        explicit StatDesc( const std::string& name ) : m_name( name ) {}
        ~StatDesc() {}

        const std::string& getName() const { return m_name; }

    private:
        std::string m_name;
    };

public:
    Stat( const std::string& name, const std::vector< StatField >& fields, StatUpdateMode::Type updateMode );
    ~Stat();

    const std::string& getName() const { return m_desc.getName(); }
    StatState::Type getState() const { return m_state; }

    StatUpdateMode::Type getUpdateMode() const { return m_updateMode; }
    void setUpdateMode( StatUpdateMode::Type updateMode );

    void update( bool doRescan = false );
    void notify( StatAction::Type action, std::shared_ptr< Metadata > metadata );

    std::vector< std::string > getAllFieldNames() const;
    const StatField& getField( const std::string& name ) const;
    const Variant& operator[]( const std::string& name ) const { return getField( name ).getValue(); }

private:
    void setStream( MetadataStream* pMetadataStream );

    bool isActive() const { return m_isActive; }
    void updateState( StatState::Type state );

    StatDesc m_desc;
    std::vector< StatField > m_fields;
    StatUpdateMode::Type m_updateMode;
    StatState::Type m_state;
    bool m_isActive;
};

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_STATISTICS_H__ */
    
