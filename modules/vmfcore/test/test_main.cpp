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
#include "test_precomp.hpp"

#include <iostream>

int main(int argc, char **argv) 
{
    std::cout << "VMF build info:\n" << vmf::getBuildInfo() << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);

    return RUN_ALL_TESTS();
}
