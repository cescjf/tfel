/*!
 * \file   MaterialException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 f�v 2007
 */

#include "Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    MaterialException::MaterialException(const std::string& s)
      :  tfel::exception::TFELException(s)
    {}
    
    const std::string 
    MaterialException::getName(void){
      return "MaterialException";
    }
    
    const std::string 
    DivergenceException::getName(void){
      return "DivergenceException";
    }

    DivergenceException::DivergenceException(const std::string& s)
      :  tfel::material::MaterialException(s)
    {}

    const std::string 
    OutOfBoundsException::getName(void){
      return "OutOfBoundsException";
    }

    OutOfBoundsException::OutOfBoundsException(const std::string& s)
      :  tfel::material::MaterialException(s)
    {}
  
  } // end of namespace material

} // end of namespace tfel
