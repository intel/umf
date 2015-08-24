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
* \file metadataset.hpp
* \brief %MetadataSet class header file
*/

#ifndef __VMF_METADATA_SET_H__
#define __VMF_METADATA_SET_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "global.hpp"
#include "metadata.hpp"
#include "iquery.hpp"
#include <functional>

namespace vmf
{
/*!
* \class MetadataSet
* \brief %MetadataSet is container of filtered metadata objects
*/
class VMF_EXPORT MetadataSet : public std::vector< std::shared_ptr< Metadata >>, public IQuery
{
public:
    /*!
    * \brief Default class constructor
    */
    MetadataSet(void);

    // Require C++ 11
    /*!
    * \brief Class constructor with move semantic
    * \param other [in] Other %MetadataSet object
    * \details All content of other metadata set object will be 
    * moved to constructed object
    */
    MetadataSet( MetadataSet&& other );

    /*!
    * \brief Class copy constructor
    * \param other [in] %MetadataSet object to copy
    */
    MetadataSet( const MetadataSet& other );

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataSet(void);

    // Require C++ 11
    /*!
    * \brief Assign operator
    * \param other [in] object to assign
    * \return assigned metadata set
    */
    MetadataSet& operator = ( MetadataSet&& other );

    // Assume C++ 11, IQuery interface, seal the interface at this level
    MetadataSet query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter ) const;
    MetadataSet queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter ) const;

    MetadataSet queryByFrameIndex( size_t index ) const;
    MetadataSet queryByTime( long long startTime, long long endTime ) const;
    MetadataSet queryBySchema( const std::string& sSchemaName ) const;
    MetadataSet queryByName( const std::string& sName ) const;

    MetadataSet queryByNameAndValue( const std::string& sMetadataName, const vmf::FieldValue& value ) const;
    MetadataSet queryByNameAndFields( const std::string& sMetadataName, const std::vector< vmf::FieldValue>& vFields ) const;

    MetadataSet queryByReference( const std::string& sReferenceName ) const;
    MetadataSet queryByReference( const std::string& sReferenceName, const vmf::FieldValue& value ) const;
    MetadataSet queryByReference( const std::string& sReferenceName, const std::vector< vmf::FieldValue>& vFields ) const;

    /*!
    * \brief Shift frame index value associated with metadata
    * \param nTarFrameIndex [in] The new frame index of the frame referenced by nSrcFrameIndex
    * \param nSrcFrameIndex [in] The frame index of the first video frame to be shifted
    * \param nNumOfFrames [in] Number of video frames that has been shifted. If this is FRAME_COUNT_ALL, metadata associated to all video 
    * frames since nSrcFrameIndex will be imported
    * \param pSetFailure [out] Set to receive metadata that failed to import, due to losing of associated frame. If this is null, no
    * information about failed metadata will be returned.
    * \details This routine should be called after trimming of a video sequence. The nTarFrameIndex, nSrcFrameIndex 
    * and nNumOfFrames parameters should reflect the way how the video was trimmed.
    */
    size_t shift( long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames = FRAME_COUNT_ALL, MetadataSet* pSetFailure = NULL );
};

};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_METADATA_SET_H__ */
