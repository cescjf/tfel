/*! 
 * \file  src/Math/Kriging2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/Kriging2D.hxx"

namespace tfel
{

  namespace math
  {

    Kriging2D::Kriging2D(const std::vector<double>& vx,
			 const std::vector<double>& vy,
			 const std::vector<double>& vz)
    {
      using namespace std;
      using tfel::math::tvector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      tvector<2u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	v(0)=this->a1*(*px)+this->b1;
	v(1)=this->a2*(*py)+this->b2;
	Kriging<2u,double>::addValue(v,*pz);
      }
      Kriging<2u,double>::buildInterpolation();
    }

    Kriging2D::Kriging2D(const tfel::math::vector<double>& vx,
			 const tfel::math::vector<double>& vy,
			 const tfel::math::vector<double>& vz)
    {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      tvector<2u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin();
	  px!=vx.end();++px,++py,++pz){
	v(0)=this->a1*(*px)+this->b1;
	v(1)=this->a2*(*py)+this->b2;
	Kriging<2u,double>::addValue(v,*pz);
      }
      Kriging<2u,double>::buildInterpolation();
    }
  
    double
    Kriging2D::operator()(const double vx,
			  const double vy) const
    {
      using namespace tfel::math;
      tvector<2u,double> v;
      v(0)=this->a1*vx+this->b1;
      v(1)=this->a2*vy+this->b2;
      return Kriging<2u,double>::operator()(v);
    } // end of Kriging2D::operator()
      
    Kriging2D::~Kriging2D() = default;

  } // end of namespace math

} // end of namespace tfel
