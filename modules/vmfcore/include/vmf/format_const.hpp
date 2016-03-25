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
* \file format_const.hpp
* \brief Constants definitions for %IReader and %IWriter interface implementations
* \details The file contains constants declarations that are required for %IReader and %IWriter interface implementations (e.g. %XMLReader)
*/

#pragma once

#ifndef __VMF_RWCONST_H__
#define __VMF_RWCONST_H__

#define TAG_VMF "vmf"
#define TAG_ATTRIBS_ARRAY "attribs"
#define TAG_STATS_ARRAY "stat-objects"
#define TAG_STAT_OBJ "stat"
#define TAG_VIDEO_SEGMENTS_ARRAY "video-segments"
#define TAG_VIDEO_SEGMENT "segment"
#define TAG_SCHEMAS_ARRAY "schemas-array"
#define TAG_SCHEMA "schema"
#define TAG_DESCRIPTION "description"
#define TAG_DESCRIPTIONS_ARRAY "descriptions-array"
#define TAG_FIELD "field"
#define TAG_FIELDS_ARRAY "fields-array"
#define TAG_METADATA_ARRAY "metadata-array"
#define TAG_METADATA "metadata"
#define TAG_METADATA_REFERENCES_ARRAY "references-array"
#define TAG_METADATA_REFERENCE "reference"

#define ATTR_NAME "name"
#define ATTR_VALUE "value"
#define ATTR_ID "id"

#define ATTR_SEGMENT_TITLE "title"
#define ATTR_SEGMENT_FPS "fps"
#define ATTR_SEGMENT_TIME "time"
#define ATTR_SEGMENT_DURATION "duration"
#define ATTR_SEGMENT_WIDTH "width"
#define ATTR_SEGMENT_HEIGHT "height"

#define ATTR_SCHEMA_AUTHOR "author"

#define ATTR_FIELD_TYPE "type"
#define ATTR_FIELD_OPTIONAL "optional"

#define ATTR_REFERENCE_UNIQUE "unique"
#define ATTR_REFERENCE_CUSTOM "custom"

#define ATTR_VMF_NEXTID "nextId"
#define ATTR_VMF_FILEPATH "filepath"
#define ATTR_VMF_CHECKSUM "checksum"
#define ATTR_METADATA_SCHEMA "schema"
#define ATTR_METADATA_DESCRIPTION "description"
#define ATTR_METADATA_FRAME_IDX "frame-index"
#define ATTR_METADATA_FRAME_IDX_HI "frame-index-hi"
#define ATTR_METADATA_FRAME_IDX_LO "frame-index-lo"
#define ATTR_METADATA_NFRAMES "nframes"
#define ATTR_METADATA_NFRAMES_HI "nframes-hi"
#define ATTR_METADATA_NFRAMES_LO "nframes-lo"
#define ATTR_METADATA_TIMESTAMP "timestamp"
#define ATTR_METADATA_TIMESTAMP_HI "timestamp-hi"
#define ATTR_METADATA_TIMESTAMP_LO "timestamp-lo"
#define ATTR_METADATA_DURATION "duration"
#define ATTR_METADATA_DURATION_HI "duration-hi"
#define ATTR_METADATA_DURATION_LO "duration-lo"

#define COMPRESSION_ALGO_PROP_NAME  "algo"
#define COMPRESSED_DATA_PROP_NAME   "data"
#define COMPRESSED_DATA_DESC_NAME   "compressed-metadata"
#define COMPRESSED_DATA_SCHEMA_NAME "com.intel.vmf.compressed-metadata"

#endif /* __VMF_RWCONST_H__ */
