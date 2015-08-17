// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __VMF_DATASOURCEFACTORY_HPP__
#define __VMF_DATASOURCEFACTORY_HPP__

/*!
 * \file datasourcefactory.hpp
 * \brief IDataSourceFactory interface header file
 */

#include "vmf/vmf.hpp"

namespace vmf
{

class IDataSource;

/*! \class IDataSourceFactory
 * \brief Interface for factory, which creates IDataSource instances
 */
class IDataSourceFactory
{
public:
    /*!
      \brief create new data IDataSource instance
     */
    virtual std::shared_ptr<IDataSource> createDataSource() = 0;
private:
};

} // namespace vmf

#endif // __VMF_DATASOURCEFACTORY_HPP__
