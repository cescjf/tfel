/*! 
 * \file  mfront/mtest/SteffensenAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<limits>
#include<ostream>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/SteffensenAccelerationAlgorithm.hxx"

namespace mtest
{

  SteffensenAccelerationAlgorithm::SteffensenAccelerationAlgorithm()
    : stat(-1)
  {} // end of SteffensenAccelerationAlgorithm::SteffensenAccelerationAlgorithm
    
  std::string
  SteffensenAccelerationAlgorithm::getName() const
  {
    return "Steffensen";
  } // end of SteffensenAccelerationAlgorithm::getName

  void
  SteffensenAccelerationAlgorithm::setParameter(const std::string& p,
						     const std::string& v)
  {
    using namespace std;
    const string m = "SteffensenAccelerationAlgorithm::setParameter";
    if(p=="AccelerationTrigger"){
      const unsigned short i =
	AccelerationAlgorithm::convertToUnsignedShort(m,v);
      if(this->stat!=-1){
	string msg("SteffensenAccelerationAlgorithm::setParameter : "
		   "the castem acceleration trigger has already "
		   "been defined");
	throw(runtime_error(msg));
      }
      if(i<3){
	string msg("SteffensenAccelerationAlgorithm::setParameter",
		   "invalid acceleration trigger value.");
	throw(runtime_error(msg));
      }
      this->stat = i;
    } else {
      string msg("SteffensenAccelerationAlgorithm::setParameter : "
		 "invalid parameter '"+p+"'.");
      throw(runtime_error(msg));
    }
  } // end of SteffensenAccelerationAlgorithm::setParameter

  void
  SteffensenAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    this->sta_u0.resize(psz,0.);      
    this->sta_u1.resize(psz,0.);
    this->sta_u2.resize(psz,0.);
    this->sta_du1.resize(psz,0.);
    this->sta_du2.resize(psz,0.);
    if(this->stat==-1){
      this->stat=3;
    }
  } // end of SteffensenAccelerationAlgorithm::initialize

  void
  SteffensenAccelerationAlgorithm::preExecuteTasks()
  {} // end of SteffensenAccelerationAlgorithm::preExecuteTasks

  void
  SteffensenAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
						const tfel::math::vector<real>&,
						const tfel::math::vector<real>&,
						const real eeps,
						const real,
						const unsigned short iter)
  {
    using namespace std;
    const real it_eps = 100*(eeps*numeric_limits<real>::epsilon());
    this->sta_u0.swap(this->sta_u1);
    this->sta_u1.swap(this->sta_u2);
    this->sta_u2 = u1;
    if((iter>=this->stat)&&((iter-this->stat)%2==0)){
      if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
	auto& log = mfront::getLogStream();
	log << "Steffensen acceleration convergence" << endl;
      }
      this->sta_du2  = this->sta_u2-this->sta_u1;
      this->sta_du1  = this->sta_u1-this->sta_u0;
      for(vector<real>::size_type id=0;id!=this->sta_du1.size();++id){
	if((abs(this->sta_du2[id])>it_eps)&&
	   (abs(this->sta_du1[id])>it_eps)){
	  const auto i1 = 1/(this->sta_du2[id]);
	  const auto i2 = 1/(this->sta_du1[id]);
	  if(abs(i1-i2)>it_eps){
	    u1[id] =  this->sta_u1[id]+1/(i1-i2);
	  }
	}
      }
    }
  } // end of SteffensenAccelerationAlgorithm::execute

  void
  SteffensenAccelerationAlgorithm::postExecuteTasks()
  {} // end of SteffensenAccelerationAlgorithm::postExecuteTasks

  SteffensenAccelerationAlgorithm::~SteffensenAccelerationAlgorithm() = default;

} // end of namespace mtest
