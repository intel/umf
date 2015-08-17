// Copyright (C) 2013, Intel Corporation, all rights reserved.

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

long long getTimestamp()
{
#ifdef _VMF_USE_STD_CHRONO
    return std::chrono::duration_cast<std::chrono::milliseconds> ( std::chrono::system_clock::now().time_since_epoch() ).count();
#else
    return std::time(NULL) * 1000LL;
#endif //_VMF_USE_STD_CHRONO
}

long long getTimestamp(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds)
{
    if(year < 1970)
        VMF_EXCEPTION(IncorrectParamException, "Invalid year param");
    if( month > 12 || month < 1 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid month param");
    if( day > 31 || day <= 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid day param");
    if( hours >= 24 || hours < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid hours param");
    if( minutes >= 60 || minutes < 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid minutes param");
    if( seconds >= 60 || seconds < 0 )
        VMF_EXCEPTION(IncorrectParamException, "Invalid seconds param");
    if( milliseconds >= 1000 || milliseconds < 0)
        VMF_EXCEPTION(IncorrectParamException, "Invalid milliseconds param");

    std::tm timeinfo;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hours;
    timeinfo.tm_min = minutes;
    timeinfo.tm_sec = seconds;

    std::time_t time = std::mktime(&timeinfo);

#ifdef _VMF_USE_STD_CHRONO
    return (std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::from_time_t(time).time_since_epoch())).count() + (long long)milliseconds;
#else
    return time * 1000LL + milliseconds;
#endif //_VMF_USE_STD_CHRONO

}

} // namespace vmf
