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
    typedef char vmf_char;

    /*!
    * \brief 64-bit integer type.
    */
    typedef int64_t vmf_integer;

    /*!
    * \brief Double precesion floating point type.
    */
    typedef double vmf_real;

    /*!
    * \brief String type.
    */
    typedef std::string vmf_string;

    /*!
    * \brief floating point vector2D type.
    */
    struct vmf_vec2d
    {
	vmf_vec2d() : x(0), y(0) {}
	vmf_vec2d(vmf_real _x, vmf_real _y) : x(_x), y(_y) {}
	bool operator == (const vmf_vec2d& value) const
	{
	    return DOUBLE_EQ(x, value.x) && DOUBLE_EQ(y, value.y);
	}
	vmf_real x;
	vmf_real y;
    };

    /*!
    * \brief floating point vector3D type.
    */
    struct vmf_vec3d : vmf_vec2d
    {
	vmf_vec3d() : vmf_vec2d(), z(0){}
	vmf_vec3d(vmf_real _x, vmf_real _y, vmf_real _z) : vmf_vec2d(_x, _y), z(_z) {}
	bool operator == (const vmf_vec3d& value) const
	{
	    return vmf_vec2d::operator==(value) && DOUBLE_EQ(z, value.z);
	}
	vmf_real z;
    };

    /*!
    * \brief floating point vector4D type.
    */
    struct vmf_vec4d : vmf_vec3d
    {
	vmf_vec4d() : vmf_vec3d(), w(0){}
	vmf_vec4d(vmf_real _x, vmf_real _y, vmf_real _z, vmf_real _w) : vmf_vec3d(_x, _y, _z), w(_w) {}
	bool operator == (const vmf_vec4d& value) const
	{
	    return vmf_vec3d::operator==(value) && DOUBLE_EQ(w, value.w);
	}
	vmf_real w;
    };

    /*!
    * \brief binary buffer with arbitrary content
    */
    struct vmf_rawbuffer : public std::vector<char>
    {
    public:
        vmf_rawbuffer() : std::vector<char>()
        { }

        vmf_rawbuffer(const std::vector<char>& vec) : std::vector<char>(vec)
        { }

        vmf_rawbuffer(std::vector<char>&& rVec) : std::vector<char>(rVec)
        { }

        vmf_rawbuffer(const vmf_rawbuffer& other) : std::vector<char>(other)
        { }

        vmf_rawbuffer(vmf_rawbuffer&& other) : std::vector<char>(static_cast< std::vector<char> && >(other))
        { }

        vmf_rawbuffer& operator=(const vmf_rawbuffer& other)
        {
            std::vector<char>::operator=( static_cast< const std::vector<char> & >(other) );
            return *this;
        }

        vmf_rawbuffer& operator=(vmf_rawbuffer&& other)
        {
            std::vector<char>::operator=( static_cast< std::vector<char> && >(other) );
            return *this;
        }

        vmf_rawbuffer(const char* ptr, const size_t len)
        {
            *this = (ptr != nullptr) ? vmf_rawbuffer(ptr, ptr + len) : vmf_rawbuffer(len);
        }

        explicit vmf_rawbuffer(const size_t len) : std::vector<char>(len)
        { }

        vmf_rawbuffer(const char* ptrBegin, const char* ptrEnd) : std::vector<char>(ptrBegin, ptrEnd)
        { }
    };


} /* vmf */
#endif /* __VMF_TYPES_H__ */
