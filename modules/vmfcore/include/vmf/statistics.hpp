/*.
 * Copyright 2016 Intel(r) Corporation
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
* \file statistics.hpp
* \brief Statistics header file.
* - Classes: %StatOpBase, %StatOpFactory, %StatField, %Stat.
* - Enums: %State::State::Type, %State::Action::Type, State::UpdateMode::Type;
*/

#ifndef VMF_STATISTICS_H
#define VMF_STATISTICS_H

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

#include <condition_variable>
#include <queue>
#include <thread>

namespace vmf
{
class Metadata;
class MetadataSchema;
class MetadataStream;


class Stat;
class StatField;

/*!
* \class StatOpBase
* \brief Base class (interface definition) for all statistics operations, builtin or user-defined
*/
class VMF_EXPORT StatOpBase
{
public:
    StatOpBase() {}
    virtual ~StatOpBase() {}

public:
    /*!
    * \brief Get name of statistics operation object
    * \return name string
    */
    virtual std::string name() const = 0;

    /*!
    * \brief Reset statistics operation object to its initial state ("zero" its value)
    */
    virtual void reset() = 0;

    /*!
    * \brief Handle addition of metadata field value
    * \param fieldValue [in] input field value to be added
    */
    virtual void handle( const Variant& fieldValue ) = 0;

    /*!
    * \brief Get current field value
    * \return value
    */
    virtual Variant value() const = 0;
};

/*!
* \class StatOpFactory
* \brief Statistics operations factory, for both builtin or user-defined operations
*/
class VMF_EXPORT StatOpFactory
{
public:
    /*!
    * \typedef InstanceCreator
    * \brief Pointer to function which must create an actual instance of operation object
    */
    typedef StatOpBase* (*InstanceCreator)();

public:
    /*!
    * \brief Create an instance of statistics operation object, by its name
    * \param name [in] input operation name string; builtins have predefined names (@ref builtinName),
    * user-defined must be registered prior to creation (@ref registerUserOp, @ref isRegistered)
    * \return pointer to newly created instance, cannot be nullptr
    * \throw NotFoundException if user-defined operation wasn't registered
    * \throw NullPointerException if instance creator for user-defined operation didn't create one (returned nullptr)
    */
    static StatOpBase* create( const std::string& name );

    /*!
    * \brief Tests if user-defined operation was registered (@ref registerUserOp)
    * \param name [in] input operation name string
    * \return true if registered, false otherwise
    */
    static bool isRegistered( const std::string& name );

    /*!
    * \brief Register user-defined operation (operation class) for further creation (@ref create)
    * \param createInstance [in] pointer to instance creator function (@ref InstanceCreator)
    * \throw NullPointerException if instance creator function wasn't supplied (nullptr),
    * or if non-nullptr instance creator actually didn't create one (returned nullptr)
    * \throw IncorrectParamException if this user-defined operation class already registered (@ref isRegistered)
    */
    static void registerUserOp( InstanceCreator createInstance );

    /*!
    * \struct BuiltinOp
    * \brief Holds builtin operations type enum (BuiltinOp::Type)
    */
    struct BuiltinOp
    {
        /*!
        * \enum Type (BuiltinOp::Type)
        * \brief Builtin operations type
        * \details
        * - BuiltinOp::Min: computes minimum value, applicable to Variant::type_integer and Variant::type_real fields,
        * result has the same type as input;
        * - BuiltinOp::Max: computes maximum value, applicable to Variant::type_integer and Variant::type_real fields,
        * result has the same type as input;
        * - BuiltinOp::Average: computes average value, applicable to Variant::type_integer and Variant::type_real fields,
        * result has always Variant::type_real type;
        * - BuiltinOp::Count: counts input field values, applicable to any types, result has Variant::type_integer type;
        * - BuiltinOp::Sum: computes sum value, applicable to Variant::type_integer and Variant::type_real fields,
        * result has the same type as input;
        * - BuiltinOp::Last: holds chronologically last value of input fields, result has the same type as input.
        */
        enum Type { Min, Max, Average, Count, Sum, Last };
    };

    /*!
    * \brief Get name string for builtin operations, by its type
    * \param opType [in] builtin operation type
    * \return name string
    * \throw IncorrectParamException if input value is outside of declared constant range
    * (expected no throws occurred with regular enum type usage)
    */
    static std::string builtinName( BuiltinOp::Type opType );

private:
    typedef std::pair< std::string, InstanceCreator > UserOpItem;
    typedef std::map< std::string, InstanceCreator > UserOpMap;
    static std::mutex& getLock();
    static UserOpMap& getClassMap();
    static std::string getUserOpName( InstanceCreator createInstance );
};

/*!
* \class StatField
* \brief Statistics field
*/
class VMF_EXPORT StatField
{
    friend class Stat; // setStream()

public:
    /*!
    * \brief Class constructor
    * \param name [in] statistics field name string
    * \param schemaName [in] metadata schema name string
    * \param metadataName [in] metadata name string
    * \param fieldName [in] metadata field name string
    * \param opName [in] statistics operation name, builtin or user-defined
    */
    StatField( const std::string& name, const std::string& schemaName,
               const std::string& metadataName, const std::string& fieldName,
               const std::string& opName );

    /*!
    * \brief Class copy constructor
    * \param other [in] source statistics field object to copy
    */
    StatField( const StatField& other );

    /*!
    * \brief Copy constructor with move semantics. This feature requires C++11 compatible compiler.
    * \param other [in] source statistics field object to move
    */
    StatField( StatField&& other );

    /*!
    * \brief Default class constructor
    */
    StatField();

    /*!
    * \brief Class destructor
    */
    ~StatField();

    /*!
    * \brief Assignment operator, copy semantics
    * \param other [in] source statistics field object to copy
    * \return reference to target object (*this)
    */
    StatField& operator=( const StatField& other );

    /*!
    * \brief Assignment operator, move semantics. This feature requires C++11 compatible compiler.
    * \param other [in] source statistics field object to move
    * \return reference to target object (*this)
    */
    StatField& operator=( StatField&& other );

    /*!
    * \brief Equality operator.
    * \param rhs [in] another StatField object reference to compare
    * \return true if fields are equal (use the same operation for the same metadata field) and false otherwise
    */
    bool operator==(const StatField& rhs) const;
    /*!
    * \brief Get name string for statistics field object
    * \return name string
    */
    std::string getName() const;

    /*!
    * \brief Get schema name string for statistics field object
    * \return schema name string
    */
    std::string getSchemaName() const;

    /*!
    * \brief Get metadata name string for statistics field object
    * \return metadata name string
    */
    std::string getMetadataName() const;


    /*!
    * \brief Get metadata field name string for statistics field object
    * \return metadata field name string
    */
    std::string getFieldName() const;

    /*!
    * \brief Get statistics operation name string for statistics field object
    * \return statistics operation field name string
    */
    std::string getOpName() const;

    /*!
    * \brief Get current field value for statistics field object
    * \return value
    */
    Variant getValue() const { return m_op->value(); }

private:
    void handle( std::shared_ptr< Metadata > metadata );
    void reset();

    class StatFieldDesc;
    std::unique_ptr<StatFieldDesc> m_desc;

    std::unique_ptr<StatOpBase> m_op;
};


/*!
* \class Stat
* \brief Statistics object
*/
class VMF_EXPORT Stat
{
    friend class MetadataStream; // setStream()
    friend class StatWorker;     // handle()

public:

    /*!
    * \struct State
    * \brief Holds statistics object states enum (State::Type)
    */
    struct State
    {
        /*!
        * \enum Type (State::Type)
        * \brief Statistics object states
        * \details
        * - State::UpToDate: all statistics fields owned by statistics object are up-to-date;
        * - State::NeedUpdate: any of fields need to be updated;
        * - State::NeedRescan: need full rescan of statistics over stream metadata.
        */
        enum Type { UpToDate = 1, NeedUpdate = 2, NeedRescan = 3 };
    };

    /*!
    * \struct Action
    * \brief Holds actions over statistics values enum (Action::Type)
    */
    struct Action
    {
        /*!
        * \enum Type (Action::Type)
        * \brief Actions over statistics values
        * \details
        * - Action::Add: need to handle addition of new metadata;
        * - Action::Remove: need to handle removal of metadata.
        */
        enum Type { Add = 1, Remove = 2 /*, Change*/ };
    };

    /*!
    * \struct UpdateMode
    * \brief Holds statistics object update modes enum (UpdateMode::Type)
    */
    struct UpdateMode
    {
        /*!
        * \enum Type (UpdateMode::Type)
        * \brief Statistics object update modes
        * \details
        * - UpdateMode::Disabled: statistics processing would be disabled for this statistics object;
        * - UpdateMode::Manual: all input metadata would be queued for processing, but user need to call update() manually;
        * - UpdateMode::OnAdd: metadata queued, processing would start automatically, immediately after queueing;
        * - UpdateMode::OnTimer: metadata queued, processing would start automatically, by timer.
        */
        enum Type { Disabled = 1, Manual = 2, OnAdd = 3, OnTimer = 4 };
    };

    /*!
    * \brief Class constructor
    * \param name [in] statistics object name string
    * \param fields [in] statistics fields (vector of)
    * \param updateMode [in] initial update mode for statistics object (@ref UpdateMode::Type)
    */
    Stat(const std::string& name, const std::vector< StatField >& fields, UpdateMode::Type updateMode = UpdateMode::Disabled);

    /*!
    * \brief Class copy constructor
    * \param other [in] source statistics object to copy
    */
    Stat( const Stat& other );

    /*!
    * \brief Copy constructor with move semantics. This feature requires C++11 compatible compiler.
    * \param other [in] source statistics object to move
    */
    Stat( Stat&& other );

    /*!
    * \brief Class destructor
    */
    ~Stat();

    /*!
    * \brief Assignment operator, copy semantics
    * \param other [in] source statistics object to copy
    * \return reference to target object (*this)
    */
    Stat& operator=( const Stat& other );

    /*!
    * \brief Assignment operator, move semantics. This feature requires C++11 compatible compiler.
    * \param other [in] source statistics object to move
    * \return reference to target object (*this)
    */
    Stat& operator=( Stat&& other );

    /*!
    * \brief Get name string for statistics object
    * \return name string
    */
    std::string getName() const;

    /*!
    * \brief Get current state for statistics object
    * \return current state (@ref State::Type)
    */
    State::Type getState() const;

    /*!
    * \brief Set update mode for statistics object
    * \param updateMode [in] update mode (@ref UpdateMode::Type)
    * \throw IncorrectParamException if input value is outside of declared constant range
    * (expected no throws occurred with regular enum type usage)
    */
    void setUpdateMode( UpdateMode::Type updateMode );

    /*!
    * \brief Get current update mode for statistics object
    * \return current update mode (@ref UpdateMode::Type)
    */
    UpdateMode::Type getUpdateMode() const { return m_updateMode; }

    /*!
    * \brief Set update timeout for statistics object
    * \param ms [in] update timeout value, milliseconds
    * \details Update timeout used only for UpdateMode::OnTimer update mode (@ref UpdateMode::Type)
    */
    void setUpdateTimeout( unsigned ms ) { m_updateTimeout = ms; }

    /*!
    * \brief Get current update timeout for statistics object
    * \return current update timeout value, milliseconds
    * \details Update timeout used only for UpdateMode::OnTimer update mode (@ref UpdateMode::Type)
    */
    unsigned getUpdateTimeout() const { return m_updateTimeout; }

    /*!
    * \brief Clear accumulated statistics
    */
    void clear();

    /*!
    * \brief Update statistics object
    * \param doWait [in] specifies whether update will be done in blocking or non-blocking mode;
    * function will wait for completion of scheduled update/rescan if true, and not wait otherwise
    */
    void update(bool doWait = false );

    /*!
    * \brief Notifies statistics object about metadata event
    * \param metadata [in] pointer to metadata to process
    * \param action [in] required action for the metadata (@ref Action::Type)
    * \details Usually there is no need to call this function by user - metadata stream will do it automatically.
    */
    void notify( std::shared_ptr< Metadata > metadata, Action::Type action = Action::Add);

    /*!
    * \brief Get names of all statistics fields for the statistics object
    * \return Statistics field names (vector of)
    */
    std::vector< std::string > getAllFieldNames() const;

    /*!
    * \brief Get statistics field by its name
    * \param name [in] statistics field name
    * \return Statistics field (reference to)
    * \throw NotFoundException if such statistics field not exist
    */
    const StatField& getField( const std::string& name ) const;

    /*!
    * \brief Get statistics field value by field name
    * \param name [in] statistics field name
    * \return value
    * \throw NotFoundException if such statistics field not exist
    */
    Variant operator[]( const std::string& name ) const { return getField( name ).getValue(); }

private:
    void handle( const std::shared_ptr< Metadata > metadata );

    class StatDesc;
    std::unique_ptr< StatDesc > m_desc;

    std::vector< StatField > m_fields;

    class StatWorker;
    std::unique_ptr< StatWorker > m_worker;

    UpdateMode::Type m_updateMode;
    unsigned m_updateTimeout;

    mutable std::mutex m_lock;
    bool m_needRescan;
};

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* VMF_STATISTICS_H */
    
