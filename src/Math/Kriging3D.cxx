/*! 
 * \file  src/Math/Kriging3D.cxx
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

#include"TFEL/Math/Kriging3D.hxx"

namespace tfel
{

  namespace math
  {

    Kriging3D::Kriging3D(const std::vector<double>& vx,
			 const std::vector<double>& vy,
			 const std::vector<double>& vz,
			 const std::vector<double>& vv)
    {
      using namespace std;
      using tfel::math::tvector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      tvector<3u,double> v;
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      pair<double,double> n3 = KrigingUtilities::normalize(vz);
      this->a3 = n3.first;
      this->b3 = n3.second;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	v(0)=this->a1*(*px)+this->b1;
	v(1)=this->a2*(*py)+this->b2;
	v(2)=this->a3*(*pz)+this->b3;
	Kriging<3u,double>::addValue(v,*pv);
      }
      Kriging<3u,double>::buildInterpolation();
    }

    Kriging3D::Kriging3D(const tfel::math::vector<double>& vx,
			 const tfel::math::vector<double>& vy,
			 const tfel::math::vector<double>& vz,
			 const tfel::math::vector<double>& vv)
    {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      vector<double>::const_iterator pz;
      vector<double>::const_iterator pv;
      tvector<3u,double> v;
      if((vx.size()!=vy.size())||
	 (vx.size()!=vz.size())||
	 (vx.size()!=vv.size())){
	throw(KrigingErrorInvalidLength());
      }
      pair<double,double> n1 = KrigingUtilities::normalize(vx);
      this->a1 = n1.first;
      this->b1 = n1.second;
      pair<double,double> n2 = KrigingUtilities::normalize(vy);
      this->a2 = n2.first;
      this->b2 = n2.second;
      pair<double,double> n3 = KrigingUtilities::normalize(vz);
      this->a3 = n3.first;
      this->b3 = n3.second;
      for(px=vx.begin(),py=vy.begin(),pz=vz.begin(),pv=vv.begin();
	  px!=vx.end();++px,++py,++pz,++pv){
	v(0)=this->a1*(*px)+this->b1;
	v(1)=this->a2*(*py)+this->b2;
	v(2)=this->a3*(*pz)+this->b3;
	Kriging<3u,double>::addValue(v,*pv);
      }
      Kriging<3u,double>::buildInterpolation();
    }
  
    double
    Kriging3D::operator()(const double vx,
			  const double vy,
			  const double vz) const
    {
      using namespace tfel::math;
      tvector<3u,double> v;
      v(0)=this->a1*(vx)+this->b1;
      v(1)=this->a2*(vy)+this->b2;
      v(2)=this->a3*(vz)+this->b3;
      return Kriging<3u,double>::operator()(v);
    } // end of Kriging3D::operator()
      
    Kriging3D::~Kriging3D() = default;

  } // end of namespace math

} // end of namespace tfel
