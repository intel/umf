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
* \file global.hpp
* \brief Global declarations of Video Metadata Framework
* \details The file contains global declarations, enumeration and initialization
* functions.
*/

#ifndef __VMF_GLOBAL_H__
#define __VMF_GLOBAL_H__

#include <string>
#include <sstream>
#include <memory>
#include <limits>
#include <functional>

#include "exceptions.hpp"
#include "types.hpp"

/*!
* \namespace vmf
* \brief Video metadata framework namespace
* \details This namespace contains all functionality related to
* Video metadata framework
*/

namespace vmf
{

template<typename T>
std::string to_string(T val)
{
#if defined ANDROID || (defined _MSC_VER && _MSC_VER < 1700)
    std::stringstream sstream;
    sstream << val;
    return sstream.str();
#else
    return std::to_string(val);
#endif
}

template<typename T>
T from_string(const std::string& str)
{
    T val;
    std::stringstream sstream(str);
    sstream >> val;
    if (sstream.fail() || !sstream.eof())
    {
        VMF_EXCEPTION(IncorrectParamException, "Unable to convert string '" + str +
                "' to " + typeid(T).name());
    }
    return val;
}

template<>
inline double from_string(const std::string& str) {
    const std::string STR_NAN = "NAN";
    const std::string STR_INF = "INF";
    std::string string_value(str);
    for (auto it = string_value.begin(); it != string_value.end(); ++it)
        *it = toupper(*it);

    bool unordinary_value = true;
    double val;

    if(string_value.find(STR_NAN) != std::string::npos)
        val = std::numeric_limits<double>::quiet_NaN();
    else if(string_value.find(STR_INF) != std::string::npos)
        val = std::numeric_limits<double>::infinity();
    else
        unordinary_value = false;

    if (unordinary_value)
    {
        if(string_value.find('-') != std::string::npos)
            return -val;
        else
            return val;
    }

    std::stringstream sstream(str);
    sstream >> val;
    if (sstream.fail() || !sstream.eof())
    {
        VMF_EXCEPTION(IncorrectParamException, "Unable to convert string '" + str +
                "' to long double");
    }
    return val;
}

/*!
* \brief returns VMF build info string
*/
VMF_EXPORT std::string getBuildInfo();

/*! 
* \brief returns current time in milliseconds since "clock's epoch" (midnight 1 Jan 1970 UTC)
*/
VMF_EXPORT long long getTimestamp();

/*! 
* \brief converts the specified data + time (in local timezone) to the 'timestamp'
* \param year >= 1970
* \param month in [1,12]
* \param day in [1,31]
* \param hours in [0,23]
* \param minutes in [0,59]
* \param seconds in [0,59]
* \param ms in [0,999]
* \throw an 'IncorrectParamException' in case of error.
* \return milliseconds since "clock's epoch"
*/
VMF_EXPORT long long getTimestamp(int year, int month, int day, int hours = 0, int minutes = 0, int seconds = 0, int ms = 0);

/*! 
* \typedef IdType
* \brief Object identifier type
*/
typedef int64_t IdType;

static const IdType INVALID_ID = -1;

template<typename P> struct select1st
{
    typename P::first_type const& operator()( P const& p ) const
    {
        return p.first;
    }
};

template<typename P> struct select2nd
{
    typename P::second_type const& operator()( P const& p ) const
    {
        return p.second;
    }
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4273)
#endif
/*!
* \brief Initialize Video Metadata Framework
* \details This function should be called before any call of VMF functions
*/
VMF_EXPORT extern void initialize();

/*!
* \brief Clear resources allocated by Video Metadata Framework
* \throw InternalErrorException throws if inetrnal error occured
* \throw DataStorageException throws if error occured if meta data provider
* \details This function should be called when program stop works with
* VMF.
*/
VMF_EXPORT extern void terminate();
#ifdef _MSC_VER
#pragma warning(pop)
#endif

} /* vmf */

#endif /* __VMF_GLOBAL_H__ */
