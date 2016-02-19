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

#ifndef VMF_COMPRESS_COMMON_HPP
#define VMF_COMPRESS_COMMON_HPP

#include "vmf/vmf.hpp"

void generateMetadata(vmf::MetadataStream& mdStream);
void readAndDumpMetadata(const vmf::vmf_string& videoFile);

int compress_builtin(const std::string& videoFile);
int compress_custom(const std::string& videoFile);

#endif //VMF_COMPRESS_COMMON_HPP
