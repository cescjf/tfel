/*!
 * \file AreaComparison.hxx
 *
 *
 *  \class AreaComparison
 *  \brief Does an integration of the absolute difference of the two curves, and compares it to the expected error
 *  \date 12 juin 2013
 *  \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFELCHECK_AREACOMPARISON_HXX
#define LIB_TFELCHECK_AREACOMPARISON_HXX

#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFELCheck/Comparison.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT AreaComparison
    : public Comparison
  {
    AreaComparison();
    virtual void compare() override;
    virtual ~AreaComparison();
  };

} /* namespace tfel_check */

#endif /* LIB_TFELCHECK_AREACOMPARISON_HXX */
