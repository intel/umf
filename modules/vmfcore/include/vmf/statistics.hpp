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

#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>

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

class VMF_EXPORT StatOpBase
{
public:
    StatOpBase() {}
    virtual ~StatOpBase() {}

public:
    virtual std::string name() const = 0;
    virtual void reset() = 0;
    virtual bool handle( StatAction::Type action, const Variant& fieldValue ) = 0;
    virtual Variant value() const = 0;
};

class VMF_EXPORT StatOpFactory
{
public:
    typedef StatOpBase* (*InstanceCreator)();

public:
    static StatOpBase* create( const std::string& name );
    static void registerUserOp( InstanceCreator createInstance );

    struct BuiltinOp { enum Type { Min, Max, Average, Count, Sum, Last }; };
    static std::string builtinName( BuiltinOp::Type opType );

private:
    typedef std::pair< std::string, InstanceCreator > UserOpItem;
    typedef std::map< std::string, InstanceCreator > UserOpMap;
    static UserOpMap& getClassMap();
};

class VMF_EXPORT StatField
{
    friend class Stat; // setStream()

public:
    StatField( const std::string& name, const std::string& schemaName,
               const std::string& metadataName, const std::string& fieldName,
               const std::string& opName );
    explicit StatField( const StatField& other );
    explicit StatField( StatField&& other );
    StatField();
    ~StatField();

    StatField& operator=( const StatField& other );
    StatField& operator=( StatField&& other );

    std::string getName() const;
    std::string getSchemaName() const;
    std::string getMetadataName() const;
    std::shared_ptr< MetadataDesc > getMetadataDesc() const;
    std::string getFieldName() const;
    FieldDesc getFieldDesc() const;
    std::string getOpName() const;

    StatState::Type getState() const { return m_state; }

    void update( bool doRescan = false );
    StatState::Type handle( std::shared_ptr< Metadata > metadata );

    Variant getValue() const { return m_op->value(); }

private:
    void setStream( MetadataStream* pMetadataStream );
    MetadataStream* getStream() const;

    bool isActive() const { return m_isActive; }
    void updateState( bool didUpdate );

    class StatFieldDesc;
    std::unique_ptr< StatFieldDesc > m_desc;

    StatOpBase* m_op;
    StatState::Type m_state;
    bool m_isActive;
};

class VMF_EXPORT Stat
{
    friend class MetadataStream; // setStream()
    friend class StatWorker;     // handle()

public:
    Stat( const std::string& name, const std::vector< StatField >& fields, StatUpdateMode::Type updateMode );
    explicit Stat( const Stat& other );
    explicit Stat( Stat&& other );
    ~Stat();

    Stat& operator=( const Stat& other );
    Stat& operator=( Stat&& other );

    std::string getName() const;
    StatState::Type getState() const;

    StatUpdateMode::Type getUpdateMode() const { return m_updateMode; }
    void setUpdateMode( StatUpdateMode::Type updateMode );

    void setUpdateTimeout( unsigned ms ) { m_updateTimeout = ms; }
    unsigned getUpdateTimeout() const { return m_updateTimeout; }

    void update( bool doRescan = false, bool doWait = false );
    void notify( StatAction::Type action, std::shared_ptr< Metadata > metadata );

    std::vector< std::string > getAllFieldNames() const;
    const StatField& getField( const std::string& name ) const;
    Variant operator[]( const std::string& name ) const { return getField( name ).getValue(); }

private:
    void setStream( MetadataStream* pMetadataStream );
    MetadataStream* getStream() const;

    bool isActive() const { return m_isActive; }

    void handle( const std::shared_ptr< Metadata > metadata );
    void rescan();
    void setState( StatState::Type state );
    void resetState() { setState( StatState::UpToDate ); }

    class StatDesc;
    std::unique_ptr< StatDesc > m_desc;

    std::vector< StatField > m_fields;

    class StatWorker;
    std::unique_ptr< StatWorker > m_worker;

    StatUpdateMode::Type m_updateMode;
    unsigned m_updateTimeout;
    bool m_isActive;

    mutable std::mutex m_lock;
    StatState::Type m_state;
};

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_STATISTICS_H__ */
    
