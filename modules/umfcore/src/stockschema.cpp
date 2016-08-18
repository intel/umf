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
#include "umf/metadataschema.hpp"

using namespace std;

static shared_ptr< umf::MetadataSchema > _createAllInOneSchema(const string& schemaName, const string& author);

namespace umf
{

/*static*/ string MetadataSchema::getStdSchemaName(StdSchemaKind kind)
{
    switch(kind)
    {
        case STD_DST: return "umf://ns.intel.com/umf/std-dst-schema-1.0";

        default: UMF_EXCEPTION(IncorrectParamException, "Unknown StdSchemaKind value");
    }
}

/*static*/ shared_ptr< MetadataSchema > MetadataSchema::getStdSchema(StdSchemaKind kind)
{
    switch(kind)
    {
        case STD_DST: return _createAllInOneSchema(getStdSchemaName(kind), "Intel Corporation");

        default: UMF_EXCEPTION(IncorrectParamException, "Unknown StdSchemaKind value");
    }
}

} // namespace umf

//
// ALL_IN_ONE
//
static shared_ptr< umf::MetadataSchema > _createAllInOneSchema(const string& schemaName, const string& author)
{
    auto schema = make_shared<umf::MetadataSchema>(schemaName, author);

    UMF_METADATA_BEGIN("person");
        UMF_FIELD_STR( "name" );
        UMF_FIELD_STR( "id" );
        UMF_FIELD_INT_OPT( "age" );
        UMF_FIELD_STR_OPT("gender");
        UMF_FIELD_STR_OPT("phone");
        UMF_FIELD_STR_OPT("email");
        UMF_FIELD_STR_OPT("address");
        UMF_FIELD_STR_OPT("comment");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("object");
        UMF_FIELD_STR( "name" );
        UMF_FIELD_STR( "id" );
        UMF_FIELD_STR_OPT("category");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("event");
        UMF_FIELD_STR( "name" );
        UMF_FIELD_STR_OPT("id");
        UMF_FIELD_STR_OPT("description");
        UMF_FIELD_STR_OPT("location");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("moment");
        UMF_FIELD_STR("title");
        UMF_FIELD_STR_OPT("description");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("rect");
        UMF_FIELD_INT( "left" );
        UMF_FIELD_INT( "top" );
        UMF_FIELD_INT( "width" );
        UMF_FIELD_INT( "height" );
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("location");
        UMF_FIELD_REAL( "latitude" );
        UMF_FIELD_REAL( "longitude" );
        UMF_FIELD_REAL_OPT("altitude");
        UMF_FIELD_REAL_OPT("accuracy");
        UMF_FIELD_REAL_OPT("speed");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("accelerometer");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("magneticfield");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("gyroscope");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("light");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("pressure");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("proximity");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("gravity");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("acceleration");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("rotation");
        UMF_FIELD_REAL( "x" );
        UMF_FIELD_REAL( "y" );
        UMF_FIELD_REAL( "z" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("orientation");
        UMF_FIELD_REAL( "azimuth" );
        UMF_FIELD_REAL( "pitch" );
        UMF_FIELD_REAL( "roll" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("humidity");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("temperature");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("speed");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("heartrate");
        UMF_FIELD_REAL( "value" );
        UMF_FIELD_REAL_OPT("accuracy");
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("bloodpressure");
        UMF_FIELD_REAL( "systolic" );
        UMF_FIELD_REAL( "diastolic" );
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("anysensor");
        UMF_FIELD_STR( "type" );
        UMF_FIELD_REAL( "r0" );
        UMF_FIELD_REAL_OPT("r1");
        UMF_FIELD_REAL_OPT("r2");
        UMF_FIELD_REAL_OPT( "r3" );
        UMF_FIELD_REAL_OPT( "r4" );
        UMF_FIELD_REAL_OPT( "r5" );
        UMF_FIELD_REAL_OPT( "r6" );
        UMF_FIELD_REAL_OPT( "r7" );
        UMF_FIELD_REAL_OPT( "r8" );
        UMF_FIELD_REAL_OPT( "r9" );
        UMF_FIELD_REAL_OPT( "r10" );
        UMF_FIELD_REAL_OPT( "r11" );
        UMF_FIELD_REAL_OPT( "r12" );
        UMF_FIELD_REAL_OPT( "r13" );
        UMF_FIELD_REAL_OPT( "r14" );
        UMF_FIELD_REAL_OPT( "r15" );
    UMF_METADATA_END(schema);

    UMF_METADATA_BEGIN("any");
        UMF_FIELD_STR( "type" );
        UMF_FIELD_STR_OPT( "s0" );
        UMF_FIELD_STR_OPT( "s1" );
        UMF_FIELD_STR_OPT( "s2" );
        UMF_FIELD_STR_OPT( "s3" );
        UMF_FIELD_STR_OPT( "s4" );
        UMF_FIELD_STR_OPT( "s5" );
        UMF_FIELD_STR_OPT( "s6" );
        UMF_FIELD_STR_OPT( "s7" );
        UMF_FIELD_INT_OPT( "i0" );
        UMF_FIELD_INT_OPT( "i1" );
        UMF_FIELD_INT_OPT( "i2" );
        UMF_FIELD_INT_OPT( "i3" );
        UMF_FIELD_INT_OPT( "i4" );
        UMF_FIELD_INT_OPT( "i5" );
        UMF_FIELD_INT_OPT( "i6" );
        UMF_FIELD_INT_OPT( "i7" );
        UMF_FIELD_REAL_OPT( "r0" );
        UMF_FIELD_REAL_OPT( "r1" );
        UMF_FIELD_REAL_OPT( "r2" );
        UMF_FIELD_REAL_OPT( "r3" );
        UMF_FIELD_REAL_OPT( "r4" );
        UMF_FIELD_REAL_OPT( "r5" );
        UMF_FIELD_REAL_OPT( "r6" );
        UMF_FIELD_REAL_OPT( "r7" );
    UMF_METADATA_END(schema);

    return schema;
}
