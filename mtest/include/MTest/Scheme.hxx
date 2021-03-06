/*!
 * \file   mtest/include/MTest/Scheme.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_SCHEME_HXX
#define LIB_MTEST_SCHEME_HXX

#include"TFEL/Tests/Test.hxx"
#include"MTest/Config.hxx"

namespace mtest{

  /*!
   * a scheme describe a specific kind of computation.
   */
  struct MTEST_VISIBILITY_EXPORT Scheme
    :  public tfel::tests::Test
  {
    //! desctructor
    virtual ~Scheme();
  }; // end of struct Scheme
  
} // end of namespace mtest

#endif /* LIB_MTEST_SCHEME_HXX */
