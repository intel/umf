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

#ifndef __VMF_EXCEPTIONS_H__
#define __VMF_EXCEPTIONS_H__

/*!
* \file exceptions.hpp
* \brief VMF cusrom exceptions
* \details Contains exception classes for several errors.
*/

#include <stdexcept>
#include <sstream>
#include "log.hpp"

/*!
* \def VMF_EXCEPTION
* \brief VMF exception throw macro
* \param [in] ex exception type
* \param[in] message text message that contains exception reason
* \details Throw an exception of specified class with text message
*/
#ifdef __GNUC__
#define VMF_EXCEPTION(ex, message ) do { VMF_LOG_ERROR(message); \
                                    throw ( ex(message, __func__, __FILE__, __LINE__) ); } while(0)
#else
#define VMF_EXCEPTION(ex, message ) do { VMF_LOG_ERROR(message); \
                                    throw ( ex(message, "", __FILE__, __LINE__) ); } while(0)
#endif

namespace vmf
{
/*! \class Exception
* \brief Base class for exceptions ierarchy
* \details 
*/
class Exception : public std::exception
{
public:
/*!
* \brief Class constructor
* \param [in] m message string that passed to exception
* \details Construct base exception class object
* with specified information message
*/
    Exception(const std::string& m) 
        : msg(m)
    {
    }

/*!
* \brief Constructor
*\ param[in] message Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
*/
    Exception(const std::string& message, const std::string& func, const std::string& file, int line)
         : msg(message), functionName(func), fileName(file), lineNumber(line) 
    {
    }

/*!
* \brief Return formatted message describing what caused the exception and where it was thrown
* \return Formatted message describing what caused the exceptionand where it was thrown
*/
    std::string getMessage() const throw()
    {
        std::stringstream msgStream;

        msgStream << fileName << ":" << lineNumber << " " << msg << " in function " << functionName << "\n";

        return msgStream.str();
    }

/*!
* \brief Function gets exception reason
* \returns exception reason string
*/
    virtual const char * what() const throw()
    {
        return msg.c_str();
    }

/*!
* \brief Class destructor
*/
    virtual ~Exception() throw()
    {
    }

private:
/*!
* \brief Message describing what caused the exception
*/
    std::string msg;

/*!
* \brief Function where the exception was thrown
*/
    std::string functionName;

/*!
* \brief File where the exception was thrown
*/
    std::string fileName;
    
/*!
* \brief Line where the exception was thrown
*/
    int lineNumber;
};

/*! \class IncorrectParamException
* \brief Throws as an exception when input parameter is incorrect
* \details 
*/
class IncorrectParamException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct IncorrectParamException class object
* with default exception reason "Incorrect parameter"
*/
    IncorrectParamException()
        : Exception("Incorrect parameter")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct IncorrectParamException class object
* with specified information message
*/
    IncorrectParamException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct IncorrectParamException class object 
* with specified information message
*/
    IncorrectParamException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class OutOfRangeException
* \brief Throws as an exception when input parameter is out of range
* \details 
*/
class OutOfRangeException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct OutOfRangeException class object
* with default exception reason "Value is out of range"
*/
    OutOfRangeException()
        : Exception("Value is out of range")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct OutOfRangeException class object
* with specified information message
*/
    OutOfRangeException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct OutOfRangeException class object 
* with specified information message
*/
    OutOfRangeException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class TypeCastException
* \brief Throws as an exception when type of element can't be cast to required type
* \details 
*/
class TypeCastException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct TypeCastException class object
* with default exception reason "Type cast exception occured"
*/
    TypeCastException()
        : Exception("Type cast exception occured")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct TypeCastException class object
* with specified information message
*/
    TypeCastException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct TypeCastException class object 
* with specified information message
*/
    TypeCastException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class NotInitializedException
* \brief Throws as an exception when object is not initialized
*/
class NotInitializedException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct NotInitializedException class object
* with default exception reason "Initialization required."
*/
    NotInitializedException()
        : Exception("Initialization required.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct NotInitializedException class object
* with specified information message
*/
    NotInitializedException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct NotInitializedException class object 
* with specified information message
*/
    NotInitializedException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class InternalErrorException
* \brief Throws as an exception when VMF library internal error occured
*/
class InternalErrorException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct InternalErrorException class object
* with default exception reason "Internal error occured."
*/
    InternalErrorException()
        : Exception("Internal error occured.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct InternalErrorException class object
* with specified information message
*/
    InternalErrorException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct InternalErrorException class object 
* with specified information message
*/
    InternalErrorException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class NullPointerException
* \brief Throws as an exception when pointer to object is null
*/
class NullPointerException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct NullPointerException class object
* with default exception reason "Pointer to object is NULL."
*/
    NullPointerException()
        : Exception("Pointer to object is NULL.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct NullPointerException class object
* with specified information message
*/
    NullPointerException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct NullPointerException class object 
* with specified information message
*/
    NullPointerException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class NotFoundException
* \brief Throws as an exception when file or object not found
*/
class NotFoundException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct NotFoundException class object
* with default exception reason "Object with specied identifier not found."
*/
    NotFoundException()
        : Exception("Object with specied identifier not found.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct NotFoundException class object
* with specified information message
*/
    NotFoundException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct NotFoundException class object 
* with specified information message
*/
    NotFoundException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class ObjectExpiredException
* \brief Throws as an exception when reference to and object expired
*/
class ObjectExpiredException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct ObjectExpiredException class object
* with default exception reason "Object already expired or destroyed."
*/
    ObjectExpiredException()
        : Exception("Object already expired or destroyed.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct ObjectExpiredException class object
* with specified information message
*/
    ObjectExpiredException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct ObjectExpiredException class object 
* with specified information message
*/
    ObjectExpiredException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class AssociationException
* \brief Throws as an exception when association error occured
*/
class AssociationException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct AssociationException class object
* with default exception reason "Object association error."
*/
    AssociationException()
        : Exception("Object association error.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct AssociationException class object
* with specified information message
*/
    AssociationException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct AssociationException class object 
* with specified information message
*/
    AssociationException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class DataStorageException
* \brief Throws as an exception when data provider error occured.
*/
class DataStorageException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct DataStorageException class object
* with default exception reason "Internal error of data storage library."
*/
    DataStorageException()
        : Exception("Internal error of data storage library.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct DataStorageException class object
* with specified information message
*/
    DataStorageException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct DataStorageException class object 
* with specified information message
*/
    DataStorageException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class NotImplementedException
* \brief Throws as an exception when specified functionality isn't implemented
*/
class NotImplementedException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct NotImplementedException class object
* with default exception reason "Functionality isn't implemented."
*/
    NotImplementedException()
        : Exception("Functionality isn't implemented.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct NotImplementedException class object
* with specified information message
*/
    NotImplementedException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct NotImplementedException class object 
* with specified information message
*/
    NotImplementedException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};

/*! \class ValidateException
* \brief Throws as an exception when metadata validation failed
*/
class ValidateException : public Exception
{
public:
/*!
* \brief Default class constructor
* \details Construct ValidateException class object
* with default exception reason "Validate operation failed."
*/
    ValidateException()
        : Exception("Validate operation failed.")
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
* \param [in] m message string that passed to exception
* \details Construct ValidateException class object
* with specified information message
*/
    ValidateException(const std::string& m)
        : Exception(m)
    {
    }

/*!
* \brief Class constructor with additional exception message parameter
*\ param[in] msg Message describing what caused the exception
*\ param[in] func Function where the exception was thrown
*\ param[in] file File where the exception was thrown
*\ param[in] line Line where the exception was thrown
* \details Construct ValidateException class object 
* with specified information message
*/
    ValidateException(const std::string& msg, const std::string& func, const std::string& file, int line)
         : Exception(msg, func, file, line)
    {
    }
};
} /* vmf */

#endif /* __VMF_EXCEPTIONS_H__ */
