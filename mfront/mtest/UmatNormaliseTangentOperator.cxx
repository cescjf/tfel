/*! 
 * \file  mfront/mtest/UmatNormaliseTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>
#include<cmath>
#include"MTest/UmatNormaliseTangentOperator.hxx"

namespace mfront{

  void
  UmatNormaliseTangentOperator::exe(tfel::math::matrix<real>& Kt,
					 const tfel::math::matrix<real>& D,
					 const unsigned short d)
  {
    using namespace std;
    using namespace tfel::math;
    static const real sqrt2 = sqrt(real(2));
    // transpose
    for(unsigned short i=0;i!=Kt.getNbRows();++i){
      for(unsigned short j=0;j!=Kt.getNbCols();++j){
    	Kt(i,j) = D(j,i);
      }
    }
    // normalise
    if(d==2u){
      Kt(0,3) *= sqrt2;
      Kt(1,3) *= sqrt2;
      Kt(2,3) *= sqrt2;
      Kt(3,0) *= sqrt2;
      Kt(3,1) *= sqrt2;
      Kt(3,2) *= sqrt2;
      Kt(3,3) *= 2;
    } else if (d==3u){
      Kt(0,3) *= sqrt2;
      Kt(1,3) *= sqrt2;
      Kt(2,3) *= sqrt2;
      Kt(0,4) *= sqrt2;
      Kt(1,4) *= sqrt2;
      Kt(2,4) *= sqrt2;
      Kt(0,5) *= sqrt2;
      Kt(1,5) *= sqrt2;
      Kt(2,5) *= sqrt2;
      Kt(3,0) *= sqrt2;
      Kt(3,1) *= sqrt2;
      Kt(3,2) *= sqrt2;
      Kt(4,0) *= sqrt2;
      Kt(4,1) *= sqrt2;
      Kt(4,2) *= sqrt2;
      Kt(5,0) *= sqrt2;
      Kt(5,1) *= sqrt2;
      Kt(5,2) *= sqrt2;
      Kt(3,3) *= 2;
      Kt(3,4) *= 2;
      Kt(3,5) *= 2;
      Kt(4,3) *= 2;
      Kt(4,4) *= 2;
      Kt(4,5) *= 2;
      Kt(5,3) *= 2;
      Kt(5,4) *= 2;
      Kt(5,5) *= 2;
    }
  } // end of UmatNormaliseTangentOperator::exe

} // end of namespace mfront
