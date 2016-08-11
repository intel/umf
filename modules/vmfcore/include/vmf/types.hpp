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

#ifndef __VMF_TYPES_H__
#define __VMF_TYPES_H__

/*!
* \file types.hpp
* \brief Header file contains declarations of VMF numeric types
*/

#include <cstdint>
#include <cmath>
#include <memory>
#include <cstring>
#include <utility>
#include <vector>
#include <limits>

namespace vmf
{
    /*!
    * \brief Compares double values
    */
    static bool DOUBLE_EQ(double a, double b)
    {
        return std::abs(a - b) < std::numeric_limits<double>::epsilon();
    }

    /*!
    * \brief Character type. Equivalent of char type in C/C++
    */
    typedef char umf_char;

    /*!
    * \brief 64-bit integer type.
    */
    typedef int64_t umf_integer;

    /*!
    * \brief Double precesion floating point type.
    */
    typedef double umf_real;

    /*!
    * \brief String type.
    */
    typedef std::string umf_string;

    /*!
    * \brief floating point vector2D type.
    */
    struct umf_vec2d
    {
	umf_vec2d() : x(0), y(0) {}
	umf_vec2d(umf_real _x, umf_real _y) : x(_x), y(_y) {}
	bool operator == (const umf_vec2d& value) const
	{
	    return DOUBLE_EQ(x, value.x) && DOUBLE_EQ(y, value.y);
	}
	umf_real x;
	umf_real y;
    };

    /*!
    * \brief floating point vector3D type.
    */
    struct umf_vec3d : umf_vec2d
    {
	umf_vec3d() : umf_vec2d(), z(0){}
	umf_vec3d(umf_real _x, umf_real _y, umf_real _z) : umf_vec2d(_x, _y), z(_z) {}
	bool operator == (const umf_vec3d& value) const
	{
	    return umf_vec2d::operator==(value) && DOUBLE_EQ(z, value.z);
	}
	umf_real z;
    };

    /*!
    * \brief floating point vector4D type.
    */
    struct umf_vec4d : umf_vec3d
    {
	umf_vec4d() : umf_vec3d(), w(0){}
	umf_vec4d(umf_real _x, umf_real _y, umf_real _z, umf_real _w) : umf_vec3d(_x, _y, _z), w(_w) {}
	bool operator == (const umf_vec4d& value) const
	{
	    return umf_vec3d::operator==(value) && DOUBLE_EQ(w, value.w);
	}
	umf_real w;
    };

    /*!
    * \brief binary buffer with arbitrary content
    */
    struct umf_rawbuffer : public std::vector<char>
    {
    public:
        umf_rawbuffer() : std::vector<char>()
        { }

        umf_rawbuffer(const std::vector<char>& vec) : std::vector<char>(vec)
        { }

        umf_rawbuffer(std::vector<char>&& rVec) : std::vector<char>(rVec)
        { }

        umf_rawbuffer(const umf_rawbuffer& other) : std::vector<char>(other)
        { }

        umf_rawbuffer(umf_rawbuffer&& other) : std::vector<char>(static_cast< std::vector<char> && >(other))
        { }

        umf_rawbuffer& operator=(const umf_rawbuffer& other)
        {
            std::vector<char>::operator=( static_cast< const std::vector<char> & >(other) );
            return *this;
        }

        umf_rawbuffer& operator=(umf_rawbuffer&& other)
        {
            std::vector<char>::operator=( static_cast< std::vector<char> && >(other) );
            return *this;
        }

        umf_rawbuffer(const char* ptr, const size_t len)
        {
            *this = (ptr != nullptr) ? umf_rawbuffer(ptr, ptr + len) : umf_rawbuffer(len);
        }

        explicit umf_rawbuffer(const size_t len) : std::vector<char>(len)
        { }

        umf_rawbuffer(const char* ptrBegin, const char* ptrEnd) : std::vector<char>(ptrBegin, ptrEnd)
        { }
    };


} /* vmf */
#endif /* __VMF_TYPES_H__ */
