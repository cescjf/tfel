/*!
 * \file   src/Math/BinaryFunction.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cerrno>
#include<stdexcept>

#include"TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      void StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException()
      {
	throw(std::runtime_error("StandardBinaryFunctionBase::"
				 "throwUnimplementedDifferentiateFunctionException: "
				 "unimplemented feature"));
      } // end of StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException

      void StandardBinaryFunctionBase::throwInvalidCallException(const int e)
      {
	throw(std::runtime_error("StandardBinaryFunctionBase::"
				 "throwInvalidCallException: "
				 "call to function failed "
				 "("+std::string(strerror(e))+")"));
      } // end of struct StandardBinaryFunctionBase::throwInvalidCallException

      BinaryFunction::~BinaryFunction() = default;

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
