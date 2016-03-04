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
#include "vmf/global.hpp"
#include "vmf/vmf.hpp"
#include "object_factory.hpp"

#include <ctime>

#if (defined(__cplusplus) && __cplusplus > 199711L) || (defined(_MSC_VER) && _MSC_VER >= 1700)
#include <chrono>
#define _VMF_USE_STD_CHRONO
#else
#include <ctime>
#endif

namespace vmf
{
std::string getBuildInfo()
{
#include "build-info.inc"

    return vmf_build_info;
}

long long getTimestamp()
{
#ifdef _VMF_USE_STD_CHRONO
    return std::chrono::duration_cast<std::chrono::milliseconds> ( std::chrono::system_clock::now().time_since_epoch() ).count();
#else
    return (long long) std::time(NULL) * 1000LL;
#endif //_VMF_USE_STD_CHRONO
}

long long getTimestamp(int year, int month, int day, int hours, int minutes, int seconds, int ms)
{
    if(year < 1970)
        VMF_EXCEPTION(IncorrectParamException, "Invalid year: " + to_string(year));
    if( month > 12 || month < 1 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid month: " + to_string(month));
    if( day > 31 || day <= 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid day: " + to_string(day));
    if( hours >= 24 || hours < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid hours: " + to_string(hours));
    if( minutes >= 60 || minutes < 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid minutes: " + to_string(minutes));
    if( seconds >= 60 || seconds < 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid seconds: " + to_string(seconds));
    if( ms >= 1000 || ms < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid milliseconds : " + to_string(ms));

    std::tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hours;
    timeinfo.tm_min = minutes;
    timeinfo.tm_sec = seconds;

    std::time_t time = std::mktime(&timeinfo);
    if (time == -1)
    {
        char buff[128] = {0};
        strftime(buff, sizeof(buff), "%c", &timeinfo);
        VMF_EXCEPTION(InternalErrorException, std::string("Error converting date + time to a timestamp: ") + buff);
    }

    long long retVal;
#ifdef _VMF_USE_STD_CHRONO
    retVal = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(time).time_since_epoch())).count() + (long long)ms;
#else
    retVal = time * 1000LL + ms;
#endif //_VMF_USE_STD_CHRONO
    return retVal;
}

} // namespace vmf
