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

class VMF_EXPORT IStatOp
{
public:
    IStatOp() {}
    virtual ~IStatOp() {}

public:
    virtual const std::string& name() const = 0;
    virtual void reset() = 0;
    virtual bool handle( StatAction::Type action, const Variant& inputValue ) = 0;
    virtual const Variant& value() const = 0;
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

    // required: UserOp::opName() & UserOp::createInstance() static members
    template< class UserOp >
    inline static void registerUserOp()
        {
            static_assert( std::is_base_of< IStatOp, UserOp >::value,
                           "User operation must implement IStatOp interface" );
            registerUserOp( UserOp::opName(), UserOp::createInstance );
        }

    static const std::string& minName();
    static const std::string& maxName();
    static const std::string& averageName();
    static const std::string& countName();
    static const std::string& sumName();
    static const std::string& lastName();

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
        explicit StatFieldDesc( StatFieldDesc&& other )
            : m_name( std::move( other.m_name )), m_schemaName( std::move( other.m_schemaName )),
              m_metadataName( std::move( other.m_metadataName )), m_metadataDesc( std::move( nullptr )),
              m_fieldName( std::move( other.m_fieldName )), m_fieldDesc( std::move( other.m_fieldDesc )),
              m_opName( std::move( other.m_opName )), m_pMetadataStream( nullptr ) {}
        StatFieldDesc()
            : m_name( "" ), m_schemaName( "" ), m_metadataName( "" ),
              m_metadataDesc( nullptr ), m_fieldName( "" ), m_fieldDesc(),
              m_opName( "" ), m_pMetadataStream( nullptr ) {}
        ~StatFieldDesc() {}

        StatFieldDesc& operator=( const StatFieldDesc& other );
        StatFieldDesc& operator=( StatFieldDesc&& other );

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
    explicit StatField( StatField&& other );
    StatField();
    ~StatField();

    StatField& operator=( const StatField& other );
    StatField& operator=( StatField&& other );

    const std::string& getName() const { return m_desc.getName(); }
    std::shared_ptr< MetadataDesc > getMetadataDesc() const { return m_desc.getMetadataDesc(); }
    const std::string& getFieldName() const { return m_desc.getFieldName(); }
    const std::string& getOpName() const { return m_desc.getOpName(); }

    StatState::Type getState() const { return m_state; }

    void update( bool doRescan = false );
    StatState::Type handle( std::shared_ptr< Metadata > metadata );

    const Variant& getValue() const { return m_op->value(); }

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
    friend class StatWorker;     // handle()

private:
    class StatDesc
    {
    public:
        explicit StatDesc( const std::string& name ) : m_name( name ) {}
        explicit StatDesc( const StatDesc& other ) : m_name( other.m_name ) {}
        explicit StatDesc( StatDesc&& other ) : m_name( std::move( other.m_name )) {}
        ~StatDesc() {}

        StatDesc& operator=( const StatDesc& other ) { m_name = other.m_name; return *this; }
        StatDesc& operator=( StatDesc&& other ) { m_name = std::move( other.m_name ); return *this; }

        const std::string& getName() const { return m_name; }

    private:
        std::string m_name;
    };

private:
    class StatWorker
    {
    public:
        explicit StatWorker( Stat* stat )
            : m_stat( stat )
            , m_worker( &StatWorker::operator(), this )
            , m_wakeupForced( false )
            , m_updateScheduled( false )
            , m_rescanScheduled( false )
            , m_exitScheduled( false )
            , m_exitImmediate( false )
            {}
        ~StatWorker()
            {
                scheduleExit();
                m_worker.join();
                m_stat = nullptr;
            }
        void operator()()
            {
                // worker is starting
                for(;;)
                {
                    // worker is going to sleep
                    {
                        std::unique_lock< std::mutex > lock( m_lock );
                        if( m_stat->getUpdateMode() == StatUpdateMode::OnTimer )
                        {
                            const unsigned tmo = std::max( m_stat->getUpdateTimeout(), (unsigned)10 );
                            bool awaken = false;
                            do {
                                awaken = m_signal.wait_for( lock, std::chrono::milliseconds(tmo), [&]
                                {
                                    return m_exitScheduled ||
                                            m_rescanScheduled ||
                                            m_wakeupForced ||
                                            (m_updateScheduled && !m_items.empty());
                                });
                            } while( !awaken );
                        }
                        else
                        {
                            m_signal.wait( lock, [&]
                            {
                                return m_exitScheduled ||
                                        m_rescanScheduled ||
                                        m_wakeupForced ||
                                        (m_updateScheduled && !m_items.empty());
                            });
                        }
                        m_wakeupForced = false;
                        if( m_exitScheduled && m_exitImmediate )
                            break;
                    }
                    // worker has awaken
                    if( m_rescanScheduled )
                    {
                        // rescan
                        if( m_stat != nullptr )
                            m_stat->rescan();
                        {
                            std::unique_lock< std::mutex > lock( m_lock );
                            m_rescanScheduled = false;
                        }
                        if( m_stat != nullptr )
                            m_stat->resetState();
                    }
                    else if( m_updateScheduled )
                    {
                        std::shared_ptr< Metadata > metadata;
                        while( tryPop( metadata ))
                        {
                            // processing of item
                            if( m_stat != nullptr )
                                m_stat->handle( metadata );
                        }
                        {
                            std::unique_lock< std::mutex > lock( m_lock );
                            m_updateScheduled = false;
                        }
                        if( m_stat != nullptr )
                            m_stat->resetState();
                    }
                    {
                        std::unique_lock< std::mutex > lock( m_lock );
                        if( m_exitScheduled && !m_exitImmediate )
                            break;
                    }
                }
                // worker is finishing
            }
        void scheduleUpdate( const std::shared_ptr< Metadata > val, bool doWake = true )
            {
                std::unique_lock< std::mutex > lock( m_lock );
                m_items.push( val );
                if( doWake && !m_updateScheduled && !m_items.empty() )
                {
                    m_updateScheduled = true;
                    m_signal.notify_one();
                }
            }
        void scheduleRescan( bool doWake = true )
            {
                std::unique_lock< std::mutex > lock( m_lock );
                if( doWake && !m_rescanScheduled )
                {
                    m_rescanScheduled = true;
                    m_signal.notify_one();
                }
            }

        void scheduleExit( bool doImmediate = false )
            {
                std::unique_lock< std::mutex > lock( m_lock );
                if( !m_exitScheduled )
                {
                    m_exitScheduled = true;
                    m_exitImmediate = doImmediate;
                    m_signal.notify_one();
                }
            }
        void wakeup( bool doForceWakeup = false )
            {
                std::unique_lock< std::mutex > lock( m_lock );
                m_updateScheduled = (m_updateScheduled || !m_items.empty());
                m_wakeupForced = doForceWakeup;
                if( m_exitScheduled || m_updateScheduled | m_wakeupForced )
                {
                    m_signal.notify_one();
                }
            }
        void reset()
            {
                std::unique_lock< std::mutex > lock( m_lock );
                if( !m_items.empty() )
                    std::queue< std::shared_ptr< Metadata >>().swap( m_items );
                m_updateScheduled = false;
                m_rescanScheduled = false;
                m_exitScheduled = false;
                m_exitImmediate = false;
            }

    private:
        bool tryPop( std::shared_ptr< Metadata >& metadata )
            {
                std::unique_lock< std::mutex > lock( m_lock );
                if( !m_items.empty() ) {
                    metadata = m_items.front();
                    m_items.pop();
                    return true;
                }
                m_updateScheduled = false;
                return false;
            }

    private:
        Stat* m_stat;
        std::thread m_worker;
        std::queue< std::shared_ptr< Metadata >> m_items;
        std::atomic< bool > m_wakeupForced;
        std::atomic< bool > m_updateScheduled;
        std::atomic< bool > m_rescanScheduled;
        std::atomic< bool > m_exitScheduled;
        std::atomic< bool > m_exitImmediate;
        std::condition_variable m_signal;
        std::mutex m_lock;
    };

public:
    Stat( const std::string& name, const std::vector< StatField >& fields, StatUpdateMode::Type updateMode );
    explicit Stat( const Stat& other );
    explicit Stat( Stat&& other );
    ~Stat();

    Stat& operator=( const Stat& other );
    Stat& operator=( Stat&& other );

    const std::string& getName() const { return m_desc.getName(); }
    StatState::Type getState() const { return m_state; }

    StatUpdateMode::Type getUpdateMode() const { return m_updateMode; }
    void setUpdateMode( StatUpdateMode::Type updateMode );

    void setUpdateTimeout( unsigned ms ) { m_updateTimeout = ms; }
    unsigned getUpdateTimeout() const { return m_updateTimeout; }

    void update( bool doRescan = false );
    void notify( StatAction::Type action, std::shared_ptr< Metadata > metadata );

    std::vector< std::string > getAllFieldNames() const;
    const StatField& getField( const std::string& name ) const;
    const Variant& operator[]( const std::string& name ) const { return getField( name ).getValue(); }

private:
    void setStream( MetadataStream* pMetadataStream );
    MetadataStream* getStream() const;

    bool isActive() const { return m_isActive; }

    void handle( const std::shared_ptr< Metadata > metadata );
    void rescan();
    void resetState() { m_state = StatState::UpToDate; }

    StatDesc m_desc;
    std::vector< StatField > m_fields;
    StatWorker m_worker;
    StatUpdateMode::Type m_updateMode;
    unsigned m_updateTimeout;
    StatState::Type m_state;
    bool m_isActive;
};

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_STATISTICS_H__ */
    
