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
#include "vmf/metadatareference.hpp"

namespace umf
{
Reference::Reference() : desc(NULL) {}

Reference::Reference(std::shared_ptr<ReferenceDesc>& desc, std::weak_ptr<Metadata>& md) : desc(desc), md(md){}

Reference::Reference(std::shared_ptr<ReferenceDesc>& desc, const std::shared_ptr<Metadata>& md) : desc(desc), md(md){}

Reference::~Reference(){}

void Reference::setReferenceMetadata(const std::shared_ptr<Metadata>& spMetadata) { md = spMetadata; }

std::weak_ptr<Metadata> Reference::getReferenceMetadata() const { return md; }

std::shared_ptr<ReferenceDesc> Reference::getReferenceDescription() const { return desc; }
} //namespace umf
