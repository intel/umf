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
#include "vmf/metadataschema.hpp"

using namespace std;

static shared_ptr< vmf::MetadataSchema > _createAllInOneSchema(const string& schemaName, const string& author);

namespace vmf
{

/*static*/ string MetadataSchema::getStdSchemaName(StdSchemaKind kind)
{
    switch(kind)
    {
        case STD_DST: return "vmf://ns.intel.com/vmf/std-dst-schema-1.0";

        default: VMF_EXCEPTION(IncorrectParamException, "Unknown StdSchemaKind value");
    }
}

/*static*/ shared_ptr< MetadataSchema > MetadataSchema::getStdSchema(StdSchemaKind kind)
{
    switch(kind)
    {
        case STD_DST: return _createAllInOneSchema(getStdSchemaName(kind), "Intel Corporation");

        default: VMF_EXCEPTION(IncorrectParamException, "Unknown StdSchemaKind value");
    }
}

} // namespace vmf

//
// ALL_IN_ONE
//
static shared_ptr< vmf::MetadataSchema > _createAllInOneSchema(const string& schemaName, const string& author)
{
    auto schema = make_shared<vmf::MetadataSchema>(schemaName, author);

    VMF_METADATA_BEGIN("person");
        VMF_FIELD_STR( "name" );
        VMF_FIELD_STR( "id" );
        VMF_FIELD_INT_OPT( "age" );
        VMF_FIELD_STR_OPT("gender");
        VMF_FIELD_STR_OPT("phone");
        VMF_FIELD_STR_OPT("email");
        VMF_FIELD_STR_OPT("address");
        VMF_FIELD_STR_OPT("comment");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("object");
        VMF_FIELD_STR( "name" );
        VMF_FIELD_STR( "id" );
        VMF_FIELD_STR_OPT("category");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("event");
        VMF_FIELD_STR( "name" );
        VMF_FIELD_STR_OPT("id");
        VMF_FIELD_STR_OPT("description");
        VMF_FIELD_STR_OPT("location");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("moment");
        VMF_FIELD_STR("title");
        VMF_FIELD_STR_OPT("description");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("rect");
        VMF_FIELD_INT( "left" );
        VMF_FIELD_INT( "top" );
        VMF_FIELD_INT( "width" );
        VMF_FIELD_INT( "height" );
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("location");
        VMF_FIELD_REAL( "latitude" );
        VMF_FIELD_REAL( "longitude" );
        VMF_FIELD_REAL_OPT("altitude");
        VMF_FIELD_REAL_OPT("accuracy");
        VMF_FIELD_REAL_OPT("speed");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("accelerometer");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("magneticfield");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("gyroscope");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("light");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("pressure");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("proximity");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("gravity");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("acceleration");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("rotation");
        VMF_FIELD_REAL( "x" );
        VMF_FIELD_REAL( "y" );
        VMF_FIELD_REAL( "z" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("orientation");
        VMF_FIELD_REAL( "azimuth" );
        VMF_FIELD_REAL( "pitch" );
        VMF_FIELD_REAL( "roll" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("humidity");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("temperature");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("speed");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("heartrate");
        VMF_FIELD_REAL( "value" );
        VMF_FIELD_REAL_OPT("accuracy");
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("bloodpressure");
        VMF_FIELD_REAL( "systolic" );
        VMF_FIELD_REAL( "diastolic" );
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("anysensor");
        VMF_FIELD_STR( "type" );
        VMF_FIELD_REAL( "r0" );
        VMF_FIELD_REAL_OPT("r1");
        VMF_FIELD_REAL_OPT("r2");
        VMF_FIELD_REAL_OPT( "r3" );
        VMF_FIELD_REAL_OPT( "r4" );
        VMF_FIELD_REAL_OPT( "r5" );
        VMF_FIELD_REAL_OPT( "r6" );
        VMF_FIELD_REAL_OPT( "r7" );
        VMF_FIELD_REAL_OPT( "r8" );
        VMF_FIELD_REAL_OPT( "r9" );
        VMF_FIELD_REAL_OPT( "r10" );
        VMF_FIELD_REAL_OPT( "r11" );
        VMF_FIELD_REAL_OPT( "r12" );
        VMF_FIELD_REAL_OPT( "r13" );
        VMF_FIELD_REAL_OPT( "r14" );
        VMF_FIELD_REAL_OPT( "r15" );
    VMF_METADATA_END(schema);

    VMF_METADATA_BEGIN("any");
        VMF_FIELD_STR( "type" );
        VMF_FIELD_STR_OPT( "s0" );
        VMF_FIELD_STR_OPT( "s1" );
        VMF_FIELD_STR_OPT( "s2" );
        VMF_FIELD_STR_OPT( "s3" );
        VMF_FIELD_STR_OPT( "s4" );
        VMF_FIELD_STR_OPT( "s5" );
        VMF_FIELD_STR_OPT( "s6" );
        VMF_FIELD_STR_OPT( "s7" );
        VMF_FIELD_INT_OPT( "i0" );
        VMF_FIELD_INT_OPT( "i1" );
        VMF_FIELD_INT_OPT( "i2" );
        VMF_FIELD_INT_OPT( "i3" );
        VMF_FIELD_INT_OPT( "i4" );
        VMF_FIELD_INT_OPT( "i5" );
        VMF_FIELD_INT_OPT( "i6" );
        VMF_FIELD_INT_OPT( "i7" );
        VMF_FIELD_REAL_OPT( "r0" );
        VMF_FIELD_REAL_OPT( "r1" );
        VMF_FIELD_REAL_OPT( "r2" );
        VMF_FIELD_REAL_OPT( "r3" );
        VMF_FIELD_REAL_OPT( "r4" );
        VMF_FIELD_REAL_OPT( "r5" );
        VMF_FIELD_REAL_OPT( "r6" );
        VMF_FIELD_REAL_OPT( "r7" );
    VMF_METADATA_END(schema);

    return schema;
}
